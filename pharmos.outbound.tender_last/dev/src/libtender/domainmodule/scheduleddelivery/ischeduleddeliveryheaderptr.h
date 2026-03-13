#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYHEADERPTR_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYHEADERPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
	namespace domMod
	{
		namespace scheduledDelivery
		{
			class IScheduledDeliveryHeader;
			typedef boost::shared_ptr< IScheduledDeliveryHeader > IScheduledDeliveryHeaderPtr;
		}
	}
}

#endif // GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYHEADERPTR_H
