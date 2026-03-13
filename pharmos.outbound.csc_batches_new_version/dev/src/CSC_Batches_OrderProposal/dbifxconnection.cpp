//--------------------------------------------------------------------------------------------------//
/*! \file		dbifxconnection.cpp
 *  \brief		definition of informix connection class
 *  \author     Johann Franz
 *  \date       09.08.2012
 *  \version	00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------//
//  include section
//--------------------------------------------------------------------------------------------------//
#include "dbifxconnection.h"
#include "loggerpool.h"
#include <classinfo_macros.h>

//--------------------------------------------------------------------------------------------------//
//  class definition section
//--------------------------------------------------------------------------------------------------//
namespace abbaUW
{
namespace dbConnection
{

/*!	\throw no-throw */
DBIfxConnection::DBIfxConnection( const basar::db::sql::DatabaseInfo& databaseInfo )
: DBConnectionBase( databaseInfo )
{
	METHODNAME_DEF( DBIfxConnection, DBIfxConnection )
	BLOG_TRACE_METHOD( m_Logger, fun );

}

/*!	\throw no-throw */
DBIfxConnection::~DBIfxConnection()
{
	METHODNAME_DEF( DBIfxConnection, ~DBIfxConnection )
	BLOG_TRACE_METHOD( m_Logger, fun );

}

void DBIfxConnection::connect()
{
	METHODNAME_DEF( DBIfxConnection, connect )
	BLOG_TRACE_METHOD( m_Logger, fun );
    
    DBConnectionBase::connect();
    setIsolationLevel( basar::INFX_COMITTEDREAD );
    const basar::UInt32 seconds = 30;
    setLockModeWait  ( basar::WAITSECS, seconds );
}

basar::ConnectionTypeEnum DBIfxConnection::getConnectionType() const
{
    return basar::INFORMIX;
}

} // end namespace dbConnection
} // end namespace abbaUW
