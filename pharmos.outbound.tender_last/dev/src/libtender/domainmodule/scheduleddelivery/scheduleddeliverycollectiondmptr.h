#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYCOLLECTIONDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYCOLLECTIONDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
    namespace domMod
    {
        namespace scheduledDelivery
        {
            class ScheduledDeliveryCollectionDM;
            typedef boost::shared_ptr< ScheduledDeliveryCollectionDM > ScheduledDeliveryCollectionDMPtr;
        }
    }
}

#endif //end GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYCOLLECTIONDMPTR_H
