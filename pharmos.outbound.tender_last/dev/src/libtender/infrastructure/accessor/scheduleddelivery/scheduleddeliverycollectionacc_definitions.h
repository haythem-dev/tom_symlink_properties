#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SCHEDULEDDELIVERYCOLLECTIONACC_DEFINITIONS_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SCHEDULEDDELIVERYCOLLECTIONACC_DEFINITIONS_H

#include <libbasarcmnutil_parameterlist.h>

namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace scheduledDelivery
            {
                namespace lit
                {
                    const basar::VarString SCHEDULEDDELIVERY_COLLECTION_ACC         ( "AccScheduledDeliveryCollection" );
                    const basar::VarString SELECT_SCHEDULEDDELIVERY_BY_PATTERN      ( "SelectScheduledDeliveryByPattern" );
					const basar::VarString IS_EXISTING								( "IsExisting" );
					const basar::VarString SELECT_QUANTITY_BY_PATTERN				( "SelectQuantityByPattern" );
                }
            }
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SCHEDULEDDELIVERYCOLLECTIONACC_DEFINITIONS_H
