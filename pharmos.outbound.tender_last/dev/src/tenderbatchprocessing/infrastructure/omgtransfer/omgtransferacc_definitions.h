#ifndef GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGTRANSFER_DEFINITIONS_H
#define GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGTRANSFER_DEFINITIONS_H

#include <libbasarcmnutil_parameterlist.h>

namespace tenderbatchprocessing
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace omgtransfer
            {
                namespace lit
                {
                    const basar::VarString OMGTRANSFER_ACC                  ( "AccOMGTransfer" );
                    const basar::VarString SELECT_OMG_TRANSFER_BY_ID        ( "SelectOMGTransferById" );
                    const basar::VarString SELECT_OMG_TRANSFER_BY_PATTERN   ( "SelectOMGTransferByPattern" );
                    const basar::VarString SAVE_OMG_TRANSFER                ( "SaveOMGTransfer" );
                } // end namespace lit
            } // end namespace omgtransfer
        } // end namespace accessor
    } // end namespace infrastructure
}
#endif //end GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGTRANSFERACC_DEFINITIONS_H