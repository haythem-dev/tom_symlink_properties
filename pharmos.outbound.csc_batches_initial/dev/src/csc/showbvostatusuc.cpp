//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "showbvostatusuc.h"

#include "icsvexportgetter.h"
#include "icsvdataexportservice.h"

#include "domainmodule/orderproposal/iorderproposalcollectionfinder.h"
#include "domainmodule/orderproposal/iorderproposal.h"
#include "domainmodule/orderprotocol/iorderprotocol.h"
#include "ibvostatusvc.h"

#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include "exceptions/orderproposalexception.h"

#pragma warning (push)
#pragma warning(disable: 4127 4231 4512 4800 )
#include <QtWidgets/QApplication>

#include <libutil/usecase.h>
#include <libutil/util.h>
#pragma warning (pop)

namespace CSC_Batches
{
	namespace useCase
	{
		BEGIN_HANDLERS_REGISTRATION(ShowBVOStatusUC)
			SYSTEM_EVENT_HANDLER_REGISTRATION(ShowBVOStatusUC, FindOrderProposalsRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(ShowBVOStatusUC, CancelOrderProposal)
			SYSTEM_EVENT_HANDLER_REGISTRATION(ShowBVOStatusUC, ExportCSVRequested);
		END_HANDLERS_REGISTRATION()

			ShowBVOStatusUC::ShowBVOStatusUC() : m_OrdersChanged(false)
		{
			METHODNAME_DEF(ShowBVOStatusUC, ShowBVOStatusUC);
			BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);
		}

		ShowBVOStatusUC::~ShowBVOStatusUC()
		{
			METHODNAME_DEF(ShowBVOStatusUC, ~ShowBVOStatusUC);
			BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);
		}

		void ShowBVOStatusUC::setSession(libutil::infrastructure::session::ISessionPtr session)
		{
			m_Session = session;
		}

		void ShowBVOStatusUC::injectCSVExportGetter(componentManager::ICSVExportGetterPtr csvExportGetter)
		{
			m_CSVExportGetter = csvExportGetter;
		}

		void ShowBVOStatusUC::injectVC(viewConn::IBVOStatusVCPtr bvoStatusVC)
		{
			m_BVOStatusVC = bvoStatusVC;
		}

		void ShowBVOStatusUC::injectDM(libabbauw::domMod::orderProposal::IOrderProposalCollectionFinderPtr proposalFinder)
		{
			m_OrderProposalCollectionFinderPtr = proposalFinder;
		}

		void ShowBVOStatusUC::injectDM(libabbauw::domMod::orderProposal::IOrderProposalPtr orderProposal)
		{
			m_OrderProposalPtr = orderProposal;
		}

		void ShowBVOStatusUC::injectDM(libabbauw::domMod::orderProtocol::IOrderProtocolPtr orderProtocol)
		{
			m_OrderProtocol = orderProtocol;
		}

		void ShowBVOStatusUC::init()
		{
			m_BVOStatusVC->init(m_OrderProposalCollectionFinderPtr->get());

			m_OrderProposalCollectionFinderPtr->resetSearchYit();
			basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_OrderProposalCollectionFinderPtr->getSearchYit();
			m_BVOStatusVC->matchToSearchCriteria(yitSearch);
		}

