//---------------------------------------------------------------------------//
/*! \file copybatchordertestfixture.h
 *  \brief definition of CopyBatchOdersTestFixture class
 *  \author Xin Zhou
 *  \date 30.06.2013
 */ 
//-----------------------------------------------------------------------//
#ifndef GUARD_COPYBATCHORDER_TEST_FIXTURE_H
#define GUARD_COPYBATCHORDER_TEST_FIXTURE_H

#include "dbfixturebase.h"
#include "copybatchordersdm.h"

namespace cscUnitTest
{
class CopyBatchOrderTestFixture : public DBFixtureBase
{
public:
	CopyBatchOrderTestFixture();
	virtual	~CopyBatchOrderTestFixture();

	domMod::CopyBatchOrdersDM		m_CopyBatchOrdersDM;

private:
	CopyBatchOrderTestFixture( const CopyBatchOrderTestFixture & r );
	CopyBatchOrderTestFixture operator = ( const CopyBatchOrderTestFixture & r );

	void prepareDBTables();
	void cleanupDBTables();
};

} // end namespace cscUnitTest

#endif	// GUARD_COPYBATCHORDER_TEST_FIXTURE_H
