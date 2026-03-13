#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PARTNERCOLLECTIONACC_DEFINITIONS_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PARTNERCOLLECTIONACC_DEFINITIONS_H

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
            namespace partner
            {
                namespace lit
                {
                    const basar::VarString PARTNER_COLLECTION_ACC	   ( "AccPartnerCollection"      );
                    const basar::VarString SELECT_PARTNER_BY_PATTERN   ( "SelectPartnerByPattern"    );
					const basar::VarString SELECT_PARTNER_BY_PARTNER_NO( "SelectPartnerByPartnerNo"  );
                }
            }
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_PARTNERCOLLECTIONACC_DEFINITIONS_H
