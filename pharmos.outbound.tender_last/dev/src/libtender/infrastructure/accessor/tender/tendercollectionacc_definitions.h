#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERCOLLECTIONACC_DEFINITIONS_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERCOLLECTIONACC_DEFINITIONS_H

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
            namespace tender
            {
                namespace lit
                {
                    const basar::VarString TENDER_COLLECTION_ACC	( "AccTenderCollection"	  );
                    const basar::VarString SELECT_TENDER_BY_PATTERN	( "SelectTenderByPattern" );
                    const basar::VarString SELECT_SUCCESSOR_TENDER	( "SelectSuccessorTender" );
                    const basar::VarString SELECT_TENDER_BY_ENDDATE	( "SelectTenderByEndDate" );
                    const basar::VarString SELECT_EXPIRED_TENDERS	( "SelectExpiredTenders"  );
					const basar::VarString SELECT_VALID_TENDERS		( "SelectValidTenders"	  );
					
                    static const basar::VarString BRANCH_NO_TWELVE = "12";
                    static const basar::VarString BRANCH_NO_THIRTEEN = "13";
                    static const basar::VarString BRANCH_NO_SIXTEEN = "16";
                }
            }
        }
    }
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERCOLLECTIONACC_DEFINITIONS_H
