#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEPTR_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
	namespace domMod
	{
		namespace scheduledDelivery
		{
			class ISchedule;
			typedef boost::shared_ptr< ISchedule > ISchedulePtr;
		}
	}
}

#endif //GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEPTR_H
