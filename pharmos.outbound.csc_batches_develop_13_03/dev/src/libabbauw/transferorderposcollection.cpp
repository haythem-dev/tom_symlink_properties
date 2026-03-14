#include "cmndm.h"

#include "transferorderposcollection.h"
#include "transferorderposcollection_definitions.h"

#include "transferorderposcollectionacc.h"
#include "istockcollectiondm.h"
#include "iordertypegetter.h"
#include "trafficlightcolorsenum.h"

#include "componentmanager/itransferorderpositionconnectedfactory.h"

#include "transferorderposcollectionpropertystatechecker.h"

#include "bitfieldresolver.h"

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{

			const libutil::misc::ClassInfo& TransferOrderPosCollection::getClassInfo()
			{
				static const libutil::misc::ClassInfo info("transferOrder::TransferOrderPosCollection", libutil::misc::Version(1, 0, 0, 0));
				return info;
			}

			TransferOrderPosCollection::TransferOrderPosCollection(basar::db::aspect::ConnectionRef connection)
				: DMBase(connection, domMod::transferOrder::ACC_TRANSFER_ORDER_POS_COLLECTION, libabbauw::LoggerPool::loggerTransferOrder)
			{
				METHODNAME_DEF(TransferOrderPosCollection, TransferOrderPosCollection)
					BLOG_TRACE_METHOD(getLogger(), fun);

				// avoid runtime error
				acc_transfer_order_pos::SelectTransferOrderPosCollection();
			}

			TransferOrderPosCollection::~TransferOrderPosCollection()
			{
				METHODNAME_DEF(TransferOrderPosCollection, ~TransferOrderPosCollection)
					BLOG_TRACE_METHOD(getLogger(), fun);

				shutdown();
			}

			void TransferOrderPosCollection::injectStockCollection(stock::IStockCollectionDMPtr stockColl)
			{
				m_StockCollection = stockColl;
			}

			void TransferOrderPosCollection::injectOrderTypeGetter(IOrderTypeGetterPtr orderTypeGetter)
			{
				m_OrderType = orderTypeGetter;
			}

			void TransferOrderPosCollection::injectTransferOrderPositionConnectedFactory(componentManager::ITransferOrderPositionConnectedFactoryPtr factory)
			{
				m_TransferOrderPositionConnectedFactory = factory;
			}

			AccessorExecuteParameterization TransferOrderPosCollection::getFindByPatternExecuteParameters() const
			{
				METHODNAME_DEF(TransferOrderPosCollection, getFindByPatternExecuteParameters)
					BLOG_TRACE_METHOD(getLogger(), fun);

				return AccessorExecuteParameterization(transferOrder::SELECT_TRANSFER_ORDER_POS_COLLECTION);
			}

			basar::VarString TransferOrderPosCollection::getSearchYIteratorAttributeNames() const
			{
				METHODNAME_DEF(TransferOrderPosCollection, getSearchYIteratorAttributeNames)
					BLOG_TRACE_METHOD(getLogger(), fun);

				using namespace libabbauw::properties;

				return libabbauw::properties::BRANCHNO.toDescriptionString() +
					libabbauw::properties::ORDERNO.toDescriptionString()
					;
			}

			void TransferOrderPosCollection::doInit(const TransferOrderPosInitParams& p)
			{
				METHODNAME_DEF(TransferOrderPosCollection, doInit)
					BLOG_TRACE_METHOD(getLogger(), fun);

				m_InitParams = p;
			}

			void TransferOrderPosCollection::doShutdown()
			{
				METHODNAME_DEF(TransferOrderPosCollection, doShutdown)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			void TransferOrderPosCollection::resetSearchYit()
			{
				DMBase::resetSearchYit();
			}

			basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderPosCollection::getSearchYit()
			{
				return DMBase::getSearchYit();
			}

			void TransferOrderPosCollection::findById(basar::Int32 orderNo)
			{
				METHODNAME_DEF(TransferOrderPosCollection, findById)
					BLOG_TRACE_METHOD(getLogger(), fun);

				resetSearchYit();
				basar::db::aspect::AccessorPropertyTable_YIterator searchYit = getSearchYit();
				searchYit.setInt16(properties::BRANCHNO, m_InitParams.m_BranchNo);
				searchYit.setInt32(properties::ORDERNO, orderNo);

				DMBase::findByPattern(searchYit);

				resolveBitFields(DMBase::getAccessor()->getPropertyTable());

				calculateMissingItems();
			}

			void TransferOrderPosCollection::calculateMissingItems()
			{
				METHODNAME_DEF(TransferOrderPosCollection, calculateMissingItems)
					BLOG_TRACE_METHOD(getLogger(), fun);

				basar::Int32 orderNo = getSearchYit().getInt32(properties::ORDERNO);

				for (basar::db::aspect::AccessorPropertyTable_YIterator iter = get().begin(); !iter.isEnd(); ++iter)
				{
					basar::Int32 position = iter.getInt32(properties::POSNO);
					basar::db::aspect::AccessorPropertyTable_YIterator stockIter = m_StockCollection->findOrderPositionInOrder(orderNo, position);

					// the colors of positions are calculated here
					if ((!stockIter.isEnd()) && stockIter.isContainedAndSet(properties::STOCKACCUMULATED))
					{
						basar::Int32 stockAccumulated = stockIter.getInt32(properties::STOCKACCUMULATED);
						iter.setInt16(properties::COLOR, stockIter.getInt16(properties::COLOR));
						basar::Int32 orderedQty = stockIter.getInt32(properties::ORDEREDQTY);

						if (stockAccumulated < 0)
						{
							basar::Int32 stock = 0;
							stockAccumulated *= -1;
							if (orderedQty <= stockAccumulated)
								stock = orderedQty;
							else
								stock = stockAccumulated;

							iter.setInt32(properties::SHORTFALLQTYACCUMULATED, stockAccumulated);
							iter.setInt32(properties::SHORTFALLQTY, stock);
						}

						++stockIter;
					}
					else
					{
						if (m_OrderType->hasOrderTypeOM() && (!m_StockCollection->itemInStock(iter) || m_StockCollection->itemInTransitStorage(iter)))
							iter.setInt16(properties::COLOR, PURPLE); // only mark those OM datasets which are not in stock
						else
							iter.setInt16(properties::COLOR, NO_COLOR);
					}
				}
			}

			void TransferOrderPosCollection::resolveBitFields(basar::db::aspect::AccessorPropertyTableRef posCollection)
			{
				METHODNAME_DEF(TransferOrderPosCollection, resolveBitFields)
					BLOG_TRACE_METHOD(getLogger(), fun);

				basar::db::aspect::AccessorPropertyTable_YIterator pos = posCollection.begin();

				//Iterate through all positions
				while (!pos.isEnd())
				{
					resolveDeliveryInfo(pos);
					resolveNotice(pos);

					++pos;
				}
			}

			void TransferOrderPosCollection::resolveDeliveryInfo(basar::db::aspect::AccessorPropertyTable_YIterator pos)
			{
				METHODNAME_DEF(TransferOrderPosCollection, resolveDeliveryInfo)
					BLOG_TRACE_METHOD(getLogger(), fun);

				basar::VarString deliveryNote = "";
				basar::Int32	 bitField1 = 0;
				basar::Int32	 bitField2 = 0;

				//Add artikellokal.etartschalter1 + artikelzentral.etartschalter1 to one bitfield,
				//because the information for the delivery note could be in both!
				bitField1 = pos.getInt32(properties::LOCALARTICLEBITFIELD) + pos.getInt32(properties::CENTRALARTICLEBITFIELD);
				bitField2 = pos.getInt32(properties::LOCALARTICLEBITFIELD2) + pos.getInt32(properties::CENTRALARTICLEBITFIELD2);

				deliveryNote = util::BitFieldResolver().resolveDeliveryNote(bitField1, bitField2, pos.getInt32(properties::STOCK));

				pos.setString(properties::DELIVERYNOTE, deliveryNote);
			}

			void TransferOrderPosCollection::resolveNotice(basar::db::aspect::AccessorPropertyTable_YIterator pos)
			{
				METHODNAME_DEF(TransferOrderPosCollection, resolveNotice)
					BLOG_TRACE_METHOD(getLogger(), fun);

				basar::VarString notice = "";

				notice = util::BitFieldResolver().resolveNotice(pos.getInt32(properties::LOCALARTICLEBITFIELD), pos.getInt32(properties::ARTICLECLASS));
				pos.setString(properties::NOTICE, notice);
			}

			void TransferOrderPosCollection::clear()
			{
				get().clear();
			}

			basar::db::aspect::AccessorPropertyTableRef TransferOrderPosCollection::get()
			{
				return DMBase::getPropTab();
			}

			basar::db::aspect::AccessorPropertyTableRef TransferOrderPosCollection::get() const
			{
				return DMBase::getPropTab();
			}

			ITransferOrderPositionPtr TransferOrderPosCollection::getPosition(const basar::db::aspect::AccessorPropertyTable_YIterator yit)
			{
				return m_TransferOrderPositionConnectedFactory->create(yit);
			}

			basar::db::aspect::AccessorPropertyTable_YIterator TransferOrderPosCollection::addEmptyPosition()
			{
				basar::db::aspect::AccessorPropertyTable_YIterator posYit = get().insert(basar::FOR_INSERT);

				posYit.setInt16(properties::BRANCHNO, m_InitParams.m_BranchNo);
				posYit.setInt32(properties::ORDEREDQTY, 0);
				posYit.setInt32(properties::NONCHARGEDQTY, 0);
				posYit.setDecimal(properties::DISCOUNTPCT, basar::Decimal(0.0));
				posYit.setString(properties::DISCOUNTTYPE, "");
				posYit.setInt16(properties::DISCOUNTCALCFROM, 0);
				posYit.setInt16(properties::DISCOUNTAPPLYTO, 1);
				posYit.setDecimal(properties::GEP, basar::Decimal(0.00));
				posYit.setDecimal(properties::AGP, basar::Decimal(0.00));

				return posYit;
			}

			void TransferOrderPosCollection::removePosition(const basar::db::aspect::AccessorPropertyTable_YIterator yIt)
			{
				METHODNAME_DEF(TransferOrderPosCollection, removePosition)
					BLOG_TRACE_METHOD(getLogger(), fun);

				basar::db::aspect::AccessorPropertyTable_YIterator yitSearchTmp = yIt;
				yitSearchTmp.setInt16(libabbauw::properties::BRANCHNO, m_InitParams.m_BranchNo);

				try
				{
					const bool clearBeforeSelect = true;
					const bool flagExcept = true;
					const basar::db::aspect::DatabaseHintEnum dbHint = basar::db::aspect::HOLD_RECORDSET;

					basar::db::aspect::ExecuteResultInfo res = getAccessor()->execute(DELETE_POSITION_FROM_POS_COLLECTION, yitSearchTmp,
						clearBeforeSelect, flagExcept, dbHint);
					if (res.hasError())
					{
						THROW_ACCESSOR_EXECUTION_EXCEPTION(fun, DELETE_POSITION_FROM_POS_COLLECTION, res);
					}

					get().erase(yIt);

				}
				catch (basar::Exception& e)
				{
					getLogger().forcedLog(log4cplus::ERROR_LOG_LEVEL, e.what(), __FILE__, __LINE__);
					throw;
				}
			}

			bool TransferOrderPosCollection::isChanged() const
			{
				bool changed = false;
				for (basar::db::aspect::AccessorPropertyTable_YIterator yit = get().begin(); !yit.isEnd() && !changed; ++yit)
				{
					accessor::transferOrder::PosCollectionPropertyStateChecker checker(yit);
					if (checker.isSavable())
						changed = true;
				}

				return changed;
			}

			bool TransferOrderPosCollection::isEmpty() const
			{
				return DMBase::isEmpty();
			}

			bool TransferOrderPosCollection::containsNonStockItem() const
			{
				bool nonStockItem = false;
				if (!isEmpty())
				{
					nonStockItem = m_StockCollection->orderContainsNonStockItem(get().begin().getInt32(properties::ORDERNO));
				}

				return nonStockItem;
			}

			void TransferOrderPosCollection::setOrderNo(basar::Int32 orderNo)
			{
				getSearchYit().setInt32(properties::ORDERNO, orderNo);

				for (basar::db::aspect::AccessorPropertyTable_YIterator yit = get().begin(); !yit.isEnd(); ++yit)
				{
					yit.setInt32(properties::ORDERNO, orderNo);
				}
			}

			void TransferOrderPosCollection::markForDelete()
			{
				METHODNAME_DEF(TransferOrderPosCollection, markForDelete)
					BLOG_TRACE_METHOD(getLogger(), fun);

				for (basar::db::aspect::AccessorPropertyTable_YIterator yit = get().begin(); !yit.isEnd(); ++yit)
				{
					get().markForDelete(yit);
				}
			}

			void TransferOrderPosCollection::save()
			{
				METHODNAME_DEF(TransferOrderPosCollection, save)
					BLOG_TRACE_METHOD(getLogger(), fun);

				for (basar::db::aspect::AccessorPropertyTable_YIterator yit = get().begin(); !yit.isEnd(); ++yit)
				{
					accessor::transferOrder::PosCollectionPropertyStateChecker checker(yit);
					if (checker.isSavable())
					{
						try
						{
							basar::db::aspect::ExecuteResultInfo res = getAccessor()->execute(SAVE_TRANSFER_ORDER_POS_COLLECTION, yit);
							if (res.hasError())
							{
								THROW_ACCESSOR_EXECUTION_EXCEPTION(fun, SAVE_TRANSFER_ORDER_POS_COLLECTION, res);
							}
						}
						catch (basar::Exception& e)
						{
							getLogger().forcedLog(log4cplus::ERROR_LOG_LEVEL, e.what(), __FILE__, __LINE__);
							throw;
						}
					}
				}
			}

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw
