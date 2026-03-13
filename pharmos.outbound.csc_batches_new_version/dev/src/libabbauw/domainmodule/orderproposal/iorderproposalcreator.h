#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCREATOR_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCREATOR_H

#include "domainmodule/orderproposal/iorderproposalptr.h"
#include "itransferordergetterptr.h"

#include <libbasar_definitions.h>

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
// interface declaration section
//----------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace orderProposal
{
    class IOrderProposalCreator
    {
        public:
			virtual ~IOrderProposalCreator() {}

            virtual void createOrderProposal ( transferOrder::ITransferOrderGetterPtr, const basar::Int32 posNo ) const = 0;
            virtual void createOrderProposals( transferOrder::ITransferOrderGetterPtr ) const                           = 0;
    };

} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCREATOR_H
