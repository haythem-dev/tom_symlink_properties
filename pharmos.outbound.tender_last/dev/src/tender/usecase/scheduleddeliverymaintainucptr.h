#ifndef GUARD_TENDER_USECASE_SCHEDULEDDELIVERYMAINTAINUCPTR_H
#define GUARD_TENDER_USECASE_SCHEDULEDDELIVERYMAINTAINUCPTR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------------------------------------------//
// forward declaration section
//--------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace useCase
    {
        class ScheduledDeliveryMaintainUC;
        typedef boost::shared_ptr< ScheduledDeliveryMaintainUC > ScheduledDeliveryMaintainUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_SCHEDULEDDELIVERYMAINTAINUCPTR_H
