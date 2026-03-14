//---------------------------------------------------------------------------//
/*! \file copybatchordertestfixture.cpp
 *  \brief implementation of CopyBatchOrderTestFixture class
 *  \author Xin Zhou
 *  \date 30.06.2013
 */ 
//-----------------------------------------------------------------------//

// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnut.h" /Fp$(IntDir)/cmnut.pch
#include "cmnut.h"

#include "orderproporderproposalbatchtestfixture.h"
//#include "copybatchorderacc.h"
#include <libbasardbsql_statementref.h>
#include "../CSC_Batches_OrderProposal/orderproposalinitparams.h"
#include "../CSC_Batches_OrderProposal/properties_definitions.h"


namespace cscUnitTest
{
	OrderProposalOPBatchFixture::OrderProposalOPBatchFixture():
		m_OrderProposalDM(getConnection())
	{
		//copybatchorderacc::saveArchiveBatchOrderHeads dummy; dummy;
		domMod::OrderProposalTransfer::OrderProposalInitParams param;
		
		m_OrderProposalDM.init( param );
		prepareDBTables();
	}

	OrderProposalOPBatchFixture::~OrderProposalOPBatchFixture()
	{
		cleanupDBTables();
		
		m_OrderProposalDM.shutdown();
	}

	void OrderProposalOPBatchFixture::cleanupDBTables()
	{
		//cleaning the datatables after testing
		//printf("cleaning the datatables ORDERPROPOSAL after testing");
		//printf("\n");	

		basar::db::sql::StatementRef statement = getSQLConnection().createStatement();
		basar::VarString sql_cleanup("DELETE FROM orderproposal WHERE (branchno = 90) AND (" + properties::ARTICLE_NO.getName() + ">=99999990); ");
		statement.execute(sql_cleanup);
	}

