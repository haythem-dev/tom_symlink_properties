#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERINPREPARATIONINITPARAMS_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERINPREPARATIONINITPARAMS_H

#include <libbasarcmnutil_i18nstring.h>
#include <libbasar_definitions.h>

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

    struct TransferOrderInPreparationInitParams
    {
	    basar::Int16     m_BranchNo;
        basar::VarString m_UserName;

	    TransferOrderInPreparationInitParams( const basar::Int16 branchNo, const basar::VarString& userName ) 
	    : m_BranchNo( branchNo ), m_UserName( userName )
	    {
	    }

		TransferOrderInPreparationInitParams() 
	    : m_BranchNo( 0 )
	    {
	    }
    };

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERINPREPARATIONINITPARAMS_H
