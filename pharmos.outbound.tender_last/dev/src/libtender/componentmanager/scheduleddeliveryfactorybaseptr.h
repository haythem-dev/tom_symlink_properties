#ifndef GUARD_LIBTENDER_COMPONENTMANAGER_SCHEDULEDDELIVERYFACTORYBASEPTR_H
#define GUARD_LIBTENDER_COMPONENTMANAGER_SCHEDULEDDELIVERYFACTORYBASEPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
    namespace componentManager
    {
        class ScheduledDeliveryFactoryBase;
        typedef boost::shared_ptr< ScheduledDeliveryFactoryBase > ScheduledDeliveryFactoryBasePtr;
    }
}

#endif //end GUARD_LIBTENDER_COMPONENTMANAGER_SCHEDULEDDELIVERYFACTORYBASEPTR_H
