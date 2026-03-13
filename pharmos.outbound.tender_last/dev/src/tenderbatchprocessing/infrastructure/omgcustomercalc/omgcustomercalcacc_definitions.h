#ifndef GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGCUSTOMERCALCACC_DEFINITIONS_H
#define GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGCUSTOMERCALCACC_DEFINITIONS_H

#include <libbasarcmnutil_parameterlist.h>

namespace tenderbatchprocessing
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace omgcustomercalc
            {
                namespace lit
                {
                    const basar::VarString OMGCUSTOMERCALC_ACC                  ( "AccOMGCustomerCalc" );
                    const basar::VarString SELECT_OMG_CUSTOMER_CALC_BY_ID        ( "SelectOMGCustomerCalcById" );
                    const basar::VarString SELECT_OMG_CUSTOMER_CALC_BY_PATTERN   ( "SelectOMGCustomerCalcByPattern" );
                    const basar::VarString SAVE_OMG_CUSTOMER_CALC                ( "SaveOMGCustomerCalc" );
                } // end namespace lit
            } // end namespace omgcalc
        } // end namespace accessor
    } // end namespace infrastructure
}
#endif //end GUARD_TENDERBATCHPROCESSING_INFRASTRUCTURE_ACCESSOR_OMGCUSTOMERCALCACC_DEFINITIONS_H