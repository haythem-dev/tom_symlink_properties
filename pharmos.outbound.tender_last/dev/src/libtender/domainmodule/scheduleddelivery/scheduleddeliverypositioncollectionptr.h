#ifndef GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYPOSITIONCOLLECTION_PTR_H
#define GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYPOSITIONCOLLECTION_PTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace scheduledDelivery
{
	class ScheduledDeliveryPositionCollection;
	typedef boost::shared_ptr< ScheduledDeliveryPositionCollection > ScheduledDeliveryPositionCollectionPtr;
}
}
}

#endif // GUARD_LIBTENDER_DOMMOD_SCHEDULEDDELIVERY_ISCHEDULEDDELIVERYPOSITIONCOLLECTION_PTR_H
