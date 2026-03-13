#ifndef GUARD_LIBTENDER_COMPONENTMANAGER_ISCHEDULEDDELIVERYCONNECTEDFACTORYPTR_H
#define GUARD_LIBTENDER_COMPONENTMANAGER_ISCHEDULEDDELIVERYCONNECTEDFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
    namespace componentManager
    {
        class IScheduledDeliveryConnectedFactory;
        typedef boost::shared_ptr< IScheduledDeliveryConnectedFactory > IScheduledDeliveryConnectedFactoryPtr;
    }
}

#endif //end GUARD_LIBTENDER_COMPONENTMANAGER_ISCHEDULEDDELIVERYCONNECTEDFACTORYPTR_H
