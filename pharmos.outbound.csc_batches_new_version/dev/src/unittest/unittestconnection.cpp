// includes
// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnmain.h" /Fp$(IntDir)/cmnmain.pch
#include "cmnut.h"

#include "unittestconnection.h"
#include <libbasarlogin.h>

namespace unittest
{
namespace utils
{
UnitTestConnection & UnitTestConnection::getUnitTestConnection( const basar::db::aspect::ConnectionRef::DatabaseInfo & dbInfo )
{
	static UnitTestConnection connection;
	if( false == connection.isConnected() )
	{
		connection.connect( dbInfo );
	}

	return connection;
}

UnitTestConnection::UnitTestConnection()
{
}

UnitTestConnection::~UnitTestConnection()
{
	disconnect(); // error because of reference counter
}

void UnitTestConnection::connect( const basar::db::aspect::ConnectionRef::DatabaseInfo & dbInfo )
{
	m_ActivConn = basar::db::aspect::Manager::getInstance().createConnect( dbInfo );
	m_ActivConn.setIsolationLevel( basar::INFX_COMITTEDREAD );
	m_ActivConn.setLockModeWait( basar::WAITSECS, 30 );	// 30 secs
}

const bool UnitTestConnection::isConnected() const
{
	return (    false == m_ActivConn.isNull() 
			 && m_ActivConn.isOpen() );
}

void UnitTestConnection::disconnect()
{
	if( false == m_ActivConn.isNull() )
	{							
		m_ActivConn.close();			
	}
}

basar::db::aspect::ConnectionRef UnitTestConnection::getCurrentConnection() const
{
	return m_ActivConn;
}

basar::db::sql::ConnectionRef UnitTestConnection::getSqlConnection() const
{
	return m_ActivConn.getSqlConnection();
}

} // end namespace utils
} // end namespace unittest
