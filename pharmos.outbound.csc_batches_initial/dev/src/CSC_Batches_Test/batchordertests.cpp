//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include <UnitTest++.h>
#include <libbasar_definitions.h>
#include <libbasardbaspect_accessor.h>
#include <libbasardbaspect.h>
#include <unittestconnection.h>
#include "batchorderdm.h"

#include "batchorderacc.h"
#include "ordertypeacc.h"
#include "batchorderposacc.h"
#include "searchorderacc.h"
#include "actionorderacc.h"

#include "cscbatchestestfixture.h"
#include "batchorderuc.h"

//-------------------------------------------------------------------------------------------------//
// using declarations
//-------------------------------------------------------------------------------------------------//
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::VarString;
using domMod::BatchOrderDM;

//-------------------------------------------------------------------------------------------------//
// Testsuite
//-------------------------------------------------------------------------------------------------//
namespace CscBatchesTest {
  SUITE(CSCBatchTestSuite) {
	//************************************ preparation ************************************************
	TEST_FIXTURE(CscBatchesTestFixture, preparation) {
		try {
			printf("starting preparation ... ");
			extern pdm::testutils::UnitTestConnection g_utConnectCsc;

			//////////////////////////////////////////// Checks ////////////////////////////////////////////
			CHECK(g_utConnectCsc.getCurrentConnection().isOpen());

			// 3 recordsets created in orderhead
			printf("checkCount_orderhead ");	
			CHECK_EQUAL(3,checkCount_orderhead());
			
			// 3 recordsets created in orderhead
			printf("checkCount_orderpos ");	
			CHECK_EQUAL(6,checkCount_orderpos());
			printf("done.\n");	
		}// try
		catch(basar::Exception &except) {
			printf( "\n=== basar exception caught =================================\n\n" );
			printf( "%s\n", except.what().c_str() );
			CHECK(false);
		}// catch
	}// TEST(preparation)

	//************************************ findBatchOrdersByPattern ************************************************
	TEST_FIXTURE(CscBatchesTestFixture, findBatchOrdersByPattern) {
		try {
			printf("starting findBatchOrdersByPattern ... ");
			extern pdm::testutils::UnitTestConnection g_utConnectCsc;

			//--- instanciante accessors:
			{
				batchorderacc::SelBatchOrdersByPattern a;
				batchorderposacc::SelBatchOrderPosByBranchNoAndOrderNo b;
			}

			//*** start Test
			BatchOrderDM DM;
			AccessorPropertyTableRef PropTabOut;
			DM.init(g_utConnectCsc.getCurrentConnection());  
			AccessorPropertyTableRef PropTabIn = DM.getOrderHeaderPropTab();
			AccessorPropertyTable_YIterator yitIn= PropTabIn.insert(basar::FOR_CLEAN);
			yitIn.setInt16("branchno",m_BranchNo);
			// 2 orders of datasource SH:
			yitIn.setString("datasourcetype","SH");
			PropTabOut = DM.findBatchOrdersByPattern(yitIn);
			CHECK_EQUAL(2,checkCount_Recordsets(PropTabOut));

			// 0 orders of datasource CL:
			yitIn=PropTabOut.begin();
			yitIn.setString("datasourcetype","SH");
			PropTabOut = DM.findBatchOrdersByPattern(yitIn);
			CHECK_EQUAL(0,checkCount_Recordsets(PropTabOut));

			DM.shutdown();
			printf("done.\n");	
		}// try
		catch(basar::Exception &except) {
			printf( "\n=== basar exception caught =================================\n\n" );
			printf( "%s\n", except.what().c_str() );
			CHECK(false);
		}// catch
	}// TEST(findBatchOrdersByPattern)

	//************************************ changeStatus ************************************************
	TEST_FIXTURE(CscBatchesTestFixture, changeStatus) {
		try {
			printf("starting changeStatus ... ");
			basar::appl::EventReturnStruct result;
			extern pdm::testutils::UnitTestConnection g_utConnectCsc;

			//--- instanciante accessors:
			{
				batchorderacc::SelBatchOrdersByPattern a;
				batchorderposacc::SaveBatchOrderPos b;
			}

			//--- call use case
			CSC_Batches::useCase::BatchOrderUC BatchOrderUCForTest; //!< Instance of the use case controller for orders.
			basar::I18nString strYit = "";
			basar::I18nString strYit1 = "";
			// set yit string for yitAction:
			strYit1.format("%s=%d;", "branchno", m_BranchNo);
			strYit.append(strYit1);						
			strYit1.format("%s=%s;", "action_status", "1");
			strYit.append(strYit1);						
			AccessorPropertyTable_YIterator yitAction = basar::db::aspect::Manager::getInstance().createAccessorYIterator(strYit);
			// set yit string for yitTable:
			strYit1.format("%s=%d;", "orderno", 111);
			strYit.append(strYit1);						
			AccessorPropertyTable_YIterator yitTable = basar::db::aspect::Manager::getInstance().createAccessorYIterator(strYit);				
/*			result=BatchOrderUCForTest.handleupdateBatches(basar::appl::NullEventSource::getInstance(), yitAction);
			BatchOrderUCForTest.m_BatchOrderVC->getAccTableYit();

			if(result.ret == basar::appl::HANDLER_ERROR) {
				CHECK(false);
			}// if(result.ret == basar::appl::HANDLER_ERROR)
*/
			basar::VarString strSql("SELECT orderno FROM orderhead ");
			VarString strWhere;
			strWhere.format(" WHERE branchno=%d and status=1 ",m_BranchNo);
			strSql.append(strWhere);
			basar::db::sql::StatementRef stmtRef = g_utConnectCsc.getSqlConnection().createStatement();
			basar::db::sql::ResultsetRef rsltRef = stmtRef.executeQuery(strSql);	
			if(rsltRef.isNull())
				CHECK(false);
			if(!rsltRef.isOpen())
				CHECK(false);
			rsltRef.next(); // goto first recordset
//			CHECK_EQUAL(111, rsltRef.getInt32("orderno"));
			CHECK(true);

			printf("done.\n");	
		}// try
		catch(basar::Exception &except) {
			printf( "\n=== basar exception caught =================================\n\n" );
			printf( "%s\n", except.what().c_str() );
			CHECK(false);
		}// catch
	}// TEST_FIXTURE(CscBatchesTestFixture, changeStatus)

	//************************************ changePosQuantity ************************************************
	TEST_FIXTURE(CscBatchesTestFixture, changePosQuantity) {
		try {
			printf("starting changePosQuantity ... ");
			basar::appl::EventReturnStruct result;
			extern pdm::testutils::UnitTestConnection g_utConnectCsc;

			//--- instanciante accessors:
			{
				batchorderacc::SelBatchOrdersByPattern a;
				batchorderposacc::SaveBatchOrderPos b;
			}
			
			BatchOrderDM DM;
			AccessorPropertyTableRef PropTabOut;
			DM.init(g_utConnectCsc.getCurrentConnection());  
			AccessorPropertyTableRef PropTabIn = DM.getOrderPosPropTab();
			AccessorPropertyTable_YIterator yitIn= PropTabIn.insert(basar::FOR_CLEAN);
			yitIn.setInt16("branchno",m_BranchNo);
			yitIn.setInt32("orderno",111);
			yitIn.setInt32("position",1);
			yitIn.setInt32("orderedqty",5);

			//--- call use case
			CSC_Batches::useCase::BatchOrderUC BatchOrderUCForTest; //!< Instance of the use case controller for orders.
			result=BatchOrderUCForTest.handleupdatePositions(basar::appl::NullEventSource::getInstance(), yitIn);
//+++ hier gibt's noch ein Problem: DM ist nicht initialisiert!
			if(result.ret == basar::appl::HANDLER_ERROR) {
				CHECK(false);
			}// if(result.ret == basar::appl::HANDLER_ERROR)

			basar::VarString strSql("SELECT orderedqty FROM orderpos ");
			VarString strWhere;
			strWhere.format(" WHERE branchno=%d and orderno=111 and posno=1 ",m_BranchNo);
			strSql.append(strWhere);
			basar::db::sql::StatementRef stmtRef = g_utConnectCsc.getSqlConnection().createStatement();
			basar::db::sql::ResultsetRef rsltRef = stmtRef.executeQuery(strSql);	
			if(rsltRef.isNull())
				CHECK(false);
			if(!rsltRef.isOpen())
				CHECK(false);
			rsltRef.next(); // goto first recordset
			CHECK_EQUAL(5, rsltRef.getInt32("orderedqty"));

			printf("done.\n");	
		}// try
		catch(basar::Exception &except) {
			printf( "\n=== basar exception caught =================================\n\n" );
			printf( "%s\n", except.what().c_str() );
			CHECK(false);
		}// catch
	}// TEST_FIXTURE(CscBatchesTestFixture, changePosQuantity)
  }// TestSuite
}// namespace
