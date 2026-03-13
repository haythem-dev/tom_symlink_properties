//--------------------------------------------------------------------------------------------------//
/*! \brief      
 *  \author     Bischof Bjoern
 *  \date       29.12.2010
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "appsession.h"
#include <classinfo_macros.h>
#include <dbconnectionbase.h>
#include <sstream>
#include <iomanip>
#include "loggerpool.h"

//--------------------------------------------------------------------------------------------------//
// class definition section
//--------------------------------------------------------------------------------------------------//
namespace abbaUW
{

AppSession::AppSession( libBatch::dbConnection::DBConnectionBasePtr dbconnectionOut,libBatch::dbConnection::DBConnectionBasePtr dbconnectionIn, const cmdLine::AppArgs& appArgs, 
                        const libBatch::userAdmin::UserLoginConfiguration& userLoginConfiguration )
:m_DBConnectionInbound( dbconnectionIn ),m_DBConnectionOutbound( dbconnectionOut ), m_AppArgs( appArgs ), m_UserLoginConfiguration( userLoginConfiguration )
{
    METHODNAME_DEF( AppSession, AppSession )
	BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
}

AppSession::~AppSession()
{
    METHODNAME_DEF( AppSession, ~AppSession )
	BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
}
void AppSession::start()
{
    m_DBConnectionInbound->connect();
	m_DBConnectionOutbound->connect();
}

void AppSession::end()
{
	m_DBConnectionInbound->disconnect();
	m_DBConnectionOutbound->disconnect();
}

std::ostream& AppSession::toStream( std::ostream& strm ) const
{
    METHODNAME_DEF( AppSession, toStream )
	BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
        
    strm << std::endl;
    strm << m_AppArgs;
    strm << std::endl;
    strm << m_UserLoginConfiguration;
    strm << std::endl;
    strm << m_DBConnectionInbound->getDBInfo();
	strm << m_DBConnectionOutbound->getDBInfo();
            
    return strm;
}

} // end namespace abbaUW

