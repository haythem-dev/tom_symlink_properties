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

//--------------------------------------------------------------------------------------------------//
// class definition section
//--------------------------------------------------------------------------------------------------//
namespace abbaUW
{

AppSession::AppSession( libBatch::dbConnection::DBConnectionBasePtr dbconnection, const cmdLine::AppArgs& appArgs, 
                        const libBatch::userAdmin::UserLoginConfiguration& userLoginConfiguration )
:m_DBConnection( dbconnection ), m_AppArgs( appArgs ), m_UserLoginConfiguration( userLoginConfiguration )
{
    METHODNAME_DEF( AppSession, AppSession ); (void) fun;
}

AppSession::~AppSession()
{
    METHODNAME_DEF( AppSession, ~AppSession ); (void) fun;
}

void AppSession::start()
{
    m_DBConnection->connect();
}

void AppSession::end()
{
    m_DBConnection->disconnect();
}

std::ostream& AppSession::toStream( std::ostream& strm ) const
{
    METHODNAME_DEF( AppSession, toStream ); (void) fun;
        
    strm << std::endl;
    strm << m_AppArgs;
    strm << std::endl;
    strm << m_UserLoginConfiguration;
    strm << std::endl;
    strm << m_DBConnection->getDBInfo();
            
    return strm;
}

} // end namespace abbaUW
