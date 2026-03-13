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

//-------------------------------------------------------------------------------------------------//
// using declarations
//-------------------------------------------------------------------------------------------------//
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using domMod::BatchOrderDM;

//-------------------------------------------------------------------------------------------------//
// Testsuite
//-------------------------------------------------------------------------------------------------//
SUITE(CSCBatchTestSuite) {
	TEST(findBatchOrdersByPattern){
		try {
			printf("starting findBatchOrdersByPattern ...");
			extern pdm::testutils::UnitTestConnection g_utConnect;
			
			//*** to reference to accessors, necessary for linking a static lib!
			{
				batchorderacc::SelBatchOrdersByPattern a;
				ordertypeacc::SelOrderTypesByOrderNo b;
				batchorderposacc::SelBatchOrderPosByBranchNoAndOrderNo c;
				searchorderacc::Dummy d;
				actionorderacc::Dummy e;
			}

			//*** start Test
			BatchOrderDM DM;
			CHECK(g_utConnect.getCurrentConnection().isOpen());
			DM.init(g_utConnect.getCurrentConnection());  
			AccessorPropertyTableRef PropTabIn = DM.getOrderHeaderPropTab();
			AccessorPropertyTable_YIterator yitIn= PropTabIn.insert(basar::FOR_CLEAN);
			yitIn.setInt16("branchno",5);
			yitIn.setInt16("weekday",1);
			AccessorPropertyTableRef PropTabOut = DM.findBatchOrdersByPattern(yitIn);
			CHECK(!PropTabOut.isNull());
			AccessorPropertyTable_YIterator yitOut = PropTabOut.begin();
			CHECK(!yitOut.isEnd());
			DM.shutdown();
			printf("done.\n");	
		}// try
		catch(basar::Exception &except) {
			printf( "\n=== basar exception caught =================================\n\n" );
			printf( "%s\n", except.what().c_str() );
			CHECK(false);
		}// catch
	}// TEST(findBatchOrdersByPattern)

			/*
		AccessorPropertyTableRef propTab = m_DM.findBookingClosureByBookingMonth(Date(m_Year + 1, 6, 30));	// (random year + 1)-06-30

		CHECK( !propTab.isNull() );

		if ( !propTab.isNull() )
		{
			AccessorPropertyTable_YIterator yIt = propTab.begin();

			CHECK( !yIt.isNull() );

			if ( !yIt.isNull() )
			{
				Int32 count = 0;

				Int32 id;
				Date  month;
				Date  closure;

				while ( yIt != propTab.end() )
				{
					id      = yIt.getInt32(lit::dm_bookingclosure::BOOKING_CLOSURE_ID.getName());
					month   = yIt.getDate (lit::dm_bookingclosure::BOOKING_MONTH.getName());
					closure = yIt.getDate (lit::dm_bookingclosure::BOOKING_CLOSURE.getName());

					CHECK (id != 0);

					CHECK(month   == Date(m_Year +  1, 6, 1));
					CHECK(closure == Date(m_Year + 11, 7, 2));

					++count;
					++yIt;
				}

				CHECK_EQUAL(1, count);	// 1 selected rows
			}
		}
			*/

}// TestSuite
