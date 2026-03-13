//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "mainuc.h"
#include "mainvc.h"
#include "tenderapplcompmgr.h"
#include "loggerpool/tenderloggerpool.h"

#include <domainmodule/tender/itendercollectiondm.h>

#include <libutil/viewconn.h>
#include <libutil/util.h>
#include <libbasarcmnutil_remote.h> // TODO remove later


//-------------------------------------------------------------------------------------------------//
// namespace sections
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace useCase
    {
        //-------------------------------------------------------------------------------------------------//
        // macro for registrating and deregistrating EventHandlers!
        //-------------------------------------------------------------------------------------------------//
        BEGIN_HANDLERS_REGISTRATION( MainUC )
            SYSTEM_EVENT_HANDLER_REGISTRATION( MainUC, StartAboutDlg         )
            SYSTEM_EVENT_HANDLER_REGISTRATION( MainUC, StartCreateNewTender  )
            SYSTEM_EVENT_HANDLER_REGISTRATION( MainUC, StartSearchTender     )
            SYSTEM_EVENT_HANDLER_REGISTRATION( MainUC, StartImportTender     )
			SYSTEM_EVENT_HANDLER_REGISTRATION( MainUC, StartImportDeliveries )
            SYSTEM_EVENT_HANDLER_REGISTRATION( MainUC, StartCreateTO         )	 // TODO remove later
        END_HANDLERS_REGISTRATION()

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // ArticleSearchUC::ArticleSearchUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MainUC::MainUC()
        : m_Logger( libtender::LoggerPool::getLoggerUseCases() )
        {
            METHODNAME_DEF( MainUC, MainUC )
            BLOG_TRACE_METHOD( m_Logger, fun );
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // ArticleSearchUC::~ArticleSearchUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        MainUC::~MainUC()
        {
            METHODNAME_DEF( MainUC, ~MainUC )
            BLOG_TRACE_METHOD( m_Logger, fun );
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void MainUC::injectMainVC( tender::viewConn::MainVCPtr mainVC )
        {
            METHODNAME_DEF( MainUC, injectMainVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_MainVC = mainVC;
        }

        tender::viewConn::MainVCPtr MainUC::getMainVC()
        {
            METHODNAME_DEF( MainUC, getMainVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_MainVC.get() );
            return m_MainVC;
        }

        void MainUC::injectQApplication( libutil::infrastructure::IQApplicationPtr qApplication )
        {
            METHODNAME_DEF( MainUC, injectQApplication )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_QApplication = qApplication;
        }

        libutil::infrastructure::IQApplicationPtr MainUC::getQApplication()
        {
            METHODNAME_DEF( MainUC, getQApplication )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_QApplication.get() );
            return m_QApplication;
        }

        void MainUC::injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter )
        {
            METHODNAME_DEF( MainUC, injectUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_UseCaseGetter = useCaseGetter;
        }

		void MainUC::injectTenderCollectionDM( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM )
        {
            METHODNAME_DEF( MainUC, injectTenderCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_TenderCollection = tenderCollectionDM;
        }

        tender::componentManager::IUseCaseGetterPtr MainUC::getUseCaseGetter()
        {
            METHODNAME_DEF( MainUC, getUseCaseGetter )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_UseCaseGetter.get() );
            return m_UseCaseGetter;
        }

		libtender::domMod::tender::ITenderCollectionDMPtr MainUC::getTenderCollectionDM()
        {
            METHODNAME_DEF( MainUC, getTenderCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

            CHECK_INSTANCE_EXCEPTION( m_TenderCollection.get() );
            return m_TenderCollection;
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void MainUC::run()
        {
            METHODNAME_DEF( MainUC, run )
            BLOG_TRACE_METHOD( m_Logger, fun );

            registerEventHandlers( true );

            getMainVC()->show();
            
            libutil::useCase::IRunnablePtr overviewUC = getUseCaseGetter()->getTenderOverviewUC();
            overviewUC->run();

            getQApplication()->exec();

            registerEventHandlers( false );
        }

        SYSTEM_EVENT_HANDLER_DEFINITION( MainUC, StartAboutDlg )
        {
            METHODNAME_DEF( MainUC, StartAboutDlg )
            BLOG_TRACE_METHOD( m_Logger, fun );

            rSource;

            basar::appl::EventReturnType result(basar::appl::HANDLER_OK);

            basar::gui::qt::About aboutDlg;
            aboutDlg.show( getMainVC().get() );

            return result;
        }

        SYSTEM_EVENT_HANDLER_DEFINITION( MainUC, StartCreateNewTender )
        {
            METHODNAME_DEF( MainUC, StartCreateNewTender )
            BLOG_TRACE_METHOD( m_Logger, fun );

            rSource;
            basar::appl::EventReturnType result(basar::appl::HANDLER_OK);

			try
			{
				basar::db::aspect::AccessorPropertyTable_YIterator yitTender;
        		libtender::domMod::tender::ITenderPtr tender = getTenderCollectionDM()->getTender( yitTender );

				libutil::useCase::IRunnablePtr tenderMaintainUC = getUseCaseGetter()->getTenderMaintainUC( tender );
				tenderMaintainUC->run();
			}
			catch( const basar::Exception& e )
			{
				BLOG_DEBUG( m_Logger, e.what() );
				throw;
			}

            return result;
        }

        SYSTEM_EVENT_HANDLER_DEFINITION( MainUC, StartSearchTender )
        {
            METHODNAME_DEF( MainUC, StartSearchTender )
            BLOG_TRACE_METHOD( m_Logger, fun );

            rSource;
            basar::appl::EventReturnType result(basar::appl::HANDLER_OK);

            try
            {
                libutil::useCase::IRunnablePtr overviewUC = getUseCaseGetter()->getTenderOverviewUC();
                overviewUC->run();
            }
            catch( const basar::Exception& e )
            {
                BLOG_DEBUG( m_Logger, e.what() );
				throw;
            }

            return result;
        }

        SYSTEM_EVENT_HANDLER_DEFINITION( MainUC, StartImportTender )
        {
            METHODNAME_DEF( MainUC, StartImportTender )
            BLOG_TRACE_METHOD( m_Logger, fun );

            rSource;
			basar::appl::EventReturnType result(basar::appl::HANDLER_OK);

            try
            {
                libutil::useCase::IRunnablePtr importUC = getUseCaseGetter()->getTenderImportUC();
                importUC->run();
            }
            catch( const basar::Exception& e )
            {
                BLOG_DEBUG( m_Logger, e.what() );
				throw;
            }

            return result;
        }

		SYSTEM_EVENT_HANDLER_DEFINITION( MainUC, StartImportDeliveries )
        {
            METHODNAME_DEF( MainUC, StartImportTender )
            BLOG_TRACE_METHOD( m_Logger, fun );

            rSource;

			basar::appl::EventReturnType result(basar::appl::HANDLER_OK);

            return result;
        }

		SYSTEM_EVENT_HANDLER_DEFINITION( MainUC, StartCreateTO ) // TODO remove later
        {
            METHODNAME_DEF( MainUC, StartCreateTO )
            BLOG_TRACE_METHOD( m_Logger, fun );

            rSource;

            basar::appl::EventReturnType result(basar::appl::HANDLER_OK);

			basar::cmnutil::RemoteTaskRef task;
			basar::cmnutil::RemoteEndpoint endpoint("zdev21");
			task.setEndpoint(endpoint);

			basar::cmnutil::CredentialBasePtr cred(new basar::cmnutil::UserPassCredential()); 
			cred->setIdentity("devuser");
			cred->setPassword("phosix+");
			
			task.setCredentials(cred);

			task.setTaskCommand("/software/wss/pfr/tender/proc/tenderordercreator.sh -start -country pfr");
			basar::cmnutil::RemoteTaskResultRef res = basar::cmnutil::RemoteTaskSubmitter::getInstance().submitRemoteTask(task);
			if (res.getExitCode() != 0)
			{
				result.ret = basar::appl::HANDLER_ERROR;
			}
			result.message = res.getResult();

            return result;
        }
    } // namespace usecase
} // namespace tender
