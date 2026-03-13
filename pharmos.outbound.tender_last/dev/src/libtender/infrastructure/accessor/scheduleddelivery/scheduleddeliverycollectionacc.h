#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SCHEDULEDDELIVERYCOLLECTIONACC_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SCHEDULEDDELIVERYCOLLECTIONACC_H

namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace scheduledDelivery
            {
                BEGIN_QUERY_BUILDER_DECLARATION( SelectScheduledDeliveryByPattern )
                END_BUILDER_DECLARATION

				BEGIN_QUERY_BUILDER_DECLARATION( IsExisting )
                END_BUILDER_DECLARATION

				BEGIN_QUERY_BUILDER_DECLARATION( SelectQuantityByPattern )
                END_BUILDER_DECLARATION
            }
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SCHEDULEDDELIVERYCOLLECTIONACC_H
