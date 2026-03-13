// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnut.h" /Fp$(IntDir)/cmnut.pch
#include "cmnut.h"

#include "dbfixturebase.h"

namespace cscUnitTest
{
const basar::db::aspect::ConnectionRef::DatabaseInfo & DBFixtureBase::getDBInfo()
{
	static basar::db::aspect::ConnectionRef::DatabaseInfo dbInfo;

	dbInfo.setInfx( "zdev21_tcp", "och21" ); //"db1_zdev1_tcp", "zpos3ch"

	return dbInfo;
}

DBFixtureBase::DBFixtureBase()
:	m_Connection( unittest::utils::UnitTestConnection::getUnitTestConnection( getDBInfo() ) )
{
}

DBFixtureBase::~DBFixtureBase()
{
}

const basar::db::aspect::ConnectionRef DBFixtureBase::getConnection() const
{
	return m_Connection.getCurrentConnection();
}

basar::db::sql::ConnectionRef DBFixtureBase::getSQLConnection() const
{
	basar::db::aspect::ConnectionRef aspectConnection = getConnection();
	return aspectConnection.getSqlConnection();
}

} // end namespace cscUnitTest
