#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYPTR_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
	namespace domMod
	{
		namespace scheduledDelivery
		{
			class IScheduledDelivery;
			typedef boost::shared_ptr< IScheduledDelivery > IScheduledDeliveryPtr;
		}
	}
}

#endif //end GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYPTR_H
