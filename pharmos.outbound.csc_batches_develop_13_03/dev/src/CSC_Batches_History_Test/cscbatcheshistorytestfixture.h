//-------------------------------------------------------------------------------------------------//
/*! \file cscbatchesautotestfixture.h
 *  \brief Test fixture for CSC_Batches_History test
 *  \author Beatrix Trömel
 *  \date 08.09.2008
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_CSCBATCHESAUTOTESTFIXTURE_H
#define GUARD_CSCBATCHESAUTOTESTFIXTURE_H

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "copybatchordersdm.h"
#include "unittestconnection.h"

namespace CscBatchesAutoTest {
  class CscBatchesAutoTestFixture {
  public:
    CscBatchesAutoTestFixture();
    virtual ~CscBatchesAutoTestFixture();
	basar::Int32 m_BranchNo;
	int checkCount_orderhead();
	int checkCount_aorderhead();
	int checkCount_orderpos();
	int checkCount_aorderpos();

  protected:
    bool m_result;
    domMod::CopyBatchOrdersDM m_CopyBatchOrdersDM;
    basar::db::aspect::AccessorPropertyTableRef m_resultPropTab;
    basar::db::aspect::AccessorPropertyTable_YIterator m_resultYit; 

  private:
    CscBatchesAutoTestFixture(const CscBatchesAutoTestFixture& fixture);
    CscBatchesAutoTestFixture operator= (const CscBatchesAutoTestFixture& fixture);
	void cleanDbTables();
	void prepareDbTables();
	void insertIntoOrderHead(basar::db::sql::StatementRef statement);
	void insertIntoOrderPos(basar::db::sql::StatementRef statement);
  };
}// namespace
#endif
