#include "transferorderposuc.h"

#include "itransferorderposvc.h"
#include "libabbauw/domainmodule/orderproposal/iorderproposal.h"
#include "libabbauw/domainmodule/orderproposal/iorderproposalcreator.h"
#include "libabbauw/icustomerfinder.h"
#include "iusecasegetter.h"

#include "libabbauw/itransferordersplitdm.h"

#include "libabbauw/itransferorder.h"
#include "libabbauw/itransferorderhead.h"
#include "libabbauw/itransferorderposcollection.h"
#include "libabbauw/itransferorderposition.h"

#include "libabbauw/iparameteredit.h"

#include "libabbauw/domainmodule/orderprotocol/iorderprotocol.h"
#include "libabbauw/domainmodule/orderprotocol/orderprotocolenums.h"

#include "itransferorderprint.h"
#include "iprintprocessor.h"
#include "filenotopenableexception.h"
#include "filenotwritableexception.h"
#include "customerlockedexception.h"

#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include "libabbauw/exceptions/orderproposalexception.h"

#pragma warning (push)
#pragma warning(disable: 4231 4512 4800 )
#include <libutil/usecase.h>
#include <libutil/util.h>
#pragma warning (pop)

namespace CSC_Batches
{
	namespace useCase
	{
		BEGIN_HANDLERS_REGISTRATION(TransferOrderPosUC)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderPosUC, ShowORStatusOrder)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderPosUC, RemoveOrderPosRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderPosUC, PlaceOrderWishRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderPosUC, PlaceOrderWishShortfallRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderPosUC, SplitRedYellowRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderPosUC, SplitNarcoticsRefrigeratedRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderPosUC, PrintOrderRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderPosUC, CheckAllowedUserActionsRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(TransferOrderPosUC, TransferOrderPositionsChanged)
		END_HANDLERS_REGISTRATION()

