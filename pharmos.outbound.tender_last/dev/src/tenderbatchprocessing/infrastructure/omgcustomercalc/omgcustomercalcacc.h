#ifndef GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGCUSTOMERCALCACC_H
#define GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGCUSTOMERCALCACC_H

#include <libutil/accessor.h>

namespace tenderbatchprocessing
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace omgcustomercalc
            {

                BEGIN_QUERY_BUILDER_DECLARATION( SelectOMGCustomerCalcById )
                END_BUILDER_DECLARATION

                BEGIN_QUERY_BUILDER_DECLARATION( SelectOMGCustomerCalcByPattern )
                END_BUILDER_DECLARATION

                BEGIN_WRITE_BUILDER_DECLARATION( InsertOMGCustomerCalc)
                END_BUILDER_DECLARATION

                BEGIN_WRITE_BUILDER_DECLARATION( UpdateOMGCustomerCalc )
                END_BUILDER_DECLARATION


            }
        }
    }
}
#endif //end GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGCUSTOMERCALCACC_H




