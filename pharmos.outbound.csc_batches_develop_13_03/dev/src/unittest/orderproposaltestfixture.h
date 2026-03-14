//---------------------------------------------------------------------------//
/*! \file orderproposaltestfixture.h
 *  \brief definition of OrderProposalTestFixture class
 *  \author Xin Zhou
 *  \date 30.06.2013
 */ 
//-----------------------------------------------------------------------//

#ifndef GUARD_ORDERPROPOSAL_TEST_FIXTURE_H
#define GUARD_ORDERPROPOSAL_TEST_FIXTURE_H

#include "dbfixturebase.h"
#include "orderproposaldm.h"

namespace cscUnitTest
{
class OrderProposalTestFixture : public DBFixtureBase
{
public:
	OrderProposalTestFixture();
	virtual	~OrderProposalTestFixture();

	domMod::OrderProposalDM		m_OrderProposalDM;

private:
	OrderProposalTestFixture( const OrderProposalTestFixture & r );
	OrderProposalTestFixture operator = ( const OrderProposalTestFixture & r );

	void prepareDBTables();
	void cleanupDBTables();
};

} // end namespace cscUnitTest

#endif	// GUARD_ORDERPROPOSAL_TEST_FIXTURE_H
