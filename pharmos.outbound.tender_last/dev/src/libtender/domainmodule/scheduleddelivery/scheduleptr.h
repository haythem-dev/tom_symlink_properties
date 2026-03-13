#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEPTR_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
	namespace domMod
	{
		namespace scheduledDelivery
		{
			class Schedule;
			typedef boost::shared_ptr< Schedule > SchedulePtr;
		}
	}
}

#endif // GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_SCHEDULEPTR_H
