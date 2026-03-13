#include "batchordermaintenanceuc.h"

#include "viewconn/ibatchordermaintenancevc.h"
#include "iorderreleaseinformationvc.h"
#include "iusecasegetter.h"

#include <libabbauw/domainmodule/batchorder/ibatchordercollectionfinder.h>
#include <libabbauw/domainmodule/batchorder/ibatchorder.h>

#include <libabbauw/loggerpool/libabbauw_loggerpool.h>
#include <libabbauw/libabbauw_properties_definitions.h>
#include <libabbauw/exceptions/orderreleaseexception.h>

#include <libutil/usecase.h>
#include <alreadylockedexception.h>

namespace CSC_Batches
{
	namespace useCase
	{
		BEGIN_HANDLERS_REGISTRATION(BatchOrderMaintenanceUC)
			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderMaintenanceUC, SearchBatchOrdersRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderMaintenanceUC, ShowPositionsRequested)

			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderMaintenanceUC, ReleaseSelectedOrdersRequested)
			SYSTEM_EVENT_HANDLER_REGISTRATION(BatchOrderMaintenanceUC, ReleaseFilteredOrdersRequested)
			END_HANDLERS_REGISTRATION()

			BatchOrderMaintenanceUC::BatchOrderMaintenanceUC()
		{
		}

		BatchOrderMaintenanceUC::~BatchOrderMaintenanceUC()
		{
		}

		void BatchOrderMaintenanceUC::injectBatchOrderCollectionFinder(libabbauw::domMod::batchOrder::IBatchOrderCollectionFinderPtr dm)
		{
			m_BatchOrderCollectionFinder = dm;
		}

		libabbauw::domMod::batchOrder::IBatchOrderCollectionFinderPtr BatchOrderMaintenanceUC::getBatchOrderCollectionFinder()
		{
			CHECK_INSTANCE_EXCEPTION(m_BatchOrderCollectionFinder.get());
			return m_BatchOrderCollectionFinder;
		}

		void BatchOrderMaintenanceUC::injectBatchOrderMaintenanceVC(viewConn::IBatchOrderMaintenanceVCPtr vc)
		{
			m_BatchOrderMaintenanceVC = vc;
		}

		viewConn::IBatchOrderMaintenanceVCPtr BatchOrderMaintenanceUC::getBatchOrderMaintenanceVC()
		{
			CHECK_INSTANCE_EXCEPTION(m_BatchOrderMaintenanceVC.get());
			return m_BatchOrderMaintenanceVC;
		}

		void BatchOrderMaintenanceUC::injectOrderReleaseInformationVC(viewConn::IOrderReleaseInformationVCPtr vc)
		{
			m_OrderReleaseInformationVC = vc;
		}

		viewConn::IOrderReleaseInformationVCPtr BatchOrderMaintenanceUC::getOrderReleaseInformationVC()
		{
			CHECK_INSTANCE_EXCEPTION(m_OrderReleaseInformationVC.get());
			return m_OrderReleaseInformationVC;
		}

		void BatchOrderMaintenanceUC::injectUseCaseGetter(componentManager::IUseCaseGetterPtr ucGetter)
		{
			m_UseCaseGetter = ucGetter;
		}

		componentManager::IUseCaseGetterPtr BatchOrderMaintenanceUC::getUseCaseGetter()
		{
			CHECK_INSTANCE_EXCEPTION(m_UseCaseGetter.get());
			return m_UseCaseGetter;
		}

		void BatchOrderMaintenanceUC::setSession(libutil::infrastructure::session::ISessionPtr session)
		{
			m_Session = session;
		}

		libutil::infrastructure::session::ISessionPtr BatchOrderMaintenanceUC::getSession()
		{
			CHECK_INSTANCE_EXCEPTION(m_Session.get());
			return m_Session;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderMaintenanceUC, SearchBatchOrdersRequested)
		{
			METHODNAME_DEF(BatchOrderMaintenanceUC, SearchBatchOrdersRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			basar::appl::EventReturnType result(basar::appl::HANDLER_OK);
			try
			{
				searchBatchOrders();
			}
			catch (libutil::exceptions::BaseException& e)
			{
				getBatchOrderMaintenanceVC()->showErrorMessage(e);
				result = basar::appl::HANDLER_ERROR;
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderMaintenanceUC, ShowPositionsRequested)
		{
			METHODNAME_DEF(BatchOrderMaintenanceUC, ShowPositionsRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			basar::appl::EventReturnType result(basar::appl::HANDLER_OK);
			try
			{
				getUseCaseGetter()->getBatchOrderPosUC(getBatchOrderMaintenanceVC()->getSelectedOrder())->run();
			}
			catch (libLockManager::exceptions::AlreadyLockedException& e)
			{
				BLOG_INFO(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_BatchOrderMaintenanceVC->showInfoMessage(e.reason());
			}

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderMaintenanceUC, ReleaseSelectedOrdersRequested)
		{
			METHODNAME_DEF(BatchOrderMaintenanceUC, ReleaseSelectedOrdersRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			basar::appl::EventReturnType result(basar::appl::HANDLER_OK);

			viewConn::IBatchOrderMaintenanceVC::YIterators yits = getBatchOrderMaintenanceVC()->getSelectedOrders();
			m_OrderReleaseInformationVC->setNumberOfOrders(static_cast<basar::Int32>(yits.size()));
			m_OrderReleaseInformationVC->show();

			try
			{
				for (viewConn::IBatchOrderMaintenanceVC::YIterators::const_iterator it = yits.begin(); it != yits.end(); ++it)
				{
					releaseOrder(*it);
				}
			}
			catch (basar::Exception& e)
			{
				BLOG_ERROR(libabbauw::LoggerPool::loggerUseCases, e.what());
				getBatchOrderMaintenanceVC()->showErrorMessage(e.what());
			}

			m_OrderReleaseInformationVC->waitForEvents();
			searchBatchOrders(); // always refresh list with changed status of items

			return result;
		}

		SYSTEM_EVENT_HANDLER_DEFINITION(BatchOrderMaintenanceUC, ReleaseFilteredOrdersRequested)
		{
			METHODNAME_DEF(BatchOrderMaintenanceUC, ReleaseFilteredOrdersRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, basar::VarString("SystemEvent ") + fun);
			rSource;
			yitFromEvSrc;

			basar::appl::EventReturnType result(basar::appl::HANDLER_OK);

			const basar::Int32 totalNumberOfOrders = static_cast<basar::Int32>(getBatchOrderCollectionFinder()->get().size());
			const basar::Int32 numberOfOrdersToBeReleased = getBatchOrderMaintenanceVC()->showNumberOfOrdersToBeReleasedInputDialog();
			if (numberOfOrdersToBeReleased < 0)
			{
				return result;
			}

			m_OrderReleaseInformationVC->setNumberOfOrders(numberOfOrdersToBeReleased < totalNumberOfOrders ? numberOfOrdersToBeReleased
				: totalNumberOfOrders);
			m_OrderReleaseInformationVC->show();

			try
			{
				basar::Int32 orderCount = 0;
				for (basar::db::aspect::AccessorPropertyTable_YIterator yit = getBatchOrderCollectionFinder()->get().begin(); !yit.isEnd() && orderCount < numberOfOrdersToBeReleased;
					++yit, ++orderCount)
				{
					releaseOrder(yit);
				}
			}
			catch (basar::Exception& e)
			{
				BLOG_ERROR(libabbauw::LoggerPool::loggerUseCases, e.what());
				getBatchOrderMaintenanceVC()->showErrorMessage(e.what());
			}

			m_OrderReleaseInformationVC->waitForEvents();
			searchBatchOrders(); // always refresh list with changed status of items

			return result;
		}

		void BatchOrderMaintenanceUC::releaseOrder(basar::db::aspect::AccessorPropertyTable_YIterator yit)
		{
			METHODNAME_DEF(BatchOrderMaintenanceUC, ReleaseFilteredOrdersRequested)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);

			const basar::Int32 orderNo = yit.getInt32(libabbauw::properties::ORDERNO);
			try
			{
				getBatchOrderCollectionFinder()->getOrder(yit)->release();
			}
			catch (libabbauw::exceptions::OrderReleaseException& e)
			{
				BLOG_WARN(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_OrderReleaseInformationVC->printError(orderNo, e.reason());
			}
			catch (libLockManager::exceptions::AlreadyLockedException& e)
			{
				BLOG_INFO(libabbauw::LoggerPool::loggerUseCases, e.what());
				m_OrderReleaseInformationVC->printError(orderNo, e.reason());
			}

			m_OrderReleaseInformationVC->update();
		}

		void BatchOrderMaintenanceUC::searchBatchOrders()
		{
			METHODNAME_DEF(BatchOrderMaintenanceUC, searchBatchOrders)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);

			getBatchOrderCollectionFinder()->resetSearchYit();
			getBatchOrderMaintenanceVC()->matchToSearchCriteria(getBatchOrderCollectionFinder()->getSearchYit());
			getBatchOrderCollectionFinder()->getSearchYit().setInt16(libabbauw::properties::BRANCHNO, getSession()->getUser()->getAreaID());
			getBatchOrderCollectionFinder()->findByPattern(getBatchOrderCollectionFinder()->getSearchYit());

			getBatchOrderMaintenanceVC()->matchFromBatchOrders(getBatchOrderCollectionFinder()->get());
		}

		void BatchOrderMaintenanceUC::run()
		{
			METHODNAME_DEF(BatchOrderMaintenanceUC, run)
				BLOG_TRACE_METHOD(libabbauw::LoggerPool::loggerUseCases, fun);

			registerEventHandlers(true);

			getBatchOrderMaintenanceVC()->show();
			searchBatchOrders();
			getBatchOrderMaintenanceVC()->waitForEvents();

			registerEventHandlers(false);
		}

	} // namespace useCase
} // namespace CSC_Batches
