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

#include "pubatchoporderproposalbatchtestfixture.h"
//#include "copybatchorderacc.h"
#include <libbasardbsql_statementref.h>
#include "../CSC_Batches_OrderProposal/properties_definitions.h"
#include "pubatchorderposinitparams.h"

namespace cscUnitTest
{

	const basar::db::aspect::ConnectionRef::DatabaseInfo & PuBatchOrderPosOPBatchTestFixture::getDBInfoP()
	{
		static basar::db::aspect::ConnectionRef::DatabaseInfo dbInfo;

		dbInfo.setInfx( "zdev21_tcp", "ich21" );
		//dbInfo.setInfx( "db1_zdev1_tcp", "zpps1ch" );

		return dbInfo;
	}

	PuBatchOrderPosOPBatchTestFixture::PuBatchOrderPosOPBatchTestFixture() :
		m_ActivConnP(getSingleConnection()),m_PuBatchOrderPosDM(getConnectionP())
	{
		//copybatchorderacc::saveArchiveBatchOrderHeads dummy; dummy;
		domMod::OrderProposalTransfer::PuBatchOrderPosInitParams param;
		m_PuBatchOrderPosDM.init( param );

		prepareDBTables();
	}

	PuBatchOrderPosOPBatchTestFixture::~PuBatchOrderPosOPBatchTestFixture()
	{
		cleanupDBTables();

		m_PuBatchOrderPosDM.shutdown();
		if( false == m_ActivConnP.isNull() )
		{							
			m_ActivConnP.close();			
		}
	}


	const basar::db::aspect::ConnectionRef PuBatchOrderPosOPBatchTestFixture::getSingleConnection()
	{
		static basar::db::aspect::ConnectionRef connectionP;
		if(false == (false == connectionP.isNull() && connectionP.isOpen() ) )
		{
			connectionP = basar::db::aspect::Manager::getInstance().createConnect( getDBInfoP() );
			connectionP.setIsolationLevel( basar::INFX_COMITTEDREAD );
			connectionP.setLockModeWait( basar::WAITSECS, 30 );	// 30 secs
		}
	
		return connectionP;
	}


	const basar::db::aspect::ConnectionRef PuBatchOrderPosOPBatchTestFixture::getConnectionP() const
	{
		return m_ActivConnP;
	}

	basar::db::sql::ConnectionRef PuBatchOrderPosOPBatchTestFixture::getSQLConnectionP() const
	{
		basar::db::aspect::ConnectionRef aspectConnection = getConnectionP();
		return aspectConnection.getSqlConnection();
	}

	void PuBatchOrderPosOPBatchTestFixture::cleanupDBTables()
	{
		//cleaning the datatables after testing
		/*printf("cleaning the datatables orderpos/aorderpos/orderhead/aorderhead after testing");
		printf("\n");*/	

		basar::db::sql::StatementRef statement = getSQLConnectionP().createStatement();
		basar::VarString sql_cleanup("DELETE FROM pubatchorderpos WHERE (" + properties::BRANCH_NO.getName() + " = 90) AND (" + properties::ARTICLE_NO.getName() + ">=99999990); ");
		statement.execute(sql_cleanup);
		
	}

	void PuBatchOrderPosOPBatchTestFixture::prepareDBTables()
	{	
		//prepare the datatable orderpos
		/*printf("starting prepareDbTables for copying orderhead ...");
		printf("\n");*/
		
		using namespace properties;
		basar::db::sql::StatementRef statement = getSQLConnectionP().createStatement();		
	
		//status 2, orderdate<given value, will be archived
		basar::VarString sql(	
			"INSERT INTO " 
			"pubatchorderpos " 
			"(" 
			+ BRANCH_NO.getName() + "," + ARTICLE_NO.getName() + "," + ORDER_PROPOSAL_QTY.getName() + "," +ORDER_NO_BATCH.getName() + "," + ORDER_PROP_PROCESSED_TIME.getName() 
			+ "," + PURCHASE_ORDER_NO.getName() + ","+ DATE_OF_PURCHASE.getName() + ","+ ORDER_PROP_DISCLAIM_NOTE.getName() 
			+ ") " 
			"VALUES " 
			"( 90, 99999999, 10, 1, '" + basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)).toString("%Y-%m-%d %H:%M:%S%F3") + "'" + ", 0, 0, 'Unittest for OrderProposalBatch');"
		);
		
		try{
			statement.execute( sql );
		}catch(basar::Exception e){
			printf(e.what().c_str());
			printf(sql.c_str());
		}

		sql= "INSERT INTO " 
			"pubatchorderpos " 
			"(" 
			 + BRANCH_NO.getName() + "," + ARTICLE_NO.getName() + "," + ORDER_PROPOSAL_QTY.getName() + "," +ORDER_NO_BATCH.getName() + "," + ORDER_PROP_PROCESSED_TIME.getName() 
			+ "," + PURCHASE_ORDER_NO.getName() + ","+ DATE_OF_PURCHASE.getName() + ","+ ORDER_PROP_DISCLAIM_NOTE.getName() 
			+ ") " 
			"VALUES " 
			"( 90, 99999999,10, 3, " + "'" + basar::DateTime::getCurrent().toString("%Y-%m-%d %H:%M:%S%F3") + "'" + ", 0, 0, 'Unittest for OrderProposalBatch')";
		
		try{
			statement.execute( sql );
		}catch(basar::Exception e){
			printf(e.what().c_str());
			printf(sql.c_str());
		}

		sql= "INSERT INTO " 
			"pubatchorderpos " 
			"(" 
			+ BRANCH_NO.getName() + "," + ARTICLE_NO.getName() + "," + ORDER_PROPOSAL_QTY.getName() + "," +ORDER_NO_BATCH.getName() + "," + ORDER_PROP_PROCESSED_TIME.getName() 
			+ "," + PURCHASE_ORDER_NO.getName() + ","+ DATE_OF_PURCHASE.getName() + ","+ ORDER_PROP_DISCLAIM_NOTE.getName() 
			+ ") " 
			"VALUES " 
			"(90, 99999999,10, 5, " + "'" + basar::DateTime::getCurrent().toString("%Y-%m-%d %H:%M:%S%F3") + "'" + ", 51, 20130505, 'Unittest for OrderProposalBatch')";

		try{
			statement.execute( sql );
		}catch(basar::Exception e){
			printf(e.what().c_str());
			printf(sql.c_str());
		}
			
	}
} // end namespace cscUnitTest