		void ShowBVOStatusUC::init(basar::Int32 orderNo, bool isOrderViewActive)
		{
			m_BVOStatusVC->init(m_OrderProposalCollectionFinderPtr->get(), orderNo);

			m_OrderProposalCollectionFinderPtr->resetSearchYit();
			basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_OrderProposalCollectionFinderPtr->getSearchYit();
			m_BVOStatusVC->matchToSearchCriteria(yitSearch);
			yitSearch.setInt32(libabbauw::properties::ORDERNO, orderNo);

			m_IsOrderViewActive = isOrderViewActive;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(ShowBVOStatusUC, FindOrderProposalsRequested)
		{
			METHODNAME_DEF(ShowBVOStatusUC, FindOrderProposalsRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);
			try
			{
				// no reset of search yit to keep order number set
				basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_OrderProposalCollectionFinderPtr->getSearchYit();
				m_BVOStatusVC->matchToSearchCriteria(yitSearch);
				m_OrderProposalCollectionFinderPtr->findByKey(yitSearch);
				m_BVOStatusVC->matchFromProposal();

				result = basar::appl::HANDLER_OK;
			}
			catch (basar::Exception& e) {
				//m_BVOStatusVC->error( e.what() );
				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(ShowBVOStatusUC, CancelOrderProposal)
		{
			METHODNAME_DEF(ShowBVOStatusUC, CancelOrderProposal)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);

			try
			{
				// no reset of search yit to keep order number set
				basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_OrderProposalPtr->getSearchYit();
				basar::db::aspect::AccessorPropertyTable_YIterator yit = m_BVOStatusVC->getSelectedOrderProposal();
				yitSearch.match(yit);
				yitSearch.setInt16(libabbauw::properties::BRANCHNO, m_Session->getUser()->getAreaID());

				m_OrderProtocol->setBackupPos(yit, m_Session->getUser()->getAreaID());

				m_OrderProposalPtr->findByKey(yitSearch);
				m_OrderProposalPtr->erase();
				m_OrdersChanged = true;

				m_OrderProtocol->protocolCancelOrderRequest(m_IsOrderViewActive);

				m_BVOStatusVC->info(QCoreApplication::translate("ShowBVOStatusUC", "Order proposal has been cancelled").toLocal8Bit().constData());

				refresh();

				result = basar::appl::HANDLER_OK;
			}
			catch (libabbauw::exceptions::OrderProposalException& e)
			{
				if (e.errorCode() == libabbauw::exceptions::OrderProposalException::ORDER_IS_TODAY)
				{
					m_BVOStatusVC->error(QCoreApplication::translate("ShowBVOStatusUC",
						"This order proposal will be ordered"
						" today and cannot be cancelled."
						" Please contact OZEK.").toLocal8Bit().constData());
				}
				else if (e.errorCode() == libabbauw::exceptions::OrderProposalException::ORDER_IS_DONE)
				{
					m_BVOStatusVC->error(QCoreApplication::translate("ShowBVOStatusUC",
						"Articles have been ordered already."
						" This order proposal cannot be cancelled."
						" If necessary contact OZEK.").toLocal8Bit().constData());
				}
				else // libabbauw::exceptions::OrderProposalException::GENERAL
				{
					m_BVOStatusVC->error(QCoreApplication::translate("ShowBVOStatusUC",
						"This order proposal cannot be cancelled.").toLocal8Bit().constData());
				}

				result.message = e.what();
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(ShowBVOStatusUC, ExportCSVRequested)
		{
			METHODNAME_DEF(ShowBVOStatusUC, ExportCSVRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			basar::appl::EventReturnStruct result(basar::appl::HANDLER_ERROR);

			services::ICSVDataExportServicePtr bvoStatusExportService = m_CSVExportGetter->getBVOStatusExportService(m_BVOStatusVC->getWidget());

			bvoStatusExportService->exportData(m_OrderProposalCollectionFinderPtr->get());

			return result;
		}

		void ShowBVOStatusUC::refresh()
		{
			QApplication::setOverrideCursor(Qt::WaitCursor);
			QApplication::processEvents();

			m_OrderProposalCollectionFinderPtr->getSearchYit().setInt16(libabbauw::properties::BRANCHNO, m_Session->getUser()->getAreaID());
			m_OrderProposalCollectionFinderPtr->findByKey(m_OrderProposalCollectionFinderPtr->getSearchYit());

			m_BVOStatusVC->matchFromProposal();

			QApplication::restoreOverrideCursor();
		}

		void ShowBVOStatusUC::run()
		{
			METHODNAME_DEF(ShowBVOStatusUC, run)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);

			registerEventHandlers(true);
			try
			{
				m_BVOStatusVC->show();

				refresh();

				m_BVOStatusVC->waitForEvents();

				if (m_OrdersChanged)
				{
					if (m_OrderProposalCollectionFinderPtr->getSearchYit().isContainedAndSet(libabbauw::properties::ORDERNO))
					{
						// runs in context of order - all order positions have to be refreshed
						basar::appl::SystemEventManager::getInstance().fire("TransferOrderPositionsChanged");
					}
					else
					{
						// refresh all orders
						basar::appl::SystemEventManager::getInstance().fire("TransferOrderChanged");
					}
				}
			}
			catch (basar::Exception& e)
			{
				BLOG_ERROR(libabbauw::LoggerPool::loggerUseCases, e.what());
				throw;
			}
			registerEventHandlers(false);
		}

	} // end namespace useCase
} // end namespace CSC_Batches
