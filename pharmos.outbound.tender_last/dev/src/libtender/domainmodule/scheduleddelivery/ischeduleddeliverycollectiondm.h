#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYCOLLECTIONDM_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYCOLLECTIONDM_H

#include "ischeduleddeliveryptr.h"

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

namespace libtender
{
    namespace domMod
    {
        namespace scheduledDelivery
        {

            class IScheduledDeliveryCollectionDM
            {
                public:
                    virtual ~IScheduledDeliveryCollectionDM(){};
					
					virtual void findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

                    virtual basar::db::aspect::AccessorPropertyTableRef         get() const          = 0;
                    virtual void                                                resetSearchYit()     = 0;
                    virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSearchYit() const = 0;
                    virtual IScheduledDeliveryPtr                               getScheduledDelivery( const basar::db::aspect::AccessorPropertyTable_YIterator ) const = 0;
                    virtual bool                                                isEmpty() const = 0;
					virtual bool												isExisting( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
					virtual void												getQuantity( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
            };

        } // end namespace scheduledDelivery
    }     // end namespace domMod
}         // end namespace libtender

#endif //GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYCOLLECTIONDM_H
