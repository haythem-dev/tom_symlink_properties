    //-----------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief      declaration of database login class
 *  \author     Bischof Bjoern
 *  \date       31.12.2010
 *  \version    00.00.00.01 first version
 */ 
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
// include section
//-----------------------------------------------------------------------------------------------//
#include "useradmindbconfiguration.h"
#include "classinfo_macros.h"
#include "loggerpool.h"
#include <libbasarlogin_manager.h>
#include "useradminparametergetter.h"
#include <sstream>
#include <iomanip>
#include "useradminfailureexception.h"

//-----------------------------------------------------------------------------------------------//
// class definition section
//-----------------------------------------------------------------------------------------------//
namespace libBatch
{
namespace userAdmin
{

UserAdminDBConfiguration::UserAdminDBConfiguration( libBatch::dbConnection::DBConnectionConfiguration& dbConnectionConfiguration )
:  m_Logger( LoggerPool::LoggerUserAdmin ), m_DBConnectionConfiguration( dbConnectionConfiguration )
{
    METHODNAME_DEF( UserAdminDBConfiguration, UserAdminDBConfiguration )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

UserAdminDBConfiguration::~UserAdminDBConfiguration()
{
}

void UserAdminDBConfiguration::configure()
{
    METHODNAME_DEF( UserAdminDBConfiguration, configure )
	BLOG_TRACE_METHOD( m_Logger, fun );

    UserAdminParameterGetter userAdminParameterGetter;

    // todo PK stay independent or not?
    /*if( false == userAdminParameterGetter.getParameterValue( m_DBConnectionConfiguration.m_AreaID, 
                                                             m_DBConnectionConfiguration.m_DBParameterName, 
                                                             m_DBConnectionConfiguration.m_DB ) ){
    */
	// Read Database-Name from UsrAdmin
    if( false == userAdminParameterGetter.getParameterValue( m_DBConnectionConfiguration.m_AppName, 
                                                             m_DBConnectionConfiguration.m_CountryCode, 
                                                             m_DBConnectionConfiguration.m_DBParameterName, 
                                                             m_DBConnectionConfiguration.m_DB ) ){
        std::stringstream ss; 
        ss << "ERROR: No parameter value found for:" << m_DBConnectionConfiguration;
        throw libBatch::userAdmin::UserAdminFailureException( basar::ExceptInfo(fun, ss.str().c_str(), __FILE__, __LINE__) );
    }

    /*if( false == userAdminParameterGetter.getParameterValue( m_DBConnectionConfiguration.m_AreaID, 
                                                             m_DBConnectionConfiguration.m_DBSrvParameterName, 
                                                             m_DBConnectionConfiguration.m_DBSrv) ){*/
	// Read DB-Server-Name from UsrAdmin
    if( false == userAdminParameterGetter.getParameterValue( m_DBConnectionConfiguration.m_AppName, 
                                                             m_DBConnectionConfiguration.m_CountryCode, 
                                                             m_DBConnectionConfiguration.m_DBSrvParameterName, 
                                                             m_DBConnectionConfiguration.m_DBSrv ) ){
                                                             
        std::stringstream ss; 
        ss << "ERROR: No parameter value found for:" << m_DBConnectionConfiguration;
        throw libBatch::userAdmin::UserAdminFailureException( basar::ExceptInfo(fun, ss.str().c_str(), __FILE__, __LINE__) );        
    }
	

    logDBConfiguration();
}

void UserAdminDBConfiguration::logDBConfiguration() const
{
    METHODNAME_DEF( UserAdminDBConfiguration, logDBConfiguration )
	BLOG_TRACE_METHOD( m_Logger, fun );

    if( m_Logger.isEnabledFor( log4cplus::INFO_LOG_LEVEL ) ){
        std::stringstream ss;
        ss  << "Successfully received db configuration from user admin: " << m_DBConnectionConfiguration;
        m_Logger.forcedLog( log4cplus::INFO_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
    }
}

const libBatch::dbConnection::DBConnectionConfiguration& UserAdminDBConfiguration::getDBConnectionConfiguration() const
{
    METHODNAME_DEF( UserAdminDBConfiguration, getDBConnectionConfiguration )
	BLOG_TRACE_METHOD( m_Logger, fun );

    assert( m_DBConnectionConfiguration.isConfigured() );
    return m_DBConnectionConfiguration;
}

} // end namespace userAdmin
} // end namespace libBatch
