#ifndef GUARD_TENDER_VIEWCONN_ISCHEDULEDDELIVERYMAINTAINVCPTR_H
#define GUARD_TENDER_VIEWCONN_ISCHEDULEDDELIVERYMAINTAINVCPTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
    namespace viewConn
    {
        class IScheduledDeliveryMaintain;
        typedef boost::shared_ptr< IScheduledDeliveryMaintain > IScheduledDeliveryMaintainVCPtr;
    }
}

#endif //end GUARD_TENDER_VIEWCONN_ISCHEDULEDDELIVERYMAINTAINVCPTR_H
