#include "userrights.h"

#include "definitions_uadm.h"

#include <libutil/logger/libutil_loggerpool.h>
#include <libbasarcmnutil_logging.h>
#include <libutil/misc/metaclassmacro_definitions.h>
#include <libutil/misc/instancechecker_macro_definition.h>

#include <libbasarcmnutil_i18nstring.h>
#include <libbasarlogin_manager.h>

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4512 4800)
#include <QtCore/QCoreApplication>
#pragma warning (pop)

namespace CSC_Batches
{
namespace infrastructure
{
namespace rights
{
    UserRights::UserRights()
        : m_Logger( libutil::LoggerPool::getLoggerLibUtilUserAdmin() )
    {
    }

    UserRights::~UserRights()
    {
    }

    void UserRights::injectParameterGateway( libutil::infrastructure::parameter::IParameterGatewayPtr parameterGW )
    {
        m_ParameterGateway = parameterGW;
    }

    libutil::infrastructure::parameter::IParameterGatewayPtr UserRights::getParameterGateway() const
    {
        CHECK_INSTANCE_EXCEPTION( m_ParameterGateway.get() );
        return m_ParameterGateway;
    }

    void UserRights::injectSession( libutil::infrastructure::session::ISessionPtr session )
    {
        m_Session = session;
    }

    libutil::infrastructure::session::ISessionPtr UserRights::getSession() const
    {
        CHECK_INSTANCE_EXCEPTION( m_Session.get() );
        return m_Session;
    }

    bool UserRights::isLegitimated( const basar::I18nString& right )
    {
        return basar::login::Manager::getInstance().isLegitimated( UADM_MOD_UNIVERSAL, right );
    }

    void UserRights::checkLegitimated( const basar::I18nString& right )
    {
        METHODNAME_DEF( UserRights, checkLegitimated )

        basar::I18nString msg;
        if( ! isUserGranted( right, msg ) )
        {
            throw libutil::exceptions::UserAdminFailureException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ) );
        }
    }

    bool UserRights::isUserGranted( const basar::I18nString& right, basar::I18nString& msg ) 
	{
		METHODNAME_DEF( UserRights, isUserGranted )
		BLOG_TRACE_METHOD( m_Logger, fun );

		bool isGranted = true;
		if( ! isLegitimated( right ) ) 
        {
            msg = QCoreApplication::translate("UserRights", 
                                              "The userright '%1' for module '%2' must be granted to your account in UADM,"
                                              "so that you are allowed to work with this part of the program!").arg( right.c_str() ).arg( UADM_MOD_UNIVERSAL ).toLocal8Bit().constData();
			BLOG_DEBUG( m_Logger, msg.c_str() );
            isGranted = false;
		}

		return isGranted;
	}

    void UserRights::checkUserAllowedToLaunchAppl()
    {
        METHODNAME_DEF( UserRights, checkUserAllowedToLaunchAppl )
		BLOG_TRACE_METHOD( m_Logger, fun );
		
        bool ready = false;

		//--- ask UsrAdmin for parameter UADM_START_APPL:
		// get user-specific START parameter:
		basar::I18nString ParamValue;
        if( getParameterGateway()->getParameterValue( UADM_APPL, getSession()->getUser()->getUserName(), UADM_START_APPL, ParamValue) )
        {
			ready = ( "TRUE" == ParamValue );
		}
		else // if no user-specific START parameter found (for individual testing!), then take application-specific one:
        {
			if( getParameterGateway()->getParameterValue( UADM_APPL, UADM_START_APPL, ParamValue ) )
            {
				ready = ( "TRUE" == ParamValue );
			}
		}

        basar::I18nString msg;
        if( ! ready )
        {
            msg = QCoreApplication::translate( "UserRights",
                                               "Application is not allowed to start! No right granted in UsrAdmin.").toLocal8Bit().constData();
            throw libutil::exceptions::UserAdminFailureException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ) );
        }

        // at least one of these rights is needed to start the application
        if(! (isUserGranted( UADM_RIGHT_GENERAL, msg ) || isUserGranted( UADM_RIGHT_UW_ACCESS, msg ) ||
              isUserGranted( UADM_RIGHT_UW_EDIT_ACCESS, msg ) ) )
        {
            throw libutil::exceptions::UserAdminFailureException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ) );
        }
    }

    void UserRights::checkUserAllowedToAccessBatchOrderMaintenance()
    {
        checkLegitimated( UADM_RIGHT_GENERAL );
    }

    void UserRights::checkUserAllowedToAccessTransferOrders()
    {
        checkLegitimated( UADM_RIGHT_UW_ACCESS );
    }

    void UserRights::checkUserAllowedToCreateTransferOrders()
    {
        checkLegitimated( UADM_RIGHT_UW_EDIT_ACCESS );
    }

    bool UserRights::isUserAllowedToChangeTransferOrders()
    {
        return isLegitimated( UADM_RIGHT_UW_CHANGE_ACCESS );
    }

    bool UserRights::isUserAllowedToPlaceOrderWishes()
    {
        return isLegitimated( UADM_RIGHT_UW_PURCHASE_ACCESS );
    }

    void UserRights::checkUserAllowedToAccessSettings()
    {
        checkLegitimated( UADM_RIGHT_SETTINGS_ACCESS );
    }

    bool UserRights::isUserAllowedToChangePrivilegedSettings()
    {
        return isLegitimated( UADM_RIGHT_PRIV_SETTINGS_ACCESS );
    }

    bool UserRights::isUserAllowedToAccessDeveloperInfo()
    {
        return isLegitimated( UADM_RIGHT_DEVELOPER_INFO );
    }


} // end namespace rights
} // end namespace infrastructure
} // end namespace CSC_Batches
