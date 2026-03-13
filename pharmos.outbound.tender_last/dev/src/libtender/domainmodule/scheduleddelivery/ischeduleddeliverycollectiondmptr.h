#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYCOLLECTIONDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYCOLLECTIONDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
    namespace domMod
    {
        namespace scheduledDelivery
        {
            class IScheduledDeliveryCollectionDM;
            typedef boost::shared_ptr< IScheduledDeliveryCollectionDM > IScheduledDeliveryCollectionDMPtr;
        }
    }
}

#endif //end GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYCOLLECTIONDMPTR_H
