//--------------------------------------------------------------------------------------------------//
/*! \file connectionbase.cpp
 *  \brief default definition of abstract connection base class
 *  \author Dirk Kapusta
 *  \date 27.01.2009
 *  \version 00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <libbasardbaspect.h>
#include "dbconnectionbase.h"
#include "loggerpool.h"
#include "classinfo_macros.h"


//--------------------------------------------------------------------------------------------------//
// using declaration section
//--------------------------------------------------------------------------------------------------//
using basar::db::aspect::ConnectionRef;
        
//--------------------------------------------------------------------------------------------------//
// class definition section
//--------------------------------------------------------------------------------------------------//
namespace libBatch
{
namespace dbConnection
{

DBConnectionBase::DBConnectionBase( const basar::db::sql::DatabaseInfo& databaseInfo )
: m_Logger( LoggerPool::LoggerDBConnection ), m_DatabaseInfo( databaseInfo )
{
	METHODNAME_DEF( DBConnectionBase, DBConnectionBase )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

DBConnectionBase::~DBConnectionBase()
{
	METHODNAME_DEF( DBConnectionBase, ~DBConnectionBase )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

void DBConnectionBase::connect()
{
	METHODNAME_DEF( DBConnectionBase, connect )
	BLOG_TRACE_METHOD( m_Logger, fun );

    m_DatabaseInfo.eConnectType = getConnectionType();
    m_ActiveConn = basar::db::aspect::Manager::getInstance().createConnect( m_DatabaseInfo );

    logConnect();
}

void DBConnectionBase::logConnect() const
{
	METHODNAME_DEF( DBConnectionBase, logConnect )
	BLOG_TRACE_METHOD( m_Logger, fun );

    if( m_Logger.isEnabledFor( log4cplus::INFO_LOG_LEVEL ) )
	{
        std::stringstream ss;
		ss << "Successfully connected to db: ";
		connectionInfoToStream( ss );
		m_Logger.forcedLog( log4cplus::INFO_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
    }
}

void DBConnectionBase::logDisconnect() const
{
	METHODNAME_DEF( DBConnectionBase, logDisconnect )
	BLOG_TRACE_METHOD( m_Logger, fun );

    if( m_Logger.isEnabledFor( log4cplus::INFO_LOG_LEVEL ) )
	{
        std::stringstream ss;
		ss << "Successfully disconnected from db: ";
		connectionInfoToStream( ss );
		m_Logger.forcedLog( log4cplus::INFO_LOG_LEVEL, ss.str().c_str(), __FILE__, __LINE__ );
    }
}

std::ostream & DBConnectionBase::connectionInfoToStream( std::ostream & str ) const
{
	str << "eConnectType: ";
	switch( m_DatabaseInfo.eConnectType )
	{
		case basar::CICS:		str << "CICS"; break;
		case basar::INFORMIX:	str << "INFORMIX"; break;
		case basar::ODBC:		str << "ODBC"; break;
		default:				str << "<unknown>"; break;
	}

	str << ", dbServer: " << m_DatabaseInfo.dbServer << ", database: " << m_DatabaseInfo.database;

	if( basar::CICS == m_DatabaseInfo.eConnectType )
	{
		str << ", cicsCustom.ipAddr: " << m_DatabaseInfo.cicsCustom.ipAddr << ", cicsCustom.port: ";
		str << m_DatabaseInfo.cicsCustom.port << ", cicsCustom.startTransact: " << m_DatabaseInfo.cicsCustom.startTransact;
	}

	return str;
}

void DBConnectionBase::disconnect()
{
	METHODNAME_DEF( DBConnectionBase, disconnect )
	BLOG_TRACE_METHOD( m_Logger, fun );

    if( true == m_ActiveConn.isNull() ){ return; }
    if (m_ActiveConn.isOpen()){
        m_ActiveConn.close();
    }

    logDisconnect();
}

/*! \return Reference to current connection; used to create an instance of an accessor definition \n \throw no-throw */
const ConnectionRef DBConnectionBase::getCurrentConnection() const
{
	METHODNAME_DEF( DBConnectionBase, getCurrentConnection )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return m_ActiveConn;
}

const basar::db::sql::DatabaseInfo& DBConnectionBase::getDBInfo() const
{
    return m_DatabaseInfo;
}

/*! \throw no-throw */
void DBConnectionBase::setIsolationLevel( basar::IsolationLevelEnum eLevel )
{
	METHODNAME_DEF( DBConnectionBase, setIsolationLevel )
	BLOG_TRACE_METHOD( m_Logger, fun );

    m_ActiveConn.setIsolationLevel( eLevel );
}

/*! \throw no-throw */
void DBConnectionBase::setLockModeWait( basar::LockModeEnum eMode , basar::UInt32 secs )
{
	METHODNAME_DEF( DBConnectionBase, setLockModeWait )
	BLOG_TRACE_METHOD( m_Logger, fun );

    m_ActiveConn.setLockModeWait( eMode, secs );
}

} // end namespace dbConnection
} // end namespace ivc
