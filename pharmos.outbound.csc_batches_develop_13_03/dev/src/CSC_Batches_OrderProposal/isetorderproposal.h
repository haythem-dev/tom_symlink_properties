#ifndef GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_I_SET_ORDER_PROPOSAL_H
#define GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_I_SET_ORDER_PROPOSAL_H

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

namespace domMod {
namespace OrderProposalTransfer {

class IOrderProposalWriter
{
    public:	
        virtual ~IOrderProposalWriter(){};
        virtual void updateOrderProposals( basar::db::aspect::AccessorPropertyTableRef ref ) = 0;
		virtual void updateTransferTime( basar::db::aspect::AccessorPropertyTable_YIterator ref ) = 0;
        virtual void updateOrderProposalCompletedFlag() = 0;
		
        virtual void beginTransaction()=0;
		virtual void commitTransaction()=0;
		virtual void rollbackTransaction()=0;
		virtual bool isInTransaction() const =0;
};

} 
}

#endif
