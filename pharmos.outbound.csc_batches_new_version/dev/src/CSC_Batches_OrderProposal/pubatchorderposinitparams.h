//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief      Init-params for PuBatchOrderPosDM.
 *              Now carries the PurchaseDBParameterProxy so the DM can resolve
 *              the remote dblink name/server at SQL execution time.
 */
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_PU_BATCH_ORDER_POS_INITPARAMS_H
#define GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_PU_BATCH_ORDER_POS_INITPARAMS_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "ipurchasedbparameterproxyptr.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace domMod
{
namespace OrderProposalTransfer
{

//-------------------------------------------------------------------------------------------------//
// struct declaration
//-------------------------------------------------------------------------------------------------//
    struct PuBatchOrderPosInitParams
    {
    // Proxy that supplies purchasedb / purchasedbsrv strings from UserAdmin parameters.
    // Must be set before PuBatchOrderPosDM::init() is called.
    abbaUW::infrastructure::db::IPurchaseDBParameterProxyPtr purchaseDBParameterProxy;
        
        PuBatchOrderPosInitParams()
     
        {}
};
	    
//-------------------------------------------------------------------------------------------------//
     
} // namespace OrderProposalTransfer
} // namespace domMod

//-------------------------------------------------------------------------------------------------//

#endif // GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_PU_BATCH_ORDER_POS_INITPARAMS_H