	void OrderProposalOPBatchFixture::prepareDBTables()
	{
		//prepare the datatable orderpos
		//printf("starting prepareDbTables for ORDERPROPOSAL ...");
		//printf("\n");

		using namespace properties;

		basar::db::sql::StatementRef statement = getSQLConnection().createStatement();		

		//status 2, orderdate<given value, will be archived
//1
		basar::VarString sql(	
			"INSERT INTO " 
			"orderproposal " 
			"(" 
			+ BRANCH_NO.getName() + "," + 
			ARTICLE_NO.getName() + "," + 
			TRANSFER_TIME_BI.getName() + "," + 
			ORDER_NO_BATCH.getName() + "," + 
			TRANSFER_TIME.getName() + ","+ 
			ORDER_PROP_PROCESSED_TIME.getName() + "," + 
			PURCHASE_ORDER_NO.getName() + "," + 
			DATE_OF_PURCHASE.getName() + "," + 
			ORDER_PROP_DISCLAIM_NOTE.getName()+ "," + 
			ORDER_PROPOSAL_TIME.getName() + "," + 
			POS_NO_BATCH.getName()+ "," + 
			ORDER_PROPOSAL_QTY.getName()         
			+ ") " 
			"VALUES "
			"("
			"90, "
			"99999999,"  
			"'" + basar::DateTime(basar::Int32(20130401),basar::Int32(135959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", "
			"1, " + 
			"'" + basar::DateTime(basar::Int32(20130401),basar::Int32(125959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", " + 
			"'" + basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", "
			"0," 
			"0, "
			"'Unittest for OrderProposalBatch', " + 
			"'" + basar::DateTime(basar::Int32(20130401),basar::Int32(115959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ","
			"1,"
			"100)" 
		);
		
		statement.execute( sql );

//2
		sql=	
			"INSERT INTO " 
			"orderproposal " 
			"(" 
			+ BRANCH_NO.getName() + "," + 
			ARTICLE_NO.getName() + "," + 
			TRANSFER_TIME_BI.getName() + "," + 
			ORDER_NO_BATCH.getName() + "," + 
			TRANSFER_TIME.getName() + ","+ 
			ORDER_PROP_PROCESSED_TIME.getName() + "," + 
			PURCHASE_ORDER_NO.getName() + "," + 
			DATE_OF_PURCHASE.getName() + "," + 
			ORDER_PROP_DISCLAIM_NOTE.getName()+ "," + 
			ORDER_PROPOSAL_TIME.getName() + "," + 
			POS_NO_BATCH.getName()+ "," + 
			ORDER_PROPOSAL_QTY.getName()         
			+ ") " 
			"VALUES "
			"("
			"90, "
			"99999999," + 
			"'" + basar::DateTime(basar::Int32(20130401),basar::Int32(135959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", "
			"1, " + 
			"'" + basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", " + 
			"'" + basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", "
			"0, "
			"0," 
			"'Unittest for OrderProposalBatch', " + 
			"'" + basar::DateTime(basar::Int32(20130401),basar::Int32(115959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ","
			"2,"
			"0)" 
		;
			
		statement.execute( sql );

//3		
		sql=
			"INSERT INTO " 
			"orderproposal " 
			"(" 
			+ BRANCH_NO.getName() + "," + 
			ARTICLE_NO.getName() + "," + 
			TRANSFER_TIME_BI.getName() + "," + 
			ORDER_NO_BATCH.getName() + "," + 
			TRANSFER_TIME.getName() + ","+ 
			ORDER_PROP_PROCESSED_TIME.getName() + "," + 
			PURCHASE_ORDER_NO.getName() + "," + 
			DATE_OF_PURCHASE.getName() + "," + 
			ORDER_PROP_DISCLAIM_NOTE.getName()+ "," + 
			ORDER_PROPOSAL_TIME.getName() + "," + 
			POS_NO_BATCH.getName()+ "," + 
			ORDER_PROPOSAL_QTY.getName()         
			+ ") " 
			"VALUES "
			"("
			"90," 
			"99999998," + 
			"'" + basar::DateTime(basar::Int32(20130401),basar::Int32(135959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", "
			"2, " + 
			"'" + basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", " + 
			"'" + basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", "
			"0, "
			"0, "
			"'Unittest for OrderProposalBatch', " + 
			"'" + basar::DateTime(basar::Int32(20130401),basar::Int32(115959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ","
			"1,"
			"200)" 
		;

		statement.execute( sql );

//4
		sql=	
			"INSERT INTO " 
			"orderproposal " 
			"(" 
			+ BRANCH_NO.getName() + "," + 
			ARTICLE_NO.getName() + "," + 
			TRANSFER_TIME_BI.getName() + "," + 
			ORDER_NO_BATCH.getName() + "," + 
			TRANSFER_TIME.getName() + ","+ 
			ORDER_PROP_PROCESSED_TIME.getName() + "," + 
			PURCHASE_ORDER_NO.getName() + "," + 
			DATE_OF_PURCHASE.getName() + "," + 
			ORDER_PROP_DISCLAIM_NOTE.getName()+ "," + 
			ORDER_PROPOSAL_TIME.getName() + "," + 
			POS_NO_BATCH.getName()+ "," + 
			ORDER_PROPOSAL_QTY.getName()         
			+ ") " 
			"VALUES "
			"("
			"90, "
			"99999997," + 
			"'" + basar::DateTime(basar::Int32(20130401),basar::Int32(135959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", "
			"3, " + 
			"'" + basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", " + 
			"'" + basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", "
			"0, "
			"0, "
			"'Unittest for OrderProposalBatch', " + 
			"'" + basar::DateTime(basar::Int32(20130401),basar::Int32(115959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ","
			"1,"
			"300)" 
		;

		statement.execute( sql );

//5
		sql=
			"INSERT INTO " 
			"orderproposal " 
			"(" 
			+ BRANCH_NO.getName() + "," + 
			ARTICLE_NO.getName() + "," + 
			TRANSFER_TIME_BI.getName() + "," + 
			ORDER_NO_BATCH.getName() + "," + 
			TRANSFER_TIME.getName() + ","+ 
			ORDER_PROP_PROCESSED_TIME.getName() + "," + 
			PURCHASE_ORDER_NO.getName() + "," + 
			DATE_OF_PURCHASE.getName() + "," + 
			ORDER_PROP_DISCLAIM_NOTE.getName()+ "," + 
			ORDER_PROPOSAL_TIME.getName() + "," + 
			POS_NO_BATCH.getName()+ "," + 
			ORDER_PROPOSAL_QTY.getName()         
			+ ") " 
			"VALUES "
			"("
			"90, "
			"99999996," + 
			"'" + basar::DateTime(basar::Int32(20130401),basar::Int32(135959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", "
			"4, " + 
			"'" + basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", " + 
			"'" + basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ", "
			"0, "
			"0, "
			"'Unittest for OrderProposalBatch', " + 
			"'" + basar::DateTime(basar::Int32(20130401),basar::Int32(115959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" +  ","
			"1,"
			"400)" 
		;

		statement.execute( sql );
	
			
	}
} // end namespace cscUnitTest
