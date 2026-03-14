#include "cmndm.h"
#include "transferorderdm.h"

#include "itransferorderhead.h"
#include "itransferorderposcollection.h"

#include "icscorderreleaser.h"
#include "iorderreleaseparameter.h"

#include "exceptions/orderreleaseexception.h"

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{
			const libutil::misc::ClassInfo& TransferOrderDM::getClassInfo()
			{
				static const libutil::misc::ClassInfo info("transferOrder::TransferOrderDM", libutil::misc::Version(1, 0, 0, 0));
				return info;
			}

			TransferOrderDM::TransferOrderDM()
				: m_Logger(libabbauw::LoggerPool::loggerTransferOrder)
			{
				METHODNAME_DEF(TransferOrderDM, TransferOrderDM)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			TransferOrderDM::~TransferOrderDM()
			{
				METHODNAME_DEF(TransferOrderDM, ~TransferOrderDM)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			void TransferOrderDM::injectTransactionFactory(libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr dbTransactionFactory)
			{
				m_DBTransactionFactory = dbTransactionFactory;
			}

			void TransferOrderDM::injectCscOrderReleaser(cscorder::ICSCOrderReleaserPtr cscOrderReleaser)
			{
				m_CscOrderReleaser = cscOrderReleaser;
			}

			void TransferOrderDM::injectHead(ITransferOrderHeadPtr transferorderhead)
			{
				m_TransferOrderHead = transferorderhead;
			}

			void TransferOrderDM::injectPosCollection(ITransferOrderPosCollectionPtr transferorderposcoll)
			{
				m_TransferOrderPosCollection = transferorderposcoll;
			}

			void TransferOrderDM::injectOrderReleaseParameter(parameter::IOrderReleaseParameterPtr parameter)
			{
				m_OrderReleaseParameterPtr = parameter;
			}

			ITransferOrderHeadPtr TransferOrderDM::getHead()
			{
				return m_TransferOrderHead;
			}

			ITransferOrderHeadPtr TransferOrderDM::getHead() const
			{
				return m_TransferOrderHead;
			}

			ITransferOrderPosCollectionPtr TransferOrderDM::getPosCollection()
			{
				return m_TransferOrderPosCollection;
			}

			ITransferOrderPosCollectionPtr TransferOrderDM::getPosCollection() const
			{
				return m_TransferOrderPosCollection;
			}

			const log4cplus::Logger& TransferOrderDM::getLogger() const
			{
				return m_Logger;
			}

			bool TransferOrderDM::isChanged() const
			{
				return getHead()->isChanged() || getPosCollection()->isChanged();
			}

			bool TransferOrderDM::isEmpty() const
			{
				return getHead()->isEmpty() || getPosCollection()->isEmpty();
			}

			void TransferOrderDM::findById(basar::Int32 orderNo)
			{
				getHead()->findById(orderNo);
				getPosCollection()->findById(orderNo);

				if (!isEmpty())
				{
					calculateOrderColor();
				}
			}

			void TransferOrderDM::findByIdWithoutLock(basar::Int32 orderNo)
			{
				// This method is used to avoid double locking in CPR0002688
				getHead()->findById(orderNo);
				getPosCollection()->findById(orderNo);

				if (!isEmpty())
				{
					calculateOrderColor();
				}
			}

			void TransferOrderDM::save()
			{
				METHODNAME_DEF(TransferOrderDM, save)
					BLOG_TRACE_METHOD(getLogger(), fun);

				libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction;

				try
				{
					transaction = m_DBTransactionFactory->createDBTransaction();
					transaction->begin();

					m_TransferOrderHead->save();
					m_TransferOrderPosCollection->setOrderNo(getOrderNo());
					m_TransferOrderPosCollection->save();

					transaction->commit();

					calculateOrderColor();
				}
				catch (const basar::Exception& e)
				{
					transaction->rollback();

					getLogger().forcedLog(log4cplus::ERROR_LOG_LEVEL, e.what(), __FILE__, __LINE__);
					throw;
				}
			}

			void TransferOrderDM::clear()
			{
				m_TransferOrderHead->clear();
				m_TransferOrderPosCollection->clear();
			}

			basar::Int32 TransferOrderDM::getOrderNo() const
			{
				return m_TransferOrderHead->getOrderNo();
			}

			basar::Int32 TransferOrderDM::getPositionCount() const
			{
				basar::Int32 count = 0;
				for (basar::db::aspect::AccessorPropertyTable_YIterator yitPos = getPosCollection()->get().begin(); !yitPos.isEnd(); ++yitPos)
				{
					if (!yitPos.getState().contains(basar::SS_DELETE))
					{
						count++;
					}
				}

				return count;
			}

			OrderStatus TransferOrderDM::getOrderStatus() const
			{
				return getHead()->getOrderStatus();
			}

			TrafficLightEnum TransferOrderDM::getColor() const
			{
				return static_cast<TrafficLightEnum>(getHead()->get().getInt16(properties::COLOR));
			}

			basar::Decimal TransferOrderDM::getOrderValue() const
			{
				basar::Decimal orderValue;
				for (basar::db::aspect::AccessorPropertyTable_YIterator yitPos = getPosCollection()->get().begin(); !yitPos.isEnd(); ++yitPos)
				{
					if (!yitPos.getState().contains(basar::SS_DELETE))
					{
						orderValue += yitPos.getDecimal(properties::WHOLESALERPURCHASEPRICE);
					}
				}

				return orderValue;
			}

			bool TransferOrderDM::hasOrderTypeOM() const
			{
				return getHead()->get().getString(properties::ORDERTYPE).substr(0, 2) == lit::OMG_ORDERTYPE;
			}

			basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderDM::addEmptyPosition()
			{
				return m_TransferOrderPosCollection->addEmptyPosition();
			}

			void TransferOrderDM::removePosition(basar::db::aspect::AccessorPropertyTable_YIterator yitPos)
			{
				METHODNAME_DEF(TransferOrderDM, removePosition)
					BLOG_TRACE_METHOD(getLogger(), fun);

				libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction;

				try
				{
					transaction = m_DBTransactionFactory->createDBTransaction();
					transaction->begin();

					getPosCollection()->removePosition(yitPos);
					if (getPosCollection()->isEmpty())
					{
						// when order doesn't contain items -> remove head too
						getHead()->markForDelete();

						if (getHead()->save())
						{
							transaction->commit();
						}
					}
					else
					{
						calculateOrderColor();
						transaction->commit();
					}
				}
				catch (const basar::Exception& e)
				{
					transaction->rollback();

					getLogger().forcedLog(log4cplus::ERROR_LOG_LEVEL, e.what(), __FILE__, __LINE__);
					throw;
				}
			}

			void TransferOrderDM::checkReleasable() const
			{
				METHODNAME_DEF(TransferOrderDM, checkReleasable)
					BLOG_TRACE_METHOD(getLogger(), fun);

				const basar::Int16 color = getHead()->get().getInt16(properties::COLOR);
				if ((color & libabbauw::domMod::transferOrder::PURPLE) == libabbauw::domMod::transferOrder::PURPLE)
				{
					basar::ExceptInfo info(fun, "Must not release purple orders!", __FILE__, __LINE__);
					throw exceptions::OrderReleaseException(info, exceptions::OrderReleaseException::ORDER_HAS_PURPLE_COLOR);
				}

				if (color == libabbauw::domMod::transferOrder::NO_COLOR)
				{
					// release of orders with no color is only allowed for OM orders
					basar::VarString orderType = getHead()->get().getString(properties::ORDERTYPE);
					if (orderType.substr(0, libabbauw::lit::OMG_ORDERTYPE.length()).compare(libabbauw::lit::OMG_ORDERTYPE) != 0)
					{
						basar::ExceptInfo info(fun, "Must not release orders without color!", __FILE__, __LINE__);
						throw exceptions::OrderReleaseException(info, exceptions::OrderReleaseException::ORDER_HAS_NO_COLOR);
					}
				}
			}

			void TransferOrderDM::checkForNonStockItem() const
			{
				METHODNAME_DEF(CSCOrderDM, checkForNonStockItems);

				if (getPosCollection()->containsNonStockItem())
				{
					QString warn = tr("Order '%1' contains at least one non stock item and therefore will not be released!").arg(getOrderNo());

					basar::ExceptInfo info(fun, warn.toLocal8Bit().constData(), __FILE__, __LINE__);
					throw exceptions::OrderReleaseException(info, exceptions::OrderReleaseException::ORDER_CONTAINS_NON_STOCK_ITEM);
				}
			}

			void TransferOrderDM::calculateOrderColor()
			{
				basar::Int16 color = NO_COLOR;
				for (basar::db::aspect::AccessorPropertyTable_YIterator yit = getPosCollection()->get().begin(); !yit.isEnd(); ++yit)
				{
					if (yit.isContainedAndSet(properties::COLOR))
					{
						color |= yit.getInt16(libabbauw::properties::COLOR);
					}
				}
				getHead()->get().setInt16(libabbauw::properties::COLOR, color);
			}

			void TransferOrderDM::release(bool postpone)
			{
				METHODNAME_DEF(TransferOrderDM, release)
					BLOG_TRACE_METHOD(getLogger(), fun);

				checkReleasable();
				checkForNonStockItem();

				//getHead()->get().setString( properties::STATUS, OrderStatus( OrderStatus::FE ).toString() );
				//getHead()->save();

				// when postpone is not forcibly set, check color of order to see if it has to be postponed or not
				basar::db::aspect::AccessorPropertyTable_YIterator yit = getHead()->get();
				bool isYellowOrRedOrder = (yit.getInt16(properties::COLOR) & libabbauw::domMod::transferOrder::YELLOW) == libabbauw::domMod::transferOrder::YELLOW ||
					(yit.getInt16(properties::COLOR) & libabbauw::domMod::transferOrder::RED) == libabbauw::domMod::transferOrder::RED;

				if (!postpone && isYellowOrRedOrder && m_OrderReleaseParameterPtr->releaseRedYellowOrdersPostponed())
				{
					postpone = true;
				}

				m_CscOrderReleaser->release(postpone);
			}
		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw
