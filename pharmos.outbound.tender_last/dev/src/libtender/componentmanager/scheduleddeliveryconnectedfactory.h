#ifndef GUARD_LIBTENDER_COMPONENTMANAGER_SCHEDULEDDELIVERYCONNECTEDFACTORY_H
#define GUARD_LIBTENDER_COMPONENTMANAGER_SCHEDULEDDELIVERYCONNECTEDFACTORY_H

#include "scheduleddeliveryfactorybase.h"
#include "ischeduleddeliveryconnectedfactory.h"


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

namespace domMod
{
    namespace scheduledDelivery
    {
        class IScheduledDeliveryPtr;
    }
}

namespace libtender
{
    namespace componentManager
    {
        class ScheduledDeliveryConnectedFactory : public /*protected*/ ScheduledDeliveryFactoryBase,
                                       public IScheduledDeliveryConnectedFactory
        {
            public:
                ScheduledDeliveryConnectedFactory();
                virtual ~ScheduledDeliveryConnectedFactory();

				virtual domMod::scheduledDelivery::IScheduledDeliveryPtr create( basar::db::aspect::AccessorPropertyTable_YIterator ) const;

            private:
                ScheduledDeliveryConnectedFactory( const ScheduledDeliveryConnectedFactory& );
                ScheduledDeliveryConnectedFactory& operator=( const ScheduledDeliveryConnectedFactory& );
        };
    }
}

#endif //end GUARD_LIBTENDER_COMPONENTMANAGER_SCHEDULEDDELIVERYCONNECTEDFACTORY_H
