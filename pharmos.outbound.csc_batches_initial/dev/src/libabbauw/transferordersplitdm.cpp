#include "cmndm.h"

#include "transferordersplitdm.h"
#include "isplittingpolicy.h"

#include "componentmanager/icreatesplittedheadfactory.h"

#include "itransferorder.h"
#include "itransferorderhead.h"
#include "itransferorderposcollection.h"
#include "itransferorderposition.h"
#include "domainmodule/orderproposal/iorderproposal.h"

#include "exceptions/ordersplitexception.h"

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{
			const libutil::misc::ClassInfo& TransferOrderSplitDM::getClassInfo()
			{
				static const libutil::misc::ClassInfo info("transferOrder::TransferOrderSplitDM", libutil::misc::Version(1, 0, 0, 0));
				return info;
			}

			TransferOrderSplitDM::TransferOrderSplitDM()
				: m_Logger(libabbauw::LoggerPool::loggerTransferOrder)
			{
				METHODNAME_DEF(TransferOrderSplitDM, TransferOrderSplitDM)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			TransferOrderSplitDM::~TransferOrderSplitDM()
			{
				METHODNAME_DEF(TransferOrderSplitDM, ~TransferOrderSplitDM)
					BLOG_TRACE_METHOD(getLogger(), fun);
			}

			void TransferOrderSplitDM::injectTransactionFactory(libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr dbTransactionFactory)
			{
				m_DBTransactionFactory = dbTransactionFactory;
			}

			void TransferOrderSplitDM::injectCreateSplittedHeadFactory(componentManager::ICreateSplittedHeadFactoryPtr splittedHeadFactory)
			{
				m_CreateSplittedHeadFactory = splittedHeadFactory;
			}

			void TransferOrderSplitDM::injectSplittingPolicy(splittingPolicy::ISplittingPolicyPtr splittingPolicy)
			{
				m_SplittingPolicy = splittingPolicy;
			}

			void TransferOrderSplitDM::injectOrderProposal(orderProposal::IOrderProposalPtr orderProp)
			{
				m_OrderProposal = orderProp;
			}

			void TransferOrderSplitDM::copyPositionToOrder(const basar::db::aspect::AccessorPropertyTable_YIterator yitPos,
				transferOrder::ITransferOrderPtr order)
			{
				// copy position to new order
				basar::db::aspect::AccessorPropertyTable_YIterator newPosYit = order->addEmptyPosition();

				newPosYit.match(yitPos);
				newPosYit.setPropertyState(properties::ORDERNO, basar::SS_UNSET); // orderNo should not be copied
			}

			void TransferOrderSplitDM::setReference(basar::db::aspect::AccessorPropertyTable_YIterator yitHead, basar::Int32 orderNo)
			{
				// set reference field to indicate that this order was splitted from another one
				std::ostringstream reference;
				reference << yitHead.getString(properties::REFERENCE)
					<< "Splitted_from:" << orderNo; // store orderno of original order
				yitHead.setString(properties::REFERENCE, reference.str().c_str());
			}

			void TransferOrderSplitDM::setDeliveryText(basar::db::aspect::AccessorPropertyTable_YIterator yitPos)
			{
				std::ostringstream text;
				text << yitPos.getInt32(properties::ORDEREDQTY);

				// Do not add NR if it was not set. DE uses it very seldom. (KSC-56 and Issue I-36)
				if (0 < yitPos.getInt32(properties::NONCHARGEDQTY))
				{
					text << "+" << yitPos.getInt32(properties::NONCHARGEDQTY);
				}

				text << " " << tr("DELIVERED IN ADD.(SPLIT)").toLocal8Bit().constData();
				yitPos.setString(properties::TEXTFIELD, text.str().substr(0, 29).c_str()); // limit to 29 chars

			}

			void TransferOrderSplitDM::resetPositionToZero(basar::db::aspect::AccessorPropertyTable_YIterator yitPos)
			{
				yitPos.setInt32(properties::ORDEREDQTY, 0);
				yitPos.setInt32(properties::NONCHARGEDQTY, 0);
				yitPos.setDecimal(properties::DISCOUNTPCT, basar::Decimal(0));
				yitPos.setInt16(properties::COLOR, GREEN);
				yitPos.setString(properties::ENTRYTYPE, ""); //TODO DZ

				// delete old calculation
				yitPos.setPropertyState(properties::SHORTFALLQTY, basar::SS_UNSET);
			}

			void TransferOrderSplitDM::processOrderProposals(transferOrder::ITransferOrderPtr newOrder, basar::Int32 previousOrderNo)
			{
				for (basar::db::aspect::AccessorPropertyTable_YIterator yitPos = newOrder->getPosCollection()->get().begin(); !yitPos.isEnd(); ++yitPos)
				{
					transferOrder::ITransferOrderPositionPtr position = newOrder->getPosCollection()->getPosition(yitPos);
					if (position->hasOrderProposalSet())
					{
						// re-reference orderproposal to splitted order
						m_OrderProposal->resetSearchYit();
						basar::db::aspect::AccessorPropertyTable_YIterator searchYit = m_OrderProposal->getSearchYit();
						searchYit.setInt16(properties::BRANCHNO, newOrder->getHead()->get().getInt16(properties::BRANCHNO));
						searchYit.setInt32(properties::ORDERNO, previousOrderNo);
						searchYit.setInt32(properties::POSNO, yitPos.getInt32(properties::POSNO));

						m_OrderProposal->findByKey(searchYit);
						if (!m_OrderProposal->isEmpty())
						{
							m_OrderProposal->setOrderNo(newOrder->getOrderNo());
							m_OrderProposal->save();
						}
					}
				}
			}

			transferOrder::ITransferOrderPtr TransferOrderSplitDM::split(transferOrder::ITransferOrderPtr transferOrder)
			{
				METHODNAME_DEF(TransferOrderSplitDM, split)
					BLOG_TRACE_METHOD(getLogger(), fun);

				if (!isSplittable(transferOrder))
				{
					throw exceptions::OrderSplitException(basar::ExceptInfo(fun, "Splitting not allowed!", __FILE__, __LINE__));
				}

				// create copy of original transfer order without positions and without orderno (because it's a copy it will get a new one later)
				transferOrder::ITransferOrderPtr newTransferOrder = m_CreateSplittedHeadFactory->createSplittedHead(transferOrder);
				// set reference field to indicate that this order was splitted from another one
				setReference(newTransferOrder->getHead()->get(), transferOrder->getHead()->getOrderNo());

				// split positions according to split policy
				for (basar::db::aspect::AccessorPropertyTable_YIterator yit = transferOrder->getPosCollection()->get().begin(); !yit.isEnd(); ++yit)
				{
					if (m_SplittingPolicy->shouldPositionBeTransfered(yit))
					{
						copyPositionToOrder(yit, newTransferOrder);
						setDeliveryText(yit);
						resetPositionToZero(yit);
					}
				}

				libutil::infrastructure::db::dbTransaction::IDBTransactionPtr transaction = m_DBTransactionFactory->createDBTransaction();

				try
				{
					transaction->begin();

					transferOrder->save();
					newTransferOrder->save();
					processOrderProposals(newTransferOrder, transferOrder->getOrderNo());

					transaction->commit();
				}
				catch (const basar::Exception& e)
				{
					transaction->rollback();

					getLogger().forcedLog(log4cplus::ERROR_LOG_LEVEL, e.what(), __FILE__, __LINE__);
					throw;
				}

				return newTransferOrder;
			}

			bool TransferOrderSplitDM::isSplittable(transferOrder::ITransferOrderPtr transferOrder)
			{
				return m_SplittingPolicy->isSplittable(transferOrder);
			}

			const log4cplus::Logger& TransferOrderSplitDM::getLogger() const
			{
				return m_Logger;
			}

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw
