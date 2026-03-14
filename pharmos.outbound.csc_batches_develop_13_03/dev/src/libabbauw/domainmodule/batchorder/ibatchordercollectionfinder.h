#ifndef GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERCOLLECTIONFINDER_H
#define GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERCOLLECTIONFINDER_H

#include "ibatchorderptr.h"
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
            class AccessorPropertyTableRef;
        }
    }
}

namespace libabbauw
{
namespace domMod
{
namespace batchOrder
{

    class IBatchOrderCollectionFinder
    {
    public:
        virtual ~IBatchOrderCollectionFinder() {}

        virtual basar::db::aspect::AccessorPropertyTableRef          get()     const        = 0;
        virtual bool                                                 isEmpty() const        = 0;

        virtual void                                                 resetSearchYit()       = 0;
        virtual basar::db::aspect::AccessorPropertyTable_YIterator   getSearchYit()         = 0;

        virtual void                                                 findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
        virtual IBatchOrderPtr                                       getOrder( basar::db::aspect::AccessorPropertyTable_YIterator )      = 0;
    };

} // end namespace batchOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERCOLLECTIONFINDER_H
