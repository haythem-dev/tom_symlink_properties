//---------------------------------------------------------------------------//
/*! \file copybatchorderfixture.cpp
 *  \brief implementation of CopyBatchOrderFixture class
 *  \author Xin Zhou
 *  \date 30.06.2013
 */ 
//-----------------------------------------------------------------------//

// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnut.h" /Fp$(IntDir)/cmnut.pch
#include "cmnut.h"

#include "copybatchordertestfixture.h"
#include "copybatchorderacc.h"
#include <libbasardbsql_statementref.h>

namespace cscUnitTest
{
	CopyBatchOrderTestFixture::CopyBatchOrderTestFixture()
	{
		copybatchorderacc::saveArchiveBatchOrderHeads dummy; dummy;
		m_CopyBatchOrdersDM.init( getConnection() );
		prepareDBTables();
	}

	CopyBatchOrderTestFixture::~CopyBatchOrderTestFixture()
	{
		cleanupDBTables();
		
		m_CopyBatchOrdersDM.shutdown();
	}

	void CopyBatchOrderTestFixture::cleanupDBTables()
	{
		//cleaning the datatables after testing
		printf("cleaning the datatables orderpos/aorderpos/orderhead/aorderhead after testing");
		printf("\n");	

		basar::db::sql::StatementRef statement = getSQLConnection().createStatement();
		basar::VarString sql_cleanup_orderhead;
		sql_cleanup_orderhead.format("DELETE FROM orderhead WHERE ((branchno = 12) AND (orderno >= 10000) AND (orderno <= 10003)); ");
		basar::VarString sql_cleanup_aorderhead;
		sql_cleanup_aorderhead.format("DELETE FROM aorderhead WHERE ((branchno = 12) AND (orderno >= 10000) AND (orderno <= 10003)); ");
		basar::VarString sql_cleanup_orderpos;
		sql_cleanup_orderpos.format("DELETE FROM orderpos WHERE ((branchno = 12) AND (orderno >= 10000) AND (orderno <= 10003)); ");
		basar::VarString sql_cleanup_aorderpos; 
		sql_cleanup_aorderpos.format("DELETE FROM aorderpos WHERE ((branchno = 12) AND (orderno >= 10000) AND (orderno <= 10003)); ");

		statement.execute(sql_cleanup_orderhead);
		statement.execute(sql_cleanup_aorderhead);
		statement.execute(sql_cleanup_orderpos);
		statement.execute(sql_cleanup_aorderpos);
	}

	void CopyBatchOrderTestFixture::prepareDBTables()
	{
		//prepare the datatable orderpos
		printf("starting prepareDbTables for copying orderhead ...");
		printf("\n");

		basar::db::sql::StatementRef statement = getSQLConnection().createStatement();		

		basar::VarString sql;

		//status 2, orderdate<given value, will be archived
		sql.format(	"INSERT INTO "
					"orderhead "
					"("
					"branchno, orderno, status, ordertype, headertext, orderdate"
					") "
					"VALUES "
					"( "
					"12, 10000, 2,'UW', 'Testdaten Xin', 20100101"
					") "
					);
		statement.execute( sql );
		
		//status 9, orderdate<given value, will be archived
		sql.format(	"INSERT INTO "
					"orderhead "
					"("
					"branchno, orderno, status, ordertype, headertext, orderdate"
					") "
					"VALUES "
					"( "
					"12, 10001, 9,'NO', 'Testdaten Xin', 20100103"
					") "
					);
		statement.execute( sql );
		
		//status 2, but orderdate > given value, will NOT be archived
		sql.format(	"INSERT INTO "
					"orderhead "
					"("
					"branchno, orderno, status, ordertype, headertext, orderdate"
					") "
					"VALUES "
					"( "
					"12, 10002, 2,'UW', 'Testdaten Xin', 20130101"
					") "
					);
		statement.execute( sql );
		
		//status 0, will NOT be archived
		sql.format(	"INSERT INTO "
					"orderhead "
					"("
					"branchno, orderno, status, ordertype, headertext, orderdate"
					") "
					"VALUES "
					"( "
					"12, 10003, 0,'NO', 'Testdaten Xin', 20100101"
					") "
					);
		statement.execute( sql );


		//prepare the datatable orderpos
		printf("starting prepareDbTables for copying orderpos ...");
		printf("\n");
		
		//the corresponding dataset in orderhead should be archived, so in orderpos
		sql.format(	"INSERT INTO "
					"orderpos "
					"("
					"branchno, orderno, posno, articleno"
					") "
					"VALUES "
					"( "
					"12, 10000, 40,400000"
					") "
					);
		statement.execute( sql );
		
		//the corresponding dataset in orderhead should be archived, so in orderpos
		sql.format(	"INSERT INTO "
					"orderpos "
					"("
					"branchno, orderno, posno, articleno"
					") "
					"VALUES "
					"( "
					"12, 10001, 41,400001"
					") "
					);
		statement.execute( sql );

		//the corresponding dataset in orderhead should NOT be archived, NOR in orderpos
		sql.format(	"INSERT INTO "
					"orderpos "
					"("
					"branchno, orderno, posno, articleno"
					") "
					"VALUES "
					"( "
					"12, 10002, 42,400002"
					") "
					);
		statement.execute( sql );
		
		//the corresponding dataset in orderhead should NOT be archived, NOR in orderpos
		sql.format(	"INSERT INTO "
					"orderpos "
					"("
					"branchno, orderno, posno, articleno"
					") "
					"VALUES "
					"( "
					"12, 10003, 43,400003"
					") "
					);
		statement.execute( sql );


		
	}
} // end namespace cscUnitTest
