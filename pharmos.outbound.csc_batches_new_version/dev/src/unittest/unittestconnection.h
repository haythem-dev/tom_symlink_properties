#ifndef GUARD_LIBTEST_UNITTEST_CONNECTION_H
#define GUARD_LIBTEST_UNITTEST_CONNECTION_H

#include <libbasardbsql_connectionref.h>
#include <libbasardbaspect.h>
#include <libbasardbsql_databaseinfo.h>

namespace unittest
{
namespace utils
{
class UnitTestConnection
{
public:	
	static UnitTestConnection & getUnitTestConnection( const basar::db::aspect::ConnectionRef::DatabaseInfo & dbInfo );

	virtual ~UnitTestConnection();
	void connect( const basar::db::aspect::ConnectionRef::DatabaseInfo & dbInfo );
	void disconnect();
	basar::db::aspect::ConnectionRef getCurrentConnection() const;
	basar::db::sql::ConnectionRef getSqlConnection() const;
	const bool isConnected() const;

private:
	UnitTestConnection();
	basar::db::aspect::ConnectionRef m_ActivConn;
};
}

} // end namespace unittest

#endif
