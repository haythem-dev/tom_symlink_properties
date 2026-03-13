#ifndef GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGTRANSFER_ACC_H
#define GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGTRANSFER_ACC_H

#include <libutil/accessor.h>

namespace tenderbatchprocessing
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace omgtransfer
            {
                BEGIN_QUERY_BUILDER_DECLARATION( SelectOMGTransferById )
                END_BUILDER_DECLARATION

                BEGIN_QUERY_BUILDER_DECLARATION( SelectOMGTransferByPattern )
                END_BUILDER_DECLARATION

                BEGIN_WRITE_BUILDER_DECLARATION( InsertOMGTransfer)
                END_BUILDER_DECLARATION

                BEGIN_WRITE_BUILDER_DECLARATION( UpdateOMGTransfer )
                END_BUILDER_DECLARATION

            }
        }
    }
}
#endif //end GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGTRANSFER_ACC_H




