//--------------------------------------------------------------------------------------------------//
/*! \file       
 *  \brief      
 *  \author     Bischof Bjoern
 *  \date       27.12.2010
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "useradminlogin.h"
#include "classinfo_macros.h"
#include "loggerpool.h"
#include "useradminfailureexception.h"

#include <libbasarlogin_manager.h>
#include <libbasarlogin_exceptions.h> 

//--------------------------------------------------------------------------------------------------//
// namespace libBatch
//--------------------------------------------------------------------------------------------------//
namespace libBatch
{
namespace userAdmin
{

UserAdminLogin::UserAdminLogin( const UserLoginConfiguration& userLoginConfiguration )
: m_Logger( LoggerPool::LoggerUserAdmin ), m_UserLoginConfiguration( userLoginConfiguration ), m_LoggedIn( false )
{
	METHODNAME_DEF( UserAdminLogin, UserAdminLogin )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

UserAdminLogin::~UserAdminLogin()
{
	METHODNAME_DEF( UserAdminLogin, ~UserAdminLogin )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

void UserAdminLogin::login()
{
    METHODNAME_DEF( UserAdminLogin, login )
	BLOG_TRACE_METHOD( m_Logger, fun );

    bool loggedIn = false;
    try {
        basar::I18nString appName = m_UserLoginConfiguration.m_AppName;
        using basar::login::Manager;
        loggedIn = Manager::getInstance().nonInteractiveLogin( appName,                       
                                                      m_UserLoginConfiguration.m_User, 
                                                      m_UserLoginConfiguration.m_Passwd, 
                                                      m_UserLoginConfiguration.m_AreaID );
    } catch( basar::login::InvalidApplNameException& e ){
        throw libBatch::userAdmin::UserAdminFailureException( basar::ExceptInfo(fun, e.what(), __FILE__, __LINE__) );      		
    }

	if( false == loggedIn ){
        std::stringstream ss;
        ss  << "ERROR: No valid user log in:" << std::endl
            << m_UserLoginConfiguration;
        throw libBatch::userAdmin::UserAdminFailureException( basar::ExceptInfo(fun, ss.str().c_str(), __FILE__, __LINE__) );      		
	}

    logLogin();
    
    m_LoggedIn = true;
}

void UserAdminLogin::logLogin() const
{
    METHODNAME_DEF( UserAdminLogin, logLogin )
	BLOG_TRACE_METHOD( m_Logger, fun );

    if( m_Logger.isEnabledFor( log4cplus::INFO_LOG_LEVEL ) ){
        std::stringstream ss;
        ss  << "Successfully logged in user admin:" << std::endl << m_UserLoginConfiguration;
        m_Logger.forcedLog( log4cplus::INFO_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
    }
}

bool UserAdminLogin::isloggedIn() const
{
    METHODNAME_DEF( UserAdminLogin, isloggedIn )
	BLOG_TRACE_METHOD( m_Logger, fun );

    return m_LoggedIn;
}

const UserLoginConfiguration& UserAdminLogin::getUserLoginConfiguration() const
{
    METHODNAME_DEF( UserAdminLogin, getUserLoginConfiguration )
	BLOG_TRACE_METHOD( m_Logger, fun );

    return m_UserLoginConfiguration;
}

} // end namespace userAdmin
} // end namespace libBatch

