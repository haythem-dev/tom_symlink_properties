#include "mainuc.h"

#include "mainwindowvc.h"
#include "libabbauw/iparameterdm.h"
#include "iuserrights.h"

#include "iusecasegetter.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include <libutil/viewconn.h>
#include <libutil/util.h>

namespace CSC_Batches
{
namespace useCase
{
    BEGIN_HANDLERS_REGISTRATION( MainUC )
        SYSTEM_EVENT_HANDLER_REGISTRATION( MainUC, startAboutDlg                       )
        SYSTEM_EVENT_HANDLER_REGISTRATION( MainUC, startBatchOrderMaintenanceRequested )
        SYSTEM_EVENT_HANDLER_REGISTRATION( MainUC, createTransferOrderRequested        )
		SYSTEM_EVENT_HANDLER_REGISTRATION( MainUC, editBatchOrderRequested             )
		SYSTEM_EVENT_HANDLER_REGISTRATION( MainUC, startSettingsRequested              )
    END_HANDLERS_REGISTRATION()

    MainUC::MainUC()
    : m_Logger(  libabbauw::LoggerPool::loggerUseCases )
    {
        METHODNAME_DEF( MainUC, MainUC )
        BLOG_TRACE_METHOD( m_Logger, fun );
    }

    MainUC::~MainUC()
    {
        METHODNAME_DEF( MainUC, ~MainUC )
        BLOG_TRACE_METHOD( m_Logger, fun );
    }

    void MainUC::injectMainVC( viewConn::MainWindowVCPtr mainVC )
    {
        METHODNAME_DEF( MainUC, injectMainVC )
        BLOG_TRACE_METHOD( m_Logger, fun );

        m_MainVC = mainVC;
    }

    void MainUC::injectApplicationConfigVC( basar::gui::qt::ApplicationConfigVCPtr applicationConfigVC )
    {
        METHODNAME_DEF( MainUC, injectApplicationConfigVC )
        BLOG_TRACE_METHOD( m_Logger, fun );
            
        m_ApplicationConfigVC = applicationConfigVC;
    }

    viewConn::MainWindowVCPtr MainUC::getMainVC() const
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

    libutil::infrastructure::IQApplicationPtr MainUC::getQApplication() const
    {
        METHODNAME_DEF( MainUC, getQApplication )
        BLOG_TRACE_METHOD( m_Logger, fun );

        CHECK_INSTANCE_EXCEPTION( m_QApplication.get() );
        return m_QApplication;
    }

    void MainUC::injectUseCaseGetter( componentManager::IUseCaseGetterPtr useCaseGetter )
    {
        METHODNAME_DEF( MainUC, injectUseCaseGetter )
        BLOG_TRACE_METHOD( m_Logger, fun );

        m_UseCaseGetter = useCaseGetter;
    }

    componentManager::IUseCaseGetterPtr MainUC::getUseCaseGetter() const
    {
        METHODNAME_DEF( MainUC, getUseCaseGetter )
        BLOG_TRACE_METHOD( m_Logger, fun );

        CHECK_INSTANCE_EXCEPTION( m_UseCaseGetter.get() );
        return m_UseCaseGetter;
    }

    void MainUC::injectParameter( libabbauw::domMod::parameter::IParameterDMPtr parameter )
    {
        m_Parameter = parameter;
    }

    libabbauw::domMod::parameter::IParameterDMPtr MainUC::getParameter() const
    {
        CHECK_INSTANCE_EXCEPTION( m_Parameter.get() );
        return m_Parameter;
    }

    void MainUC::injectUserRights( infrastructure::rights::IUserRightsPtr userRights )
    {
        m_UserRights = userRights;
    }

