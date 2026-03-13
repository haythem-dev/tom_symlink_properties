#ifndef GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGCALCACC_DEFINITIONS_H
#define GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGCALCACC_DEFINITIONS_H

#include <libbasarcmnutil_parameterlist.h>

namespace tenderbatchprocessing
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace omgcalc
            {
                namespace lit
                {
                    const basar::VarString OMGCALC_ACC                  ( "AccOMGCalc" );
					
                    const basar::VarString SELECT_OMG_CALC_FOR_TRANSFER( "SelectOMGCalcForTransfer" );
                    const basar::VarString SELECT_OMG_CALC_BY_ID        ( "SelectOMGCalcById" );
                    const basar::VarString SELECT_OMG_CALC_BY_PATTERN   ( "SelectOMGCalcByPattern" );
                    const basar::VarString SAVE_OMG_CALC                ( "SaveOMGCalc" );
                } // end namespace lit
            } // end namespace omgcalc
        } // end namespace accessor
    } // end namespace infrastructure
}
#endif //end GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGCALCACC_DEFINITIONS_H