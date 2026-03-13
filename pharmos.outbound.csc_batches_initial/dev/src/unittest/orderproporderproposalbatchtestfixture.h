//---------------------------------------------------------------------------//
/*! \file copybatchordertestfixture.h
 *  \brief definition of CopyBatchOdersTestFixture class
 *  \author Xin Zhou
 *  \date 30.06.2013
 */ 
//-----------------------------------------------------------------------//
#ifndef GUARD_ORDER_PROP_ORDER_PROPOSAL_BATCH_FIXTURE_H
#define GUARD_ORDER_PROP_ORDER_PROPOSAL_BATCH_FIXTURE_H

#include "dbfixturebase.h"
#include "../CSC_Batches_OrderProposal/orderproposaldm.h"

namespace cscUnitTest
{
class OrderProposalOPBatchFixture : public DBFixtureBase
{
public:
	OrderProposalOPBatchFixture();
	virtual	~OrderProposalOPBatchFixture();

	domMod::OrderProposalTransfer::OrderProposalDM		m_OrderProposalDM;

private:
	OrderProposalOPBatchFixture( const OrderProposalOPBatchFixture & r );
	OrderProposalOPBatchFixture operator = ( const OrderProposalOPBatchFixture & r );

	void prepareDBTables();
	void cleanupDBTables();
};

} // end namespace cscUnitTest

#endif	// GUARD_ORDER_PROPOSAL_UNIT_TEST_FIXTURE_H
