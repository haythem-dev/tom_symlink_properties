// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnut.h" /Fp$(IntDir)/cmnut.pch
#include "cmnut.h"

#include <libbasar_definitions.h>
#include "pubatchoporderproposalbatchtestfixture.h"
#include <libbasardbsql_statementref.h>
#include <libbasardbsql_resultsetref.h>
#include <libbasardbaspect.h>
#include "../CSC_Batches_OrderProposal/properties_definitions.h"
#include "../CSC_Batches_OrderProposal/orderproposaldm.h"

namespace cscUnitTest
{
	using namespace unittest;

	SUITE( TestOPBatchPuBatchOrderPosDM )
	{
		//													methodName_condition_expectation
		FIXTURE_TEST_START( PuBatchOrderPosOPBatchTestFixture, findPuBatchOrderPositions_DatabaseWithValues_PropertyTableWithValues )
		{
			basar::db::aspect::AccessorPropertyTableRef ref;
			ref = m_PuBatchOrderPosDM.findPuBatchOrderPositions();

			basar::db::aspect::AccessorPropertyTable_YIterator yit = ref.begin();
			
			CHECK( !ref.isEmpty());

			int count = 0;
			while(!yit.isEnd()){
				if(yit.getInt16(properties::BRANCH_NO.getName())==90){
					count++;
				}
				yit++;
			}
			
			CHECK(count>=3);
		}
		FIXTURE_TEST_END

		FIXTURE_TEST_START( PuBatchOrderPosOPBatchTestFixture, addFromOrderProposal_filledPropertyTable_UpdateDatabaseAndPropRefTransfertime )
		{
			using namespace properties;
			
			basar::db::aspect::ConnectionRef con;

			domMod::OrderProposalTransfer::OrderProposalDM orderprop(getConnection());
			domMod::OrderProposalTransfer::OrderProposalInitParams param;
		
			orderprop.init( param );
			
			basar::db::aspect::AccessorPropertyTableRef ref;
			try{
				ref =  orderprop.getTable();
			}catch(basar::Exception& e){
				printf(e.what().c_str());
				throw e;
			}

			ref.clear();

			basar::InsertTypeEnum status = basar::FOR_UPDATE;

			basar::db::aspect::AccessorPropertyTable_YIterator yit = ref.insert(status);//basar::InsertTypeEnum::FOR_UPDATE);
			
			//for insert
			yit.setInt16(BRANCH_NO.getName(), 90);
			yit.setInt32(ARTICLE_NO.getName(), 99999999);
			yit.setInt32(ORDER_PROPOSAL_QTY.getName(), 12);
			yit.setInt32(ORDER_NO_BATCH.getName(),2);
			yit.setInt16( REASON_ID.getName() ,0);
			yit.setDateTime( ORDER_PROPOSAL_TIME.getName(), basar::DateTime(basar::Int32(20100401),basar::Int32(120000000)));//basar::DateTime::getCurrent());
			yit.setDateTime( TRANSFER_TIME.getName(),basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)));

			bool testQTYZero = false;

			try{
				m_PuBatchOrderPosDM.savePuBatchOrderPositions(yit);
			}catch(...){
				
				//throw;
			}

			//for updatestep 
			//yit = ref.insert(status);//basar::InsertTypeEnum::FOR_UPDATE);
			//yit.setInt16(BRANCH_NO.getName(), 90);
			//yit.setInt32(ARTICLE_NO.getName(), 99999999);
			//yit.setInt32(ORDER_PROPOSAL_QTY.getName(), 12);
			//yit.setInt32(ORDER_NO_BATCH.getName(),1);
			//yit.setInt16( ORDER_TYPE_BATCH,	0);
			//yit.setInt16( REASON_ID ,0);
			//yit.setDateTime( ORDER_PROPOSAL_TIME, basar::DateTime(basar::Int32(20100401),basar::Int32(120000000))); // basar::DateTime::getCurrent());

			//try{
			//	m_PuBatchOrderPosDM.savePuBatchOrderPositions(yit);
			//}catch(...){
			//	//throw;
			//}

