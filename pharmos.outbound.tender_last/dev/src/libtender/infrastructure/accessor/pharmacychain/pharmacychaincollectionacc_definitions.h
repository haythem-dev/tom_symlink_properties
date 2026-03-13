#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PHARMACYCHAINCOLLECTIONACC_DEFINITIONS_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PHARMACYCHAINCOLLECTIONACC_DEFINITIONS_H

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
            namespace pharmacychain
            {
                namespace lit
                {
                    const basar::VarString PHARMACYCHAIN_COLLECTION_ACC			  ( "AccPharmacyChainCollection"      );
                    const basar::VarString SELECT_PHARMACYCHAIN_BY_PATTERN		  ( "SelectPharmacyChainByPattern"    );
					const basar::VarString SELECT_PHARMACYCHAIN_BY_PHARMACYCHAINNO( "SelectPharmacyByPharmacyChainNo" );
					const basar::VarString SELECT_CUSTOMERGROUPID_BY_PHARMACYCHAIN( "SelectCustomerGroupIDByPharmacychain" );
				}
            }
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PHARMACYCHAINCOLLECTIONACC_DEFINITIONS_H