			TransferOrderPosUC::TransferOrderPosUC() : m_OrderChanged(false)
		{
			BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, "TransferOrderPosUC::TransferOrderPosUC().");
		}

		TransferOrderPosUC::~TransferOrderPosUC()
		{
			BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, "TransferOrderPosUC::~TransferOrderPosUC().");
		}

		void TransferOrderPosUC::injectUseCaseGetter(componentManager::IUseCaseGetterPtr useCaseGetter)
		{
			m_UseCaseGetter = useCaseGetter;
		}

		void TransferOrderPosUC::injectVC(viewConn::ITransferOrderPosVCPtr transferOrderPosVCPtr)
		{
			m_TransferOrderPosVC = transferOrderPosVCPtr;
		}

		void TransferOrderPosUC::injectDM(libabbauw::domMod::transferOrder::ITransferOrderPtr transferOrderDM)
		{
			m_TransferOrderDMPtr = transferOrderDM;
		}

		void TransferOrderPosUC::injectDM(libabbauw::domMod::orderProposal::IOrderProposalPtr orderProposal)
		{
			m_OrderProposal = orderProposal;
		}

		void TransferOrderPosUC::injectDM(libabbauw::domMod::orderProposal::IOrderProposalCreatorPtr orderProposalCreator)
		{
			m_OrderProposalCreator = orderProposalCreator;
		}

		void TransferOrderPosUC::injectDM(libabbauw::domMod::customer::ICustomerFinderPtr customerFinder)
		{
			m_CustomerFinderDMPtr = customerFinder;
		}

		void TransferOrderPosUC::injectDM(libabbauw::domMod::orderProtocol::IOrderProtocolPtr orderProtocol)
		{
			m_OrderProtocol = orderProtocol;
		}

		void TransferOrderPosUC::injectDM(libabbauw::domMod::parameter::IParameterEditPtr dm)
		{
			m_ParameterEdit = dm;
		}

		void TransferOrderPosUC::injectColorSplitDM(libabbauw::domMod::transferOrder::ITransferOrderSplitDMPtr orderSplit)
		{
			m_TransferOrderColorSplitDMPtr = orderSplit;
		}

		void TransferOrderPosUC::injectNarcoticsRefrigeratedSplitDM(libabbauw::domMod::transferOrder::ITransferOrderSplitDMPtr orderSplit)
		{
			m_TransferOrderNarcoticsRefrigeratedSplitDMPtr = orderSplit;
		}

		void TransferOrderPosUC::injectPrintProcessor(::print::IPrintProcessorPtr printProcessorPtr)
		{
			m_PrintProcessorPtr = printProcessorPtr;
		}

		void TransferOrderPosUC::injectPrint(CSC_Batches::print::ITransferOrderPrintPtr print)
		{
			m_TransferOrderPrintPtr = print;
		}

		void TransferOrderPosUC::setIsOrderViewActive(bool isOrderViewActive)
		{
			m_IsOrderViewActive = isOrderViewActive;
		}

		void TransferOrderPosUC::searchCustomer()
		{
			m_CustomerFinderDMPtr->findByKey(m_TransferOrderDMPtr->getHead()->get().getInt32(libabbauw::properties::PHARMACYNO), true);
		}

		void TransferOrderPosUC::refreshView()
		{
			m_TransferOrderDMPtr->getPosCollection()->findById(m_TransferOrderDMPtr->getOrderNo());
			m_TransferOrderPosVC->matchFromTransferOrderPositions();
		}

		void TransferOrderPosUC::markOrderChanged()
		{
			m_OrderChanged = true;
		}

		bool TransferOrderPosUC::isReserveQtyDifferent()
		{
			if (m_ParameterEdit->getReserveQuantityOrderFactor() == m_ParameterEdit->getReserveQuantityReleaseFactor())
			{
				if (!m_ParameterEdit->isNotSalesEffectiveReserveCheckSet())
				{
					return false;
				}
				else if (m_ParameterEdit->getNotSalesEffectiveReserveQtyOrderFactor() == m_ParameterEdit->getNotSalesEffectiveReserveQtyReleaseFactor())
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			else
			{
				return true;
			}
		}

		void TransferOrderPosUC::createOrderProposalForPosition(const basar::Int32 posNo)
		{
			METHODNAME_DEF(TransferOrderPosUC, createOrderProposalForPosition)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);

			using libabbauw::domMod::orderProposal::IOrderProposalPtr;

			try
			{
				m_OrderProposalCreator->createOrderProposal(m_TransferOrderDMPtr, posNo);
			}
			catch (libabbauw::exceptions::CustomerLockedException& e)
			{
				BLOG_INFO(libabbauw::LoggerPool::loggerUseCases, e.reason());
				m_TransferOrderPosVC->info(tr("No order wish possible because customer is locked!").toLocal8Bit().constData());
			}
		}

		void TransferOrderPosUC::createOrderProposalShortfall(libabbauw::domMod::transferOrder::ITransferOrderPtr transferOrder)
		{
			METHODNAME_DEF(TransferOrderPosUC, createOrderProposalShortfall)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);
			transferOrder;

			using libabbauw::domMod::orderProposal::IOrderProposalPtr;

			try
			{
				m_OrderProposalCreator->createOrderProposals(m_TransferOrderDMPtr);
			}
			catch (libabbauw::exceptions::CustomerLockedException& e)
			{
				BLOG_INFO(libabbauw::LoggerPool::loggerUseCases, e.reason());
				m_TransferOrderPosVC->info(tr("No order wish possible because customer is locked!").toLocal8Bit().constData());
			}
		}

		void TransferOrderPosUC::eraseOrderProposalForPosition(const basar::Int32 posNo)
		{
			METHODNAME_DEF(TransferOrderPosUC, eraseOrderProposalForPosition)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);

			try
			{
				m_OrderProposal->resetSearchYit();
				basar::db::aspect::AccessorPropertyTable_YIterator searchYit = m_OrderProposal->getSearchYit();
				searchYit.setInt16(libabbauw::properties::BRANCHNO, m_TransferOrderDMPtr->getHead()->get().getInt16(libabbauw::properties::BRANCHNO));
				searchYit.setInt32(libabbauw::properties::ORDERNO, m_TransferOrderDMPtr->getOrderNo());
				searchYit.setInt32(libabbauw::properties::POSNO, posNo);

				m_OrderProposal->findByKey(searchYit);
				m_OrderProposal->erase();
			}
			catch (libabbauw::exceptions::OrderProposalException&)
			{
				basar::VarString msg = tr("A related order proposal cannot be cancelled!").toLocal8Bit().constData();
				m_TransferOrderPosVC->error(msg);
			}
		}

		void TransferOrderPosUC::checkAllowedUserActions()
		{
			m_TransferOrderPosVC->setDeletePositionAllowed(m_TransferOrderDMPtr->getOrderStatus().getStatus() == libabbauw::domMod::OrderStatus::ZU);
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderPosUC, RemoveOrderPosRequested)
		{
			METHODNAME_DEF(TransferOrderPosUC, RemoveOrderPosRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_OK);

			try
			{
				basar::db::aspect::AccessorPropertyTable_YIterator yit = m_TransferOrderPosVC->getSelectedOrderPosition();

				// if there's an unprocessed order proposal for this order position, it has to be deleted first
				eraseOrderProposalForPosition(yit.getInt32(libabbauw::properties::POSNO));

				// delete order position and head if order contains just one position
				yit.setInt32(libabbauw::properties::ORDERNO, m_TransferOrderDMPtr->getOrderNo());

				//Backup needed because we cant protocol it after it was deleted!
				m_OrderProtocol->setBackupHead(m_TransferOrderDMPtr->getHead()->get());
				m_OrderProtocol->setBackupPos(m_TransferOrderDMPtr->getPosCollection()->getPosition(yit)->get());

				m_TransferOrderDMPtr->removePosition(yit);

				m_OrderProtocol->protocolDeletePositon(m_TransferOrderDMPtr->isEmpty());

				markOrderChanged();

				if (m_TransferOrderDMPtr->isEmpty())
				{
					// close order position window because there are no remaining positions left
					m_TransferOrderPosVC->shutdown();
				}
				else
				{
					m_TransferOrderPosVC->matchFromTransferOrderPositions();
				}
			}
			catch (basar::Exception& e)
			{
				m_TransferOrderPosVC->error(e.what());
				result = HANDLER_ERROR;
				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderPosUC, PlaceOrderWishRequested)
		{
			METHODNAME_DEF(TransferOrderPosUC, PlaceOrderWishRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);

			try
			{
				basar::db::aspect::AccessorPropertyTable_YIterator yitPos = m_TransferOrderPosVC->getSelectedOrderPosition();
				if (m_TransferOrderDMPtr->getPosCollection()->getPosition(yitPos)->hasOrderProposalSet())
				{
					return result;
				}
				createOrderProposalForPosition(yitPos.getInt32(libabbauw::properties::POSNO));

				//Protocol it!
				if (m_OrderProtocol->isOrderProposalAllowed(yitPos))
				{
					m_OrderProtocol->protocolOrderRequest(yitPos, m_IsOrderViewActive);
				}

				markOrderChanged();
				refreshView();

				result = basar::appl::HANDLER_OK;
			}
			catch (basar::Exception& e)
			{
				BLOG_ERROR(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_TransferOrderPosVC->error(e.what());
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderPosUC, PlaceOrderWishShortfallRequested)
		{
			METHODNAME_DEF(TransferOrderPosUC, PlaceOrderWishShortfallRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);

			try
			{
				createOrderProposalShortfall(m_TransferOrderDMPtr);

				//Protocol it!
				basar::db::aspect::AccessorPropertyTable_YIterator yitPos = m_TransferOrderDMPtr->getPosCollection()->get().begin();
				while (!yitPos.isEnd())
				{
					if (m_OrderProtocol->isOrderProposalAllowed(yitPos))
					{
						m_OrderProtocol->protocolOrderRequest(yitPos, m_IsOrderViewActive);
					}

					++yitPos;
				}

				m_TransferOrderPosVC->matchFromTransferOrderPositions();
				markOrderChanged();

				result = basar::appl::HANDLER_OK;
			}
			catch (basar::Exception& e)
			{
				BLOG_ERROR(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_TransferOrderPosVC->error(e.what());
				refreshView();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderPosUC, PrintOrderRequested)
		{
			METHODNAME_DEF(TransferOrderPosUC, PrintOrderRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try
			{
				m_TransferOrderPrintPtr->setOrder(m_TransferOrderDMPtr);
				m_TransferOrderPrintPtr->setCustomer(m_CustomerFinderDMPtr);

				m_PrintProcessorPtr->previewPDF(m_TransferOrderPrintPtr);

				result = HANDLER_OK;
			}
			catch (::print::exceptions::FileNotOpenableException& e)
			{
				m_TransferOrderPosVC->error(e.what()); // TODO: Translated error msg
				result.message = e.what();
			}
			catch (::print::exceptions::FileNotWritableException& e)
			{
				m_TransferOrderPosVC->showFileStillOpenedInfo();
				result.message = e.what();
			}
			catch (basar::Exception& e)
			{
				m_TransferOrderPosVC->error(e.what());
				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderPosUC, ShowORStatusOrder)
		{
			METHODNAME_DEF(TransferOrderPosUC, ShowORStatusOrder)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try
			{
				m_UseCaseGetter->getBVOOrderStatusUC(m_TransferOrderDMPtr->getOrderNo(), m_IsOrderViewActive)->run();
				result = HANDLER_OK;
			}
			catch (basar::Exception& e) {
				m_TransferOrderPosVC->error(e.what());
				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderPosUC, SplitRedYellowRequested)
		{
			METHODNAME_DEF(TransferOrderPosUC, SplitRedYellowRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try
			{
				libabbauw::domMod::transferOrder::ITransferOrderPtr splittedOrder = m_TransferOrderColorSplitDMPtr->split(m_TransferOrderDMPtr);

				//Protocol the split process!
				m_OrderProtocol->protocolSplitOrder(m_TransferOrderDMPtr->getHead()->get(), m_TransferOrderDMPtr->getPosCollection()->get(),
					splittedOrder->getHead()->get(), splittedOrder->getPosCollection()->get(),
					libabbauw::domMod::orderProtocol::SPLIT_RED_YELLOW,
					m_IsOrderViewActive);

				m_TransferOrderPosVC->confirmOrderSplit(splittedOrder->getOrderNo(), splittedOrder->getPositionCount());
				markOrderChanged();
				m_TransferOrderPosVC->matchFromTransferOrderPositions();

				result = HANDLER_OK;
			}
			catch (basar::Exception& e)
			{
				m_TransferOrderPosVC->error(e.what());
				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderPosUC, SplitNarcoticsRefrigeratedRequested)
		{
			METHODNAME_DEF(TransferOrderPosUC, SplitNarcoticsRefrigeratedRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try
			{
				libabbauw::domMod::transferOrder::ITransferOrderPtr splittedOrder = m_TransferOrderNarcoticsRefrigeratedSplitDMPtr->split(m_TransferOrderDMPtr);

				//Protocol the split process!
				m_OrderProtocol->protocolSplitOrder(m_TransferOrderDMPtr->getHead()->get(), m_TransferOrderDMPtr->getPosCollection()->get(),
					splittedOrder->getHead()->get(), splittedOrder->getPosCollection()->get(),
					libabbauw::domMod::orderProtocol::SPLIT_BTM_K,
					m_IsOrderViewActive);

				m_TransferOrderPosVC->confirmOrderSplit(splittedOrder->getOrderNo(), splittedOrder->getPositionCount());
				markOrderChanged();
				m_TransferOrderPosVC->matchFromTransferOrderPositions();

				result = HANDLER_OK;
			}
			catch (basar::Exception& e)
			{
				m_TransferOrderPosVC->error(e.what());
				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderPosUC, CheckAllowedUserActionsRequested)
		{
			METHODNAME_DEF(TransferOrderPosUC, CheckAllowedUserActionsRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			try
			{
				checkAllowedUserActions();

				result = HANDLER_OK;
			}
			catch (basar::Exception& e)
			{
				m_TransferOrderPosVC->error(e.what());
				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(TransferOrderPosUC, TransferOrderPositionsChanged)
		{
			METHODNAME_DEF(TransferOrderPosUC, TransferOrderPositionsChanged)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			using namespace basar::appl;
			EventReturnStruct result(HANDLER_ERROR);

			// refresh
			m_TransferOrderDMPtr->findById(m_TransferOrderDMPtr->getOrderNo());
			m_TransferOrderPosVC->matchFromTransferOrderPositions();
			markOrderChanged();

			result = HANDLER_OK;
			return result;
		}

		void TransferOrderPosUC::run()
		{
			METHODNAME_DEF(TransferOrderPosUC, run)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);

			registerEventHandlers(true);

			m_OrderChanged = false;

			try
			{
				m_TransferOrderPosVC->init(m_TransferOrderDMPtr->getPosCollection()->get(), isReserveQtyDifferent(), m_IsOrderViewActive);

				m_TransferOrderPosVC->setSplitNarcoticsRefrigeratedAllowed(m_TransferOrderNarcoticsRefrigeratedSplitDMPtr->isSplittable(m_TransferOrderDMPtr));
				m_TransferOrderPosVC->setSplitRedYellowAllowed(m_TransferOrderColorSplitDMPtr->isSplittable(m_TransferOrderDMPtr));
				m_TransferOrderPosVC->show(); // non blocking

				searchCustomer();

				m_TransferOrderPosVC->matchFromPharmacy(m_CustomerFinderDMPtr->get());
				m_TransferOrderPosVC->matchFromTransferOrder(m_TransferOrderDMPtr->getHead()->get());
				m_TransferOrderPosVC->matchFromTransferOrderPositions();

				checkAllowedUserActions();
			}
			catch (basar::Exception& e)
			{
				m_TransferOrderPosVC->error(e.what());
			}

			m_TransferOrderPosVC->waitForEvents();

			if (m_OrderChanged)
			{
				basar::appl::SystemEventManager::getInstance().fire("TransferOrderChanged");
			}

			registerEventHandlers(false);
		}
	} // end namespace useCase
} // end namespace CSC_Batches
