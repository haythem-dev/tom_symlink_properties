//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief  
 *  \author Hendrik Polzin 
 *  \date 15.08.2013  */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_I_GET_PU_BATCH_ORDER_POS_PTR_H
#define GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_I_GET_PU_BATCH_ORDER_POS_PTR_H

#include <boost/shared_ptr.hpp>

namespace domMod 
{
    namespace OrderProposalTransfer
    {
        class IPuBatchOrderPositionFinder;
        typedef boost::shared_ptr< domMod::OrderProposalTransfer::IPuBatchOrderPositionFinder > IPuBatchOrderPositionFinderPtr;
    }
}

#endif
