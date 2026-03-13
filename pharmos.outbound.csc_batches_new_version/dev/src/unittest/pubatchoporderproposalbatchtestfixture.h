//---------------------------------------------------------------------------//
/*! \file copybatchordertestfixture.h
 *  \brief definition of CopyBatchOdersTestFixture class
 *  \author Xin Zhou
 *  \date 30.06.2013
 */ 
//-----------------------------------------------------------------------//
#ifndef GUARD_PU_BATCH_O_P_ORDER_PROPOSAL_BATCH_TEST_FIXTURE_H
#define GUARD_PU_BATCH_O_P_ORDER_PROPOSAL_BATCH_TEST_FIXTURE_H

#include "dbfixturebase.h"
#include "unittestconnection.h"
#include "../CSC_Batches_OrderProposal/pubatchorderposdm.h"
#include "libbasardbaspect_connectionref.h"

namespace cscUnitTest
{
class PuBatchOrderPosOPBatchTestFixture : public DBFixtureBase
{
public:
	PuBatchOrderPosOPBatchTestFixture();
	virtual	~PuBatchOrderPosOPBatchTestFixture();

	

	const basar::db::aspect::ConnectionRef getConnectionP() const;
	basar::db::sql::ConnectionRef getSQLConnectionP() const;
	static const basar::db::aspect::ConnectionRef::DatabaseInfo & getDBInfoP();
	const basar::db::aspect::ConnectionRef getSingleConnection();

private:
	PuBatchOrderPosOPBatchTestFixture( const PuBatchOrderPosOPBatchTestFixture & r );
	PuBatchOrderPosOPBatchTestFixture operator = ( const PuBatchOrderPosOPBatchTestFixture & r );

	void prepareDBTables();
	void cleanupDBTables();

	//unittest::utils::UnitTestConnection		m_ConnectionP;
	basar::db::aspect::ConnectionRef		m_ActivConnP;

public:
	domMod::OrderProposalTransfer::PuBatchOrderPosDM		m_PuBatchOrderPosDM;
};

} // end namespace cscUnitTest

#endif	// GUARD_ORDER_PROPOSAL_UNIT_TEST_FIXTURE_H
