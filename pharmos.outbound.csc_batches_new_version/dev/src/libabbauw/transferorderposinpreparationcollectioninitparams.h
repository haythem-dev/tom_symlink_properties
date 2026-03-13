#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSINPREPARATIONINITPARAMS_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSINPREPARATIONINITPARAMS_H

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

    struct TransferOrderPosInPreparationCollectionInitParams
    {
	    basar::Int16     m_BranchNo;
        basar::VarString m_UserName;

	    TransferOrderPosInPreparationCollectionInitParams( const basar::Int16 branchNo, const basar::VarString& userName ) 
	    : m_BranchNo( branchNo ), m_UserName( userName )
	    {
	    }

		TransferOrderPosInPreparationCollectionInitParams() 
	    : m_BranchNo( 0 )
	    {
	    }
    };

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERPOSINPREPARATIONINITPARAMS_H
