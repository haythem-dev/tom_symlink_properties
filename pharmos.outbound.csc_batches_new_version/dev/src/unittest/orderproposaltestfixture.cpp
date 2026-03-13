//---------------------------------------------------------------------------//
/*! \file orderproposaltestfixture.cpp
 *  \brief implementation of OrderProposalTestFixture class
 *  \author Xin Zhou
 *  \date 30.06.2013
 */ 
 //-----------------------------------------------------------------------//

// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnut.h" /Fp$(IntDir)/cmnut.pch
#include "cmnut.h"

#include "orderproposaltestfixture.h"
#include "orderproposalacc.h"
#include <libbasardbsql_statementref.h>

namespace cscUnitTest
{
	//*************************************** C'tor ******************************************************
	OrderProposalTestFixture::OrderProposalTestFixture()
	{
		orderproposalacc::saveArchiveOrderProposal dummy; dummy;
		m_OrderProposalDM.init( getConnection() );
		prepareDBTables();
	}

	//*************************************** D'tor ******************************************************
	OrderProposalTestFixture::~OrderProposalTestFixture()
	{
		cleanupDBTables();
		m_OrderProposalDM.shutdown();
	}

	//*************************************** prepareDBTables() ******************************************************
	void OrderProposalTestFixture::prepareDBTables()
	{
		printf("starting prepareDbTables for orderproposal ...");
		printf("\n");

		basar::db::sql::StatementRef statement = getSQLConnection().createStatement();		

		basar::VarString sql;
		//orderproposaltime 10 days ago, will be archived, if age is set to 5
		sql.format(	"INSERT INTO "
					"orderproposal "
					"("
					"branchno, articleno, ordernobatch, posnobatch, orderproposaltime, transfertimebi"
					") "
					"VALUES "
					"( "
					"12, 9999999, 10,11, (SELECT CURRENT -10 UNITS DAY FROM systables WHERE tabid=99), '9999-12-31 23:59:59.0'"
					") "
					);
		statement.execute( sql );

		//orderproposaltime 8 days ago, will be archived, if age is set to 5
		sql.format(	"INSERT INTO "
					"orderproposal "
					"("
					"branchno, articleno, ordernobatch, posnobatch, orderproposaltime, transfertimebi"
					") "
					"VALUES "
					"( "
					"12, 9999999, 8,7, (SELECT CURRENT -8 UNITS DAY FROM systables WHERE tabid=99), '9999-12-31 23:59:59.0'"
					") "
					);
		statement.execute( sql );
		
		//orderproposaltime 3 days ago, will NOT be archived, if age is set to 5
		sql.format(	"INSERT INTO "
					"orderproposal "
					"("
					"branchno, articleno, ordernobatch, posnobatch, orderproposaltime, transfertimebi"
					") "
					"VALUES "
					"( "
					"12, 9999999, 4,3, (SELECT CURRENT -3 UNITS DAY FROM systables WHERE tabid=99), '9999-12-31 23:59:59.0'"
					") "
					);
		statement.execute( sql );
	}


	void OrderProposalTestFixture::cleanupDBTables()
	{
		//cleaning the datatables after testing
		printf("cleaning the datatables orderproposal/aorderproposal after testing");
		printf("\n");	

		basar::db::sql::StatementRef statement = getSQLConnection().createStatement();
		basar::VarString sql_cleanup_orderproposal;
		sql_cleanup_orderproposal.format("DELETE FROM orderproposal WHERE ((branchno = 999) AND (articleno = 9999999) AND (ordernobatch = 4) AND (posnobatch = 3)); ");
		basar::VarString sql_cleanup_aorderproposal;
		sql_cleanup_aorderproposal.format("DELETE FROM aorderproposal WHERE ((branchno = 999) AND (articleno = 9999999) AND ((ordernobatch = 10) OR (ordernobatch = 8))); ");		

		statement.execute(sql_cleanup_orderproposal);
		statement.execute(sql_cleanup_aorderproposal);
	}
} // end namespace cscUnitTest
