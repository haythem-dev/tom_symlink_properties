//----------------------------------------------------------------------------//
/*! \file	
 *  \brief  
 *  \author Hendrik Polzin 
 *  \date 15.08.2013 
 */
//----------------------------------------------------------------------------//
#ifndef GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_I_SET_PU_BATCH_ORDER_POS_H
#define GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_I_SET_PU_BATCH_ORDER_POS_H

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
    class IPuBatchOrderPositionWriter
    {
        public:	
            virtual ~IPuBatchOrderPositionWriter(){};
			virtual void savePuBatchOrderPositions( basar::db::aspect::AccessorPropertyTable_YIterator it )=0;

			virtual void beginTransaction()=0;
			virtual void commitTransaction()=0;
			virtual void rollbackTransaction()=0;
			virtual bool isInTransaction() const=0;
	};

} // end namespace 
} // end namespace 

//----------------------------------------------------------------------------
#endif	
