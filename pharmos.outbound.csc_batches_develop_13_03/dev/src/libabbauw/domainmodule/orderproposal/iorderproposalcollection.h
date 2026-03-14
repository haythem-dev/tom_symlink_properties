#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTION_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTION_H

#include "iorderproposalcollectionfinder.h"

//----------------------------------------------------------------------------//
// interface declaration section
//----------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace orderProposal
{
    class IOrderProposalCollection : public IOrderProposalCollectionFinder
    {
        public:
            virtual ~IOrderProposalCollection() {}

            virtual basar::db::aspect::AccessorPropertyTableRef        get()        const                                               = 0;
            virtual basar::db::aspect::AccessorPropertyTable_YIterator addNewItem()                                                     = 0;
            virtual void                                               removeItem( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
            virtual void                                               save()                                                           = 0;
    };

} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSALCOLLECTION_H
