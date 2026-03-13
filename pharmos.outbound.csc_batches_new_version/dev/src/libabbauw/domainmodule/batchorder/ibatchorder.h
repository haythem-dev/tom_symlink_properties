#ifndef GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDER_H
#define GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDER_H

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

    class IBatchOrder
    {
    public:
        virtual ~IBatchOrder() {}

        virtual basar::db::aspect::AccessorPropertyTable_YIterator   getHead()               = 0;
        virtual basar::db::aspect::AccessorPropertyTableRef          getPositionCollection() = 0;

        virtual bool                                                 isEmpty() const         = 0;

        virtual void                                                 resetSearchYit()        = 0;
        virtual basar::db::aspect::AccessorPropertyTable_YIterator   getSearchYit()          = 0;

        virtual void                                                 findByKey( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

        virtual void                                                 release()               = 0;
    };

} // end namespace batchOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_BATCHORDER_IBATCHORDER_H