			yit = ref.insert(status);//basar::InsertTypeEnum::FOR_UPDATE);
			yit.setInt16(BRANCH_NO.getName(), 90);
			yit.setInt32(ARTICLE_NO.getName(), 99999999);
			yit.setInt32(ORDER_PROPOSAL_QTY.getName(), 0);
			yit.setInt32(ORDER_NO_BATCH.getName(),2);
			yit.setInt16( REASON_ID.getName() ,0);
			yit.setDateTime( ORDER_PROPOSAL_TIME.getName(), basar::DateTime(basar::Int32(20100401),basar::Int32(120000000)));//basar::DateTime::getCurrent());
			yit.setDateTime( TRANSFER_TIME.getName(),basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)));
			
			try{
				m_PuBatchOrderPosDM.savePuBatchOrderPositions(yit);
			}catch(...){
				testQTYZero = true;
				//throw;
			}

			CHECK(testQTYZero);
			//m_PuBatchOrderPosDM.commitTransaction();

			basar::db::sql::StatementRef stmtRef = getSQLConnectionP().createStatement();

			basar::VarString sql;
			//sql = "SELECT * FROM pubatchorderpos WHERE ("+ BRANCH_NO.getName() + "= 90) AND (" + ARTICLE_NO.getName() + "= 99999999) ";
			////printf(sql.c_str());
			basar::db::sql::ResultsetRef rr;
			//rr = stmtRef.executeQuery(sql);
			//
			//rr.next();
			////printf("%i", rr.getFetchedRows());
			////CHECK(rr.getFetchedRows()>=4);

			sql = "SELECT * FROM cscpubatchorderpos WHERE ("+ BRANCH_NO.getName() + "= 90) AND (" + ARTICLE_NO.getName() + "= 99999999) AND ("+ORDER_NO_BATCH.getName()+"= 2) ";

			rr = stmtRef.executeQuery(sql);
			
			rr.next();

			CHECK(rr.getFetchedRows()==1);

			//CHECK(rr.getInt32(ORDER_PROPOSAL_QTY.getName())==12);

			yit= ref.begin();

			CHECK(!(yit.getDateTime( TRANSFER_TIME.getName())==basar::DateTime(basar::Int32(99991231),basar::Int32(235959000))));
			yit++;
			CHECK((yit.getDateTime( TRANSFER_TIME.getName())==basar::DateTime(basar::Int32(99991231),basar::Int32(235959000))));

			/*sql = "SELECT * FROM pubatchorderpos WHERE ("+ BRANCH_NO.getName() + "= 90) AND (" + ARTICLE_NO.getName() + "= 99999999) AND ("+ORDER_NO_BATCH.getName()+"= 2) ";

			rr = stmtRef.executeQuery(sql);

			rr.next();

			CHECK(rr.getFetchedRows()==1);

			CHECK(rr.getInt32(ORDER_PROPOSAL_QTY.getName())==12);

			sql = "SELECT * FROM pubatchorderpos WHERE ("+ BRANCH_NO.getName() + "= 90) AND (" + ARTICLE_NO.getName() + "= 99999999) AND ("+ORDER_NO_BATCH.getName()+"= 3) ";

			rr = stmtRef.executeQuery(sql);

			rr.next();

			CHECK(rr.getFetchedRows()==1);

			CHECK(rr.getInt32(ORDER_PROPOSAL_QTY.getName())==10);

			sql = "SELECT * FROM pubatchorderpos WHERE ("+ BRANCH_NO.getName() + "= 90) AND (" + ARTICLE_NO.getName() + "= 99999999) AND ("+ORDER_NO_BATCH.getName()+"= 5) ";

			rr = stmtRef.executeQuery(sql);

			rr.next();

			CHECK(rr.getFetchedRows()==1);

			CHECK(rr.getInt32(ORDER_PROPOSAL_QTY.getName())==10);*/

			orderprop.shutdown();
		}
		FIXTURE_TEST_END

	} // end TESTSUITE
} // end namespace cscUnitTest

