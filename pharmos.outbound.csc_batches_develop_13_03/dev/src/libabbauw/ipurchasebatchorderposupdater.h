#ifndef GUARD_LIBABBAUW_DOMMOD_PURCHASEBATCHORDER_IPURCHASEBATCHORDERPOSUPDATER_H
#define GUARD_LIBABBAUW_DOMMOD_PURCHASEBATCHORDER_IPURCHASEBATCHORDERPOSUPDATER_H

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
namespace purchaseBatchOrder
{
    class IPurchaseBatchOrderPosUpdater
    {
        public:	
			virtual ~IPurchaseBatchOrderPosUpdater() {}

            virtual void                                               resetSearchYit()                                                      = 0;
			virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYit()                                                        = 0;

			virtual void                                               findByKey( const basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
            virtual bool                                               isEmpty() const                                                       = 0;

            virtual void                                               setOrderNo( basar::Int32 )                                            = 0;

            virtual void                                               save()                                                                = 0;
            virtual void                                               erase()                                                               = 0;
    };

} // end namespace purchaseBatchOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_PURCHASEBATCHORDER_IPURCHASEBATCHORDERPOSUPDATER_H
