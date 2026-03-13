#ifndef GUARD_LIBTENDER_COMPONENTMANAGER_ISCHEDULEDDELIVERYCONNECTEDFACTORY_H
#define GUARD_LIBTENDER_COMPONENTMANAGER_ISCHEDULEDDELIVERYCONNECTEDFACTORY_H

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
        class IScheduledDeliveryConnectedFactory
        {
            public:
                virtual ~IScheduledDeliveryConnectedFactory(){};
				virtual domMod::scheduledDelivery::IScheduledDeliveryPtr create( basar::db::aspect::AccessorPropertyTable_YIterator ) const = 0;
        };
    }
}

#endif //end GUARD_LIBTENDER_COMPONENTMANAGER_ISCHEDULEDDELIVERYCONNECTEDFACTORY_H
