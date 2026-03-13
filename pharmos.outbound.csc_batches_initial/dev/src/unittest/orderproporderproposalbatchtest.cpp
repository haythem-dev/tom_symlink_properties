// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnut.h" /Fp$(IntDir)/cmnut.pch
#include "cmnut.h"

#include "orderproporderproposalbatchtestfixture.h"
#include "orderproposaldm.h"
//#include "copybatchorderacc.h"
#include <libbasardbsql_statementref.h>
#include <libbasardbsql_resultsetref.h>
#include <libbasar_definitions.h>
#include "../CSC_Batches_OrderProposal/properties_definitions.h"

namespace cscUnitTest
{
	using namespace unittest;

	SUITE( TestOPBatchOrderProposalDM )
	{
		//													methodName_condition_expectation
		FIXTURE_TEST_START( OrderProposalOPBatchFixture, findOrderProposals_existingInTable_copyInArchivTable )
		{
			basar::db::aspect::AccessorPropertyTableRef ref = m_OrderProposalDM.findOrderProposals();

			basar::db::aspect::AccessorPropertyTable_YIterator yit = ref.begin();
			
			CHECK( !ref.isEmpty());
			
			int count = 0;
			while(!yit.isEnd()){
				if(yit.getInt16(properties::BRANCH_NO.getName())==90){
					if(yit.getInt32(properties::ORDER_NO_BATCH.getName())==2){
						count++;
					}
					if(yit.getInt32(properties::ORDER_NO_BATCH.getName())==3){
						count++;
					}
					if(yit.getInt32(properties::ORDER_NO_BATCH.getName())==4){
						count++;
					}
				}
				yit++;
			}
		
			CHECK(count>=3);
		}
		FIXTURE_TEST_END

		FIXTURE_TEST_START( OrderProposalOPBatchFixture, updateFromPuBatchOrderPos_existingInTable_copyInArchivTable )
		{
			using namespace properties;
			basar::db::aspect::AccessorPropertyTableRef ref = m_OrderProposalDM.getTable();
			ref.clear();
			basar::InsertTypeEnum status = basar::FOR_UPDATE;

			basar::db::aspect::AccessorPropertyTable_YIterator yit = ref.insert(status);//basar::InsertTypeEnum::FOR_UPDATE);
			
			basar::DateTime datet = basar::DateTime::getCurrent();
			basar::DateTime datett = basar::DateTime(basar::Int32(20130401),basar::Int32(125959000));
			basar::DateTime dateot = basar::DateTime(basar::Int32(20130401),basar::Int32(115959000));
			basar::DateTime datebt = basar::DateTime(basar::Int32(99991231),basar::Int32(235959000));
			//for updatestep

//1
			yit.setInt16(BRANCH_NO.getName(), 90);
			yit.setInt32(ORDER_NO_BATCH.getName(),1);
			yit.setDateTime(ORDER_PROPOSAL_TIME.getName() ,basar::DateTime(basar::Int32(20130401),basar::Int32(115959000)));
			yit.setDateTime(ORDER_PROP_PROCESSED_TIME.getName(),datet);
			yit.setInt32(PURCHASE_ORDER_NO.getName(),0);
			yit.setInt32(DATE_OF_PURCHASE.getName(),0);
			yit.setString(ORDER_PROP_DISCLAIM_NOTE.getName(),"Unittest");

//3
			yit = ref.insert(status);
			yit.setInt16(BRANCH_NO.getName(), 90);
			yit.setInt32(ORDER_NO_BATCH.getName(),2);
			yit.setDateTime(ORDER_PROPOSAL_TIME.getName() ,basar::DateTime(basar::Int32(20130401),basar::Int32(115959000)));
			yit.setDateTime(ORDER_PROP_PROCESSED_TIME.getName(),datet);
			yit.setInt32(PURCHASE_ORDER_NO.getName(),5);
			yit.setInt32(DATE_OF_PURCHASE.getName(),0);
			yit.setString(ORDER_PROP_DISCLAIM_NOTE.getName(),"Unittest");

//4
			yit = ref.insert(status);
			yit.setInt16(BRANCH_NO.getName(), 90);
			yit.setInt32(ORDER_NO_BATCH.getName(),3);
			yit.setDateTime(ORDER_PROPOSAL_TIME.getName() ,basar::DateTime(basar::Int32(20130401),basar::Int32(115959000)));
			yit.setDateTime(ORDER_PROP_PROCESSED_TIME.getName(),datet);
			yit.setInt32(PURCHASE_ORDER_NO.getName(),0);
			yit.setInt32(DATE_OF_PURCHASE.getName(),20001111);
			yit.setString(ORDER_PROP_DISCLAIM_NOTE.getName(),"Unittest");

//5
			yit = ref.insert(status);
			yit.setInt16(BRANCH_NO.getName(), 90);
			yit.setInt32(ORDER_NO_BATCH.getName(),4);
			yit.setDateTime(ORDER_PROPOSAL_TIME.getName() ,basar::DateTime(basar::Int32(20130401),basar::Int32(115959000)));
			yit.setDateTime(ORDER_PROP_PROCESSED_TIME.getName(),datet);
			yit.setInt32(PURCHASE_ORDER_NO.getName(),6);
			yit.setInt32(DATE_OF_PURCHASE.getName(),20001111);
			yit.setString(ORDER_PROP_DISCLAIM_NOTE.getName(),"Unittest");

			m_OrderProposalDM.updateOrderProposals(ref);
	
			basar::db::sql::StatementRef stmtRef = getSQLConnection().createStatement();
			basar::db::sql::ResultsetRef rr;


		//1
			basar::VarString sql = 
				"SELECT * FROM orderproposal where " + 
				BRANCH_NO.getName() + "=90 AND " + 
				ORDER_PROPOSAL_TIME.getName() + "='"+dateot.toString("%Y-%m-%d %H:%M:%S%F3") +"' AND " + 
				ARTICLE_NO.getName() + ">=99999999 AND " +
				ORDER_NO_BATCH.getName() + "=1 AND " +
				ORDER_PROPOSAL_QTY.getName() + ">0";
			
			rr = stmtRef.executeQuery(sql);

			rr.next();
			CHECK(rr.getDateTime(ORDER_PROP_PROCESSED_TIME.getName())==datet);
			CHECK(rr.getInt32(DATE_OF_PURCHASE.getName())==0);
			CHECK(rr.getInt32(PURCHASE_ORDER_NO.getName())==0);
			CHECK(rr.getDateTime(TRANSFER_TIME_BI.getName())==basar::DateTime(basar::Int32(20130401),basar::Int32(135959000)));
			//CHECK(rr.getString(ORDER_PROP_DISCLAIM_NOTE.getName())=="Unittest");

		//2
			 sql = 
				"SELECT * FROM orderproposal where " + 
				BRANCH_NO.getName() + "=90 AND " + 
				ORDER_PROPOSAL_TIME.getName() + "='"+dateot.toString("%Y-%m-%d %H:%M:%S%F3") +"' AND " + 
				ARTICLE_NO.getName() + ">=99999999 AND " +
				ORDER_NO_BATCH.getName() + "=1 AND " +
				ORDER_PROPOSAL_QTY.getName()+ "=0";
			
			rr = stmtRef.executeQuery(sql);

			rr.next();
			CHECK(rr.getDateTime(ORDER_PROP_PROCESSED_TIME.getName())==datebt);
			CHECK(rr.getInt32(DATE_OF_PURCHASE.getName())==0);
			CHECK(rr.getInt32(PURCHASE_ORDER_NO.getName())==0);
			CHECK(rr.getDateTime(TRANSFER_TIME_BI.getName())==basar::DateTime(basar::Int32(20130401),basar::Int32(135959000)));


		//3
			 sql = 
				"SELECT * FROM orderproposal where " + 
				BRANCH_NO.getName() + "=90 AND " + 
				ORDER_PROPOSAL_TIME.getName() + "='"+dateot.toString("%Y-%m-%d %H:%M:%S%F3") +"' AND " + 
				ARTICLE_NO.getName() + ">=99999998 AND " +
				ORDER_NO_BATCH.getName() + "=2 AND " +
				ORDER_PROPOSAL_QTY.getName()+ ">0";
			
			rr = stmtRef.executeQuery(sql);
	
			rr.next();
			CHECK(rr.getDateTime(ORDER_PROP_PROCESSED_TIME.getName())==datet);
			CHECK(rr.getInt32(DATE_OF_PURCHASE.getName())==0);
			CHECK(rr.getInt32(PURCHASE_ORDER_NO.getName())==5);
			CHECK(rr.getDateTime(TRANSFER_TIME_BI.getName())==datebt);


		//4
			sql = 
				"SELECT * FROM orderproposal where " + 
				BRANCH_NO.getName() + "=90 AND " + 
				ORDER_PROPOSAL_TIME.getName() + "='"+dateot.toString("%Y-%m-%d %H:%M:%S%F3") +"' AND " + 
				ARTICLE_NO.getName() + ">=99999997 AND " +
				ORDER_NO_BATCH.getName() + "=3 AND " +
				ORDER_PROPOSAL_QTY.getName()+ ">0";
			
			rr = stmtRef.executeQuery(sql);

			rr.next();
			CHECK(rr.getDateTime(ORDER_PROP_PROCESSED_TIME.getName())==datet);
			CHECK(rr.getInt32(DATE_OF_PURCHASE.getName())==20001111);
			CHECK(rr.getInt32(PURCHASE_ORDER_NO.getName())==0);
			CHECK(rr.getDateTime(TRANSFER_TIME_BI.getName())==datebt);
	
			
		//5
			sql = 
				"SELECT * FROM orderproposal where " + 
				BRANCH_NO.getName() + "=90 AND " + 
				ORDER_PROPOSAL_TIME.getName() + "='"+dateot.toString("%Y-%m-%d %H:%M:%S%F3") +"' AND " + 
				ARTICLE_NO.getName() + ">=99999996 AND " +
				ORDER_NO_BATCH.getName() + "=4 AND " +
				ORDER_PROPOSAL_QTY.getName()+ ">0";
			
			rr = stmtRef.executeQuery(sql);
	
			rr.next();
			rr.next();
			CHECK(rr.getDateTime(ORDER_PROP_PROCESSED_TIME.getName())==datet);
			CHECK(rr.getInt32(DATE_OF_PURCHASE.getName())==20001111);
			CHECK(rr.getInt32(PURCHASE_ORDER_NO.getName())==6);
			CHECK(rr.getDateTime(TRANSFER_TIME_BI.getName())==datebt);
		
		}
		FIXTURE_TEST_END

		//FIXTURE_TEST_START( OrderProposalOPBatchFixture, updateTransferTime_existingInTable_copyInArchivTable )
		//{
		//	using namespace properties;
		//	basar::db::aspect::AccessorPropertyTableRef ref = m_OrderProposalDM.getTable();
		//	ref.clear();
		//	basar::InsertTypeEnum status = basar::FOR_UPDATE;

		//	basar::db::aspect::AccessorPropertyTable_YIterator yit = ref.insert(status);//basar::InsertTypeEnum::FOR_UPDATE);
		//	
		//	basar::DateTime datet = basar::DateTime::getCurrent();
		//	//for updatestep
		//	yit.setInt16(BRANCH_NO.getName(), 90);
		//	yit.setInt32(POS_NO_BATCH.getName(), 7);
		//	yit.setInt32(ORDER_NO_BATCH.getName(), 1);
		//	yit.setDateTime(ORDER_PROPOSAL_TIME.getName(), basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)));
		//	yit.setDateTime(TRANSFER_TIME.getName(), datet);
		//	
		//	

		//	basar::VarString sql = "Select * from orderproposal where " + BRANCH_NO.getName() + "=90 AND " + ORDER_NO_BATCH.getName() + "=1 AND " + ORDER_PROPOSAL_TIME.getName() + "='"+basar::DateTime(basar::Int32(99992131),basar::Int32(235959000)).toString("%Y-%m-%d %H:%M:%S%F3") +"' AND " + POS_NO_BATCH.getName() + "=7";
		//	//printf( sql.c_str());
		//	basar::db::sql::StatementRef stmtRef = getSQLConnection().createStatement();

		//	basar::db::sql::ResultsetRef rr = stmtRef.executeQuery(sql);

		//	rr.next();
		//	CHECK(rr.getFetchedRows()==1);

		//	CHECK(rr.getDateTime(TRANSFER_TIME.getName())==basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)));
		//	
		//	m_OrderProposalDM.updateTransferTime(yit);

		//	rr = stmtRef.executeQuery(sql);

		//	rr.next();

		//	CHECK(rr.getFetchedRows()==1);

		//	CHECK(rr.getDateTime(TRANSFER_TIME.getName())==datet);
		//	
		//}
		//FIXTURE_TEST_END



	} // end TESTSUITE
} // end namespace cscUnitTest

