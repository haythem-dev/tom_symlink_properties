#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SUPPLIERCOLLECTIONACC_DEFINITIONS_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SUPPLIERCOLLECTIONACC_DEFINITIONS_H

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
            namespace supplier
            {
                namespace lit
                {
                    const basar::VarString SUPPLIER_COLLECTION_ACC     ( "AccSupplierCollection"      );
                    const basar::VarString SELECT_SUPPLIER_BY_PATTERN   ( "SelectSupplierByPattern"    );
                    const basar::VarString SELECT_SUPPLIER_BY_SUPPLIER_NO( "SelectSupplierBySupplierNo"  );
                }
            }
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SUPPLIERCOLLECTIONACC_DEFINITIONS_H
