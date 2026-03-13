#include "appinitializer.h"
#include "useradminlogin_definitions.h"
#include "abbauwcmdlineevaluator.h"
#include "abbauwappl_definitions.h"
#include "appinitializerexception.h"
#include "loggerpool.h"
#include "appargs.h"
#include "libcscbatch_definitions.h"

// LIB: libbatch
#include <classinfo_macros.h>
#include <cmdlineexception.h>
#include <useradminfailureexception.h>
#include "dbconnectionconfiguration.h"

#include <useradminlogin.h>
#include <useradmindbconfiguration.h>

// LIB: BASAR
#include <libbasar_definitions.h>
#include <libbasarcmnutil_logging.h>

namespace abbaUW {

AppInitializer::AppInitializer( const libBatch::cmdLine::CmdLineConfiguration& cmdLineConfiguration )
: m_Logger( LoggerPool::LoggerAbbaUW ), m_CmdLineConfiguration( cmdLineConfiguration )
{  
    METHODNAME_DEF( AppInitializer, AppInitializer );
	BLOG_TRACE_METHOD( m_Logger, fun );
}

AppInitializer::~AppInitializer()
{
    METHODNAME_DEF( AppInitializer, ~AppInitializer );
	BLOG_TRACE_METHOD( m_Logger, fun );
}

void AppInitializer::init()
{
    METHODNAME_DEF( AppInitializer, init );
    BLOG_TRACE_METHOD( m_Logger, fun );

    try 
	{
        handleCmdLine();
        handleUserAdmin();
    } 
	catch( libBatch::cmdLine::CmdLineException& e )
	{
        throw AppInitializerException( basar::ExceptInfo(fun, e.what(), __FILE__, __LINE__) );
    } 
	catch( libBatch::userAdmin::UserAdminFailureException& e )
	{
        throw AppInitializerException( basar::ExceptInfo(fun, e.what(), __FILE__, __LINE__) );
    }
}
void AppInitializer::handleCmdLine()
{
    METHODNAME_DEF( AppInitializer, handleCmdLine );
	BLOG_TRACE_METHOD( m_Logger, fun );

    m_AbbaUWCmdLineEvaluator = cmdLine::AbbaUWCmdLineEvaluatorPtr( new cmdLine::AbbaUWCmdLineEvaluator( m_CmdLineConfiguration) );
    m_AbbaUWCmdLineEvaluator->evaluate();
}

void AppInitializer::handleUserAdmin()
{
    METHODNAME_DEF( AppInitializer, handleUserAdmin );
	BLOG_TRACE_METHOD( m_Logger, fun );

    handleUserAdminLogIn();
    handleUserAdminDBParameter();
}

void AppInitializer::handleUserAdminLogIn()
{
    METHODNAME_DEF( AppInitializer, handleUserAdminLogIn );
	BLOG_TRACE_METHOD( m_Logger, fun );

    libBatch::userAdmin::UserLoginConfiguration userLoginConfiguration;
    userLoginConfiguration.m_User    = abbaUW::userAdmin::USER;
    userLoginConfiguration.m_Passwd  = abbaUW::userAdmin::PASSWD;
    userLoginConfiguration.m_AreaID  = abbaUW::userAdmin::AREAID;
    
    userLoginConfiguration.m_AppName = UADM_APPLICATION_NAME;
    
    m_UserAdminLogin = libBatch::userAdmin::UserAdminLoginPtr( new libBatch::userAdmin::UserAdminLogin(userLoginConfiguration) );
    m_UserAdminLogin->login();
}

void AppInitializer::handleUserAdminDBParameter()
{
    METHODNAME_DEF( AppInitializer, handleUserAdminDBParameter );
	BLOG_TRACE_METHOD( m_Logger, fun );

    assert( m_UserAdminLogin->isloggedIn() );

    using namespace libBatch::dbConnection;
    DBConnectionConfiguration dbConnectionConfiguration;
    dbConnectionConfiguration.m_AppName     = abbaUW::USER_ADMIN_APPLICATION_NAME;
    dbConnectionConfiguration.m_CountryCode = m_AbbaUWCmdLineEvaluator->getAppArgs().m_CountryCode;

    m_UserAdminDBConfiguration = libBatch::userAdmin::UserAdminDBConfigurationPtr( new libBatch::userAdmin::UserAdminDBConfiguration(dbConnectionConfiguration) );
    m_UserAdminDBConfiguration->configure();
}

const cmdLine::AppArgs& AppInitializer::getAppArgs() const
{
    METHODNAME_DEF( AppInitializer, getAppArgs );
	BLOG_TRACE_METHOD( m_Logger, fun );

    return m_AbbaUWCmdLineEvaluator->getAppArgs();
}

const libBatch::userAdmin::UserLoginConfiguration& AppInitializer::getUserLoginConfiguration() const
{
    METHODNAME_DEF( AppInitializer, getUserLoginConfiguration );
	BLOG_TRACE_METHOD( m_Logger, fun );

    return m_UserAdminLogin->getUserLoginConfiguration();
}

const libBatch::dbConnection::DBConnectionConfiguration& AppInitializer::getDBConnectionConfiguration() const
{
    METHODNAME_DEF( AppInitializer, getDBConnectionConfiguration );
	BLOG_TRACE_METHOD( m_Logger, fun );

    return m_UserAdminDBConfiguration->getDBConnectionConfiguration();
}

} // end namespace returnproposer
