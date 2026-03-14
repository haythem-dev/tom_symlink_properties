#ifndef GUARD_DB_FIXTURE_BASE_H
#define GUARD_DB_FIXTURE_BASE_H

#include "testfixturebase.h"
#include "unittestconnection.h"

namespace cscUnitTest
{
class DBFixtureBase : public TestFixtureBase
{
public:
	static const basar::db::aspect::ConnectionRef::DatabaseInfo & getDBInfo();

	DBFixtureBase();
	virtual	~DBFixtureBase();
	const basar::db::aspect::ConnectionRef getConnection() const;
	basar::db::sql::ConnectionRef getSQLConnection() const;

private:
	DBFixtureBase( const DBFixtureBase & r );
	DBFixtureBase operator = ( const DBFixtureBase & r );

	unittest::utils::UnitTestConnection		m_Connection;
};

} // end namespace cscUnitTest

#endif	// GUARD_DB_FIXTURE_BASE_H
