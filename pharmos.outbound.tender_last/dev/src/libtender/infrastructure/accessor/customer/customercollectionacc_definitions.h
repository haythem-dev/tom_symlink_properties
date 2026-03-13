#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_CUSTOMERCOLLECTIONACC_DEFINITIONS_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_CUSTOMERCOLLECTIONACC_DEFINITIONS_H

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
            namespace customer
            {
                namespace lit
                {
                    const basar::VarString CUSTOMER_COLLECTION_ACC       ( "AccTenderCustomerCollection" );
                    const basar::VarString SELECT_CUSTOMER_BY_PATTERN    ( "SelectCustomerByPattern"  );
                    const basar::VarString SELECT_CUSTOMER_BY_CUSTOMER_NO( "SelectCustomerByCustomerNo"  );
                    const basar::VarString SELECT_CUSTOMER_BY_PHARMACY_GROUP_ID( "SelectCustomerByPharmacyGroupId" );
                }
            }
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_CUSTOMERCOLLECTIONACC_DEFINITIONS_H
