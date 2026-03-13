//---------------------------------------------------------------------------//
/*! \file orderproposaltest.cpp
 *  \brief implementation of orderproposaltest
 *  \author Xin Zhou
 *  \date 30.06.2013
 */ 
 //-----------------------------------------------------------------------//

// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnut.h" /Fp$(IntDir)/cmnut.pch
#include "cmnut.h"

#include "orderproposaltestfixture.h"
#include "orderproposaldm.h"
#include "orderproposalacc.h"
#include <libbasardbsql_statementref.h>
#include <libbasardbsql_resultsetref.h>

namespace cscUnitTest
{
	using namespace unittest;

	SUITE( TestOrderProposalDM )
	{
		//													methodName_condition_expectation
		FIXTURE_TEST_START( OrderProposalTestFixture, copyOrderProposal_existingDataInOrderProposal_existingDataInAOrderProposal )
		{
			// prepareDBTables() is already done in C'tor!

			// find accounts and compare values
			basar::ConstString sql_aorderproposal = "SELECT COUNT(*) FROM aorderproposal a WHERE (a.branchno = 999) AND (a.articleno=9999999) ";
			basar::ConstString sql_orderproposal = "SELECT COUNT(*) FROM orderproposal a WHERE (a.branchno = 999) AND (a.articleno=9999999) ";

			basar::db::sql::StatementRef stmtRef1 = getSQLConnection().createStatement();
			basar::Decimal countAOrderProposalBefore = stmtRef1.executeAggregate( sql_aorderproposal );
			basar::Decimal countOrderProposalBefore = stmtRef1.executeAggregate( sql_orderproposal );


			// get the YIterator and call copyOrderProposal in DM
			basar::db::aspect::AccessorPropertyTable_YIterator yitSearch = m_OrderProposalDM.getSearchYit();
			yitSearch.setInt16("branchno", 12);
			yitSearch.setInt32("minimumagefororderproposal", 5);
			bool copyOK = m_OrderProposalDM.copyOrderProposal( yitSearch );

			if (false == copyOK)
			{
				//Fehlerbehandlung
				BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "OrderProposalTestFixture--OrderProposalDM::copyOrderProposal()."); 
				throw; 
			}

			// find accounts and compare values
			basar::db::sql::StatementRef stmtRef2 = getSQLConnection().createStatement();
			basar::Decimal countAOrderProposalAfter = stmtRef2.executeAggregate( sql_aorderproposal );
			basar::Decimal countOrderProposalAfter = stmtRef2.executeAggregate( sql_orderproposal );

			//check if 2 rows are archived in aorderproposal
			CHECK_EQUAL( 2, countAOrderProposalAfter.toInt32()-countAOrderProposalBefore.toInt32());
			
			//check if these 2 rows are deleted in orderproposal
			CHECK_EQUAL( -2, countOrderProposalAfter.toInt32()-countOrderProposalBefore.toInt32());

						
		}
		FIXTURE_TEST_END

	} // end TESTSUITE
} // end namespace cscUnitTest

