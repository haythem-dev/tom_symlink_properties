#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace scheduledDelivery
{
	class ScheduledDeliveryDM;
	typedef boost::shared_ptr< ScheduledDeliveryDM > ScheduledDeliveryDMPtr;
}
}
}

#endif //end GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEDDELIVERYDMPTR_H
