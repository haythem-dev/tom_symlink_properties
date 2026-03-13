#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PHARMACYGROUPCOLLECTIONACC_DEFINITIONS_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PHARMACYGROUPCOLLECTIONACC_DEFINITIONS_H

//----------------------------------------------------------------------------//
// include section
//----------------------------------------------------------------------------//
#include <libbasarcmnutil_parameterlist.h>

//----------------------------------------------------------------------------//
// lit definition section
//----------------------------------------------------------------------------//
namespace libtender
{
    namespace infrastructure
    {
        namespace accessor
        {
            namespace pharmacygroup
            {
                namespace lit
                {
                    const basar::VarString PHARMACY_GROUP_COLLECTION_ACC                            ( "AccPharmacyGroup" );
                    const basar::VarString SELECT_PHARMACY_GROUP_BY_PATTERN                         ( "SelectPharmacyGroupByPattern" );
                    const basar::VarString SELECT_PHARMACY_GROUP_CUSTOMERS_ASSIGNMENT_BY_PATTERN    ( "SelectPharmacyGroupCustomerAssignmentByPattern" );
                    const basar::VarString SELECT_PHARMACY_GROUP_BY_GROUP_ID                        ( "SelectPharmacyGroupByGroupID" );
                    const basar::VarString SELECT_PHARMACY_GROUP_BY_PSEUDOCUSTOMER_NO               ( "SelectPharmacyGroupByPseudoCustomerNo" );
                }
            }
        }
    }
}

#endif // end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PHARMACYGROUPCOLLECTIONACC_DEFINITIONS_H
