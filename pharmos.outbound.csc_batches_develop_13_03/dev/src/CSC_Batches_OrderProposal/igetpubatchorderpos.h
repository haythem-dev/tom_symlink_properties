//----------------------------------------------------------------------------//
/*! \file	
 *  \brief  
 *  \author Hendrik Polzin 
 *  \date 15.08.2013
 */
//----------------------------------------------------------------------------//
#ifndef GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_I_GET_PU_BATCH_ORDER_POS_H
#define GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_I_GET_PU_BATCH_ORDER_POS_H

//----------------------------------------------------------------------------//
// forward declaration section
//----------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
        }
    }
}

//----------------------------------------------------------------------------//
// class declaration section
//----------------------------------------------------------------------------//
namespace domMod
{
namespace OrderProposalTransfer
{
    class IPuBatchOrderPositionFinder
    {
        public:	
            virtual ~IPuBatchOrderPositionFinder(){};
            virtual basar::db::aspect::AccessorPropertyTableRef getTable() const = 0;
			virtual const basar::db::aspect::AccessorPropertyTableRef findPuBatchOrderPositions() =0;
    };

} 
} 

//----------------------------------------------------------------------------
#endif	
