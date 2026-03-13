#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERPROGRESSACC_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERPROGRESSACC_H

namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace tenderprogress
            {

                BEGIN_QUERY_BUILDER_DECLARATION( SelectArticleQuantity )
                END_BUILDER_DECLARATION

                BEGIN_QUERY_BUILDER_DECLARATION( SelectTenderProgressByPattern )
                END_BUILDER_DECLARATION

				BEGIN_QUERY_BUILDER_DECLARATION( SelectTenderProgressByTender )
                END_BUILDER_DECLARATION

                BEGIN_QUERY_BUILDER_DECLARATION( SelectTenderProgressForOMGCalculation )
                END_BUILDER_DECLARATION

                BEGIN_WRITE_BUILDER_DECLARATION( InsertTenderProgress)
                END_BUILDER_DECLARATION

                BEGIN_WRITE_BUILDER_DECLARATION( UpdateTenderProgress )
                END_BUILDER_DECLARATION

                BEGIN_WRITE_BUILDER_DECLARATION( DeleteTenderProgress )
                END_BUILDER_DECLARATION

            }
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERPROGRESSACC_H
