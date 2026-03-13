#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYPOSITIONCOLLECTIONPTR_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYPOSITIONCOLLECTIONPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
	namespace domMod
	{
		namespace scheduledDelivery
		{
			class IScheduledDeliveryPositionCollection;
			typedef boost::shared_ptr< IScheduledDeliveryPositionCollection > IScheduledDeliveryPositionCollectionPtr;
		}
	}
}

#endif //GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYPOSITIONCOLLECTIONPTR_H
