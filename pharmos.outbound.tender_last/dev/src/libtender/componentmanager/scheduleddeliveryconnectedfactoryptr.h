#ifndef GUARD_LIBTENDER_COMPONENTMANAGER_SCHEDULEDDELIVERYCONNECTEDFACTORYPTR_H
#define GUARD_LIBTENDER_COMPONENTMANAGER_SCHEDULEDDELIVERYCONNECTEDFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
    namespace componentManager
    {
        class ScheduledDeliveryConnectedFactory;
        typedef boost::shared_ptr< ScheduledDeliveryConnectedFactory > ScheduledDeliveryConnectedFactoryPtr;
    }
}

#endif //end GUARD_LIBTENDER_COMPONENTMANAGER_SCHEDULEDDELIVERYCONNECTEDFACTORYPTR_H
