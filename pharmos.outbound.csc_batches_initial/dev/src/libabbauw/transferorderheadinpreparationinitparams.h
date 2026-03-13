#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERINPREPARATIONINITPARAMS_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERINPREPARATIONINITPARAMS_H

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

    struct TransferOrderHeadInPreparationInitParams
    {
	    basar::Int16     m_BranchNo;
        basar::VarString m_UserName;

	    TransferOrderHeadInPreparationInitParams( const basar::Int16 branchNo, const basar::VarString& userName ) 
	    : m_BranchNo( branchNo ), m_UserName( userName )
	    {
	    }

		TransferOrderHeadInPreparationInitParams() 
	    : m_BranchNo( 0 )
	    {
	    }
    };

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERINPREPARATIONINITPARAMS_H
