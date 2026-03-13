#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTIONPROTECTIONPROXY_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTIONPROTECTIONPROXY_H

#include "iorderproposalcollection.h"

#include <ilockguardptr.h>
#include <libbasar_definitions.h>

//----------------------------------------------------------------------------//
// interface declaration section
//----------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace orderProposal
{
    class IOrderProposalCollectionProtectionProxy : public IOrderProposalCollection
    {
        public:
            virtual ~IOrderProposalCollectionProtectionProxy() {}

            virtual void                                               passLockGuard( basar::Int32 orderNo, libLockManager::ILockGuardPtr ) = 0;
    };

} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTIONPROTECTIONPROXY_H
