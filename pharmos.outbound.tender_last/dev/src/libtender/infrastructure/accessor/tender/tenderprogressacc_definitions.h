#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERPROGRESSACC_DEFINITIONS_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERPROGRESSACC_DEFINITIONS_H

#include <libbasarcmnutil_parameterlist.h>

namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace tenderprogress
            {
                namespace lit
                {
                    const basar::VarString TENDERPROGRESS_ACC			( "AccTenderProgress" );
                    const basar::VarString SELECT_TENDERPROGRESS_BY_ARTICLEQUANTITY ( "SelectArticleQuantity" );
                    const basar::VarString SELECT_TENDERPROGRESS_BY_PATTERN ( "SelectTenderProgressByPattern"  );
                    const basar::VarString SELECT_TENDERPROGRESS_BY_TENDER( "SelectTenderProgressByTender" );
                    const basar::VarString SELECT_TENDERPROGRESS_FOR_OMGCALCULATION( "SelectTenderProgressForOMGCalculation" );
                    const basar::VarString SAVE_TENDER_PROGRESS( "SaveTenderProgress" );
                }
            }
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERPROGRESSACC_DEFINITIONS_H
