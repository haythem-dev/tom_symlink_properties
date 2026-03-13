// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnut.h" /Fp$(IntDir)/cmnut.pch
#include "cmnut.h"

#include "copybatchordertestfixture.h"
#include "copybatchordersdm.h"
#include "copybatchorderacc.h"
#include <libbasardbsql_statementref.h>
#include <libbasardbsql_resultsetref.h>

namespace cscUnitTest
{
	using namespace unittest;

	SUITE( TestCopyBatchOrderDM )
	{
		//													methodName_condition_expectation
		FIXTURE_TEST_START( CopyBatchOrderTestFixture, copyBatchOrders_existingInTable_copyInArchivTable )
		{
			//prepareDBTables() for orderhead & orderpos is in C'tor already done!
			
			
			// find accounts and compare values
			basar::ConstString sql_aorderhead = "SELECT COUNT(*) FROM aorderhead WHERE (branchno = 999) AND (orderno >= 10000) ";
			basar::ConstString sql_orderhead = "SELECT COUNT(*) FROM orderhead WHERE (branchno = 999) AND (orderno >= 10000) ";

			basar::db::sql::StatementRef stmtRef = getSQLConnection().createStatement();
			basar::Decimal countAOrderHeadBefore = stmtRef.executeAggregate( sql_aorderhead );
			basar::Decimal countOrderHeadBefore = stmtRef.executeAggregate( sql_orderhead );

			basar::ConstString sql_aorderpos = "SELECT COUNT(*) FROM aorderpos WHERE (branchno = 999) AND (orderno >= 10000) ";
			basar::ConstString sql_orderpos = "SELECT COUNT(*) FROM orderpos WHERE (branchno = 999) AND (orderno >= 10000) ";

			
			basar::Decimal countAOrderPosBefore = stmtRef.executeAggregate( sql_aorderpos );
			basar::Decimal countOrderPosBefore = stmtRef.executeAggregate( sql_orderpos );

			
			basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_CopyBatchOrdersDM.getSearchYit();
			yitSearch.setInt16("branchno", 12);
			yitSearch.setInt32("lessthanorderdate", 20101231);
			bool copyOK = m_CopyBatchOrdersDM.copyBatchOrders( yitSearch );

			if (false == copyOK)
			{
				//Error handling
				BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrderTestFixture--CopyBatchOrdersDM::copyBatchOrders()."); 
				throw; 
			}

			// find accounts and compare values	
			basar::Decimal countAOrderHeadAfter = stmtRef.executeAggregate( sql_aorderhead );
			basar::Decimal countOrderHeadAfter = stmtRef.executeAggregate( sql_orderhead );

			basar::Decimal countAOrderPosAfter = stmtRef.executeAggregate( sql_aorderpos );
			basar::Decimal countOrderPosAfter = stmtRef.executeAggregate( sql_orderpos );

			//check the actual rows in aorderpos
			CHECK_EQUAL( 2, countAOrderPosAfter.toInt32()-countAOrderPosBefore.toInt32());
			//check the actual rows in orderpos
			CHECK_EQUAL( -2, countOrderPosAfter.toInt32()-countOrderPosBefore.toInt32());
			
			//check the actual rows in aorderhead
			CHECK_EQUAL( 2, countAOrderHeadAfter.toInt32()-countAOrderHeadBefore.toInt32());
			//Check the actual rows in orderhead
			CHECK_EQUAL( -2, countOrderHeadAfter.toInt32()-countOrderHeadBefore.toInt32());
			
		}
		FIXTURE_TEST_END

	} // end TESTSUITE
} // end namespace cscUnitTest