    infrastructure::rights::IUserRightsPtr MainUC::getUserRights() const
    {
        CHECK_INSTANCE_EXCEPTION( m_UserRights.get() );
        return m_UserRights;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( MainUC, startAboutDlg )
    {
        METHODNAME_DEF( MainUC, startAboutDlg )
        BLOG_TRACE_METHOD( m_Logger, fun );

        rSource;
        yitFromEvSrc;

        basar::appl::EventReturnType result( basar::appl::HANDLER_OK );
        try
        {
            m_ApplicationConfigVC->show( getMainVC().get() );
        }
        catch( basar::Exception& e )
        {
            BLOG_ERROR( m_Logger, e.what() );
            result = basar::appl::HANDLER_ERROR;
        }

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( MainUC, startBatchOrderMaintenanceRequested )
    {
        METHODNAME_DEF( MainUC, startBatchOrderMaintenanceRequested )
        BLOG_TRACE_METHOD( m_Logger, basar::VarString( "SystemEvent " ) + fun );
		rSource;
        yitFromEvSrc;

        basar::appl::EventReturnType result( basar::appl::HANDLER_OK );
        try
		{
            getUserRights()->checkUserAllowedToAccessBatchOrderMaintenance();
            getUseCaseGetter()->getBatchOrderMaintenanceUC()->run();
        }
        catch( libutil::exceptions::BaseException& e )
		{
            getMainVC()->showMsg( e.reason() );
			result.ret = basar::appl::HANDLER_ERROR;
		}

        return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( MainUC, createTransferOrderRequested )
    {
        METHODNAME_DEF( MainUC, createTransferOrderRequested )
        BLOG_TRACE_METHOD( m_Logger, basar::VarString( "SystemEvent " ) + fun );
		rSource;
        yitFromEvSrc;

        basar::appl::EventReturnType result( basar::appl::HANDLER_OK );

        basar::I18nString msg;
        try
		{
            getUserRights()->checkUserAllowedToCreateTransferOrders();
            getUseCaseGetter()->getTransferOrderCreatorUC()->run();
		}
        catch( libutil::exceptions::BaseException& e )
		{
            getMainVC()->showMsg( e.reason() );
			result.ret = basar::appl::HANDLER_ERROR;
		}
		
		return result;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( MainUC, editBatchOrderRequested )
    {
		METHODNAME_DEF( MainUC, editBatchOrderRequested )
        BLOG_TRACE_METHOD( m_Logger, basar::VarString( "SystemEvent " ) + fun );
		rSource;
        yitFromEvSrc;

		basar::appl::EventReturnType result( basar::appl::HANDLER_OK );

        try
		{
            getUserRights()->checkUserAllowedToAccessTransferOrders();
            getUseCaseGetter()->getBatchOrderEditionUC()->run();
		}
        catch( libutil::exceptions::BaseException& e )
		{
            getMainVC()->showMsg( e.reason() );
			result.ret = basar::appl::HANDLER_ERROR;
		}
		
		return result;
    }

	SYSTEM_EVENT_HANDLER_DEFINITION( MainUC, startSettingsRequested )
	{
		METHODNAME_DEF( MainUC, startSettingsRequested )
        BLOG_TRACE_METHOD( m_Logger, basar::VarString( "SystemEvent " ) + fun );
		rSource;
        yitFromEvSrc;

		basar::appl::EventReturnType result( basar::appl::HANDLER_OK );
        
        try
		{
            getUserRights()->checkUserAllowedToAccessSettings();
            getUseCaseGetter()->getUserSettingsUC()->run();
		}
        catch( libutil::exceptions::BaseException& e )
		{
            getMainVC()->showMsg( e.reason() );
			result.ret = basar::appl::HANDLER_ERROR;
		}

		return result;
	}

    void MainUC::run()
    {
        METHODNAME_DEF( MainUC, run )
        BLOG_TRACE_METHOD( m_Logger, fun );

        try
        {
            getUserRights()->checkUserAllowedToLaunchAppl();
        }
        catch( libutil::exceptions::UserAdminFailureException& e )
        {
            BLOG_INFO( m_Logger, e.what() );
            getMainVC()->showMsg( e.reason() );
            return;
        }

        registerEventHandlers( true );
        getMainVC()->show();
        
        // first check all parameters and reset wrong ones (with user notification)
        getParameter()->load();
        if( getParameter()->setInvalidParametersToDefault() )
        {
	        getMainVC()->showSettingSetToDefaultNotification();
	        getParameter()->save();
        }

        getQApplication()->exec();

        registerEventHandlers( false );
    }
} // end namespace usecase
} // end namespace CSC_Batches
