#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSAL_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSAL_H

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
    class IOrderProposal
    {
        public:
			virtual ~IOrderProposal() {}

            virtual basar::db::aspect::AccessorPropertyTable_YIterator get() const                                                           = 0;

            virtual void                                               resetSearchYit()                                                      = 0;
			virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit()                                                        = 0;

			virtual void                                               findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
            virtual bool                                               isEmpty() const                                                       = 0;

            virtual void                                               setOrderNo( basar::Int32 )                                            = 0;

            virtual void                                               save()                                                                = 0;
            virtual void                                               erase()                                                               = 0;
    };

} // end namespace orderProposal
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROPOSAL_IORDERPROPOSAL_H
