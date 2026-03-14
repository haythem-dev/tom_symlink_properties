#ifndef GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERHEADFINDER_H
#define GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERHEADFINDER_H

#include "iordergetter.h"

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

    class IBatchOrderHeadFinder : public cscorder::IOrderGetter
    {
    public:
        virtual ~IBatchOrderHeadFinder() {}

        virtual basar::db::aspect::AccessorPropertyTable_YIterator   get()     const      = 0;
        virtual bool                                                 isEmpty() const      = 0;

        virtual void                                                 resetSearchYit()     = 0;
        virtual basar::db::aspect::AccessorPropertyTable_YIterator   getSearchYit()       = 0;

        virtual void                                                 findByKey( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
    };

} // end namespace batchOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDERHEADFINDER_H
