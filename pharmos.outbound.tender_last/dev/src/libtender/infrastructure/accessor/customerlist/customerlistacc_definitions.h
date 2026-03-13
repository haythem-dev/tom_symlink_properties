#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_CUSTOMERLISTACC_DEFINITIONS_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_CUSTOMERLISTACC_DEFINITIONS_H

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
            namespace customerlist
            {
                namespace lit
                {
                    const basar::VarString CUSTOMER_LIST_ACC		   ( "AccCustomerList"			);

                    const basar::VarString SELECT_CUSTOMER_BY_PATTERN  ( "SelectCustomerByPattern"  );
                    const basar::VarString SELECT_CUSTOMER_BY_TENDER_NO( "SelectCustomerByTenderNo"	);

					const basar::VarString SAVE_CUSTOMER_LIST		   ( "SaveCustomerList"			);
                }
            }
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_CUSTOMERLISTACC_DEFINITIONS_H
