#ifndef GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_I_GET_ORDER_PROPOSAL_H
#define GUARD_DOMMOD_ORDER_PROPOSAL_TRANSFER_I_GET_ORDER_PROPOSAL_H

namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTableRef;
			class AccessorPropertyTable_YIterator;
        }
    }
}

namespace domMod {
namespace OrderProposalTransfer {

class IOrderProposalFinder
{
    public:	
        virtual ~IOrderProposalFinder(){};
        virtual basar::db::aspect::AccessorPropertyTableRef getTable() const = 0;
		virtual basar::db::aspect::AccessorPropertyTableRef findOrderProposals() = 0;
};

} 
} 

#endif	
