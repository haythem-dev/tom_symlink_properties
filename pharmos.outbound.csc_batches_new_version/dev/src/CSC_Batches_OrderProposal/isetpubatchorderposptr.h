//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief  
 *  \author Hendrik Polzin 
 *  \date 15.08.2013   */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_I_SET_PU_BATCH_ORDER_POS_PTR_H
#define GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_I_SET_PU_BATCH_ORDER_POS_PTR_H

#include <boost/shared_ptr.hpp>

namespace domMod 
{
    namespace OrderProposalTransfer
    {
        class IPuBatchOrderPositionWriter;
        typedef boost::shared_ptr< domMod::OrderProposalTransfer::IPuBatchOrderPositionWriter > IPuBatchOrderPositionWriterPtr;
    }
}

#endif 
