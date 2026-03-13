//-------------------------------------------------------------------------------------------------//
/*! \file cscbatchestestfixture.h
 *  \brief Test fixture for CSC_Batches test
 *  \author Beatrix Trömel
 *  \date 11.02.2008
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_CSCBATCHES_H
#define GUARD_CSCBATCHES_H

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
//#include "copybatchordersdm.h"
//#include "ozekorderuc.h"
#include "unittestconnection.h"
#include <libbasardbaspect_accessorpropertytable.h>

//-------------------------------------------------------------------------------------------------//
// using declarations
//-------------------------------------------------------------------------------------------------//
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;

namespace CscBatchesTest {
  class CscBatchesTestFixture {
  public:
	CscBatchesTestFixture();
	int checkCount_Recordsets(AccessorPropertyTableRef PropSet);
	int checkCount_orderhead();
	int checkCount_orderpos();
	virtual ~CscBatchesTestFixture();
	basar::Int16 m_BranchNo;

  protected:
	bool m_result;
//	domMod::CopyBatchOrdersDM m_CopyBatchOrdersDM;
//	basar::db::aspect::AccessorPropertyTableRef m_resultPropTab;
//	basar::db::aspect::AccessorPropertyTable_YIterator m_resultYit; 

  private:
//+++	CscBatchesTestFixture(const CscBatchesHistoryTestFixture& fixture);
	//+++CscBatchesTestFixture operator= (const CscBatchesHistoryTestFixture& fixture);
	void cleanDbTables();
	void prepareDbTables();
  };
}// namespace
#endif
