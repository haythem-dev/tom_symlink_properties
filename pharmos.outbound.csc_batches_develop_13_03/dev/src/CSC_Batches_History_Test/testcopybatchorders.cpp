//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include <UnitTest++.h>
#include <libbasar_definitions.h>
#include <libbasardbaspect_accessor.h>
#include <libbasardbaspect.h>
#include <unittestconnection.h>
#include "cscbatchesautotestfixture.h"
#include "copybatchordersdm.h"
#include "copybatchorderacc.h"

//-------------------------------------------------------------------------------------------------//
// using declarations
//-------------------------------------------------------------------------------------------------//
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;

//-------------------------------------------------------------------------------------------------//
// Testsuite
//-------------------------------------------------------------------------------------------------//
namespace CscBatchesAutoTest {
  SUITE(CscBatchesAutoTestSuite) {
	//*************************************** testConnection ******************************************************
	TEST(testConnection) {
		try {
			printf("starting testConnection ...");
			extern pdm::testutils::UnitTestConnection g_utConnect;
			
			//--- check connection:
			CHECK(g_utConnect.getCurrentConnection().isOpen());

			printf("done.\n");	
		}// try
		catch(basar::Exception &except) {
			printf( "\n=== basar exception caught =================================\n\n" );
			printf( "%s\n", except.what().c_str() );
			CHECK(false);
		}// catch
	}// TEST(testConnection)

	//*************************************** testReleaseBatchOrders ******************************************************
	TEST_FIXTURE(CscBatchesAutoTestFixture, testReleaseBatchOrders) {
		try {
			printf("starting testReleaseBatchOrders ...");
			extern pdm::testutils::UnitTestConnection g_utConnect;
			
			//--- to reference to accessors, necessary for linking a static lib!
			{
				copybatchorderacc::saveArchiveBatchOrderHeads dummy1;
			}

			//--- check copy:
			// create yit:
			basar::I18nString strYit = "";
			basar::I18nString strYit1 = "";
			// branch no:
			strYit1.format("%s=%d;", "branchno", m_BranchNo);
			strYit.append(strYit1);			
			strYit1.format("%s=%d;", "lessthanorderdate", 20080901);
			strYit.append(strYit1);			
			AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(strYit);
			CHECK(m_CopyBatchOrdersDM.copyBatchOrders(yit));

			//////////////////////////////////////////// Checks ////////////////////////////////////////////
			// 3 orders in state 2 + 2 in state 9 - 1 order with wrong orderdate (9 orders in total)
			printf("checkCount_aorderhead ");	
			CHECK_EQUAL(4,checkCount_aorderhead());

			// 9 orders in total - 4 archived
			printf("checkCount_orderhead ");	
			CHECK_EQUAL(5,checkCount_orderhead());

			// 10 positions in total. 
			// relevant: 
			//		4 of orderno 111 (5 is not relevant because of wrong branchno)
			//		0 of orderno 112 (not relevant because not in OrderHead)
			//		3 of orderno 115
			//		0 of orderno 116 (wrong orderdate)
			//		1 of orderno 117 (1 is not relevant because of wrong branchno)
			//		0 of orderno 118 (wrong branchno)
			//		0 of orderno 120 (not relevant because not in OrderHead)
			printf("checkCount_aorderpos ");	
			CHECK_EQUAL(8,checkCount_aorderpos());

			// 15 positions in total - 8 archived
			printf("checkCount_orderpos ");	
			CHECK_EQUAL(7,checkCount_orderpos());

			printf("done.\n");	
		}// try
		catch(basar::Exception &except) {
			printf( "\n=== basar exception caught =================================\n\n" );
			printf( "%s\n", except.what().c_str() );
			CHECK(false);
		}// catch
	}// TEST(testReleaseBatchOrders)
  }// TestSuite
}// namespace
