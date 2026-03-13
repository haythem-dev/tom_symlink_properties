#ifndef GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_I_GET_ORDER_PROPOSAL_PTR_H
#define GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_I_GET_ORDER_PROPOSAL_PTR_H

#include <boost/shared_ptr.hpp>

namespace domMod 
{
    namespace OrderProposalTransfer
    {
        class IOrderProposalFinder;
        typedef boost::shared_ptr< domMod::OrderProposalTransfer::IOrderProposalFinder > IOrderProposalFinderPtr;
    }
}

#endif 
