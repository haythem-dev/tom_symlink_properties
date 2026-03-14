#ifndef GUARD_LIBABBAUW_TRANSFERORDERHEADCOLLECTIONINITPARAMS_H
#define GUARD_LIBABBAUW_TRANSFERORDERHEADCOLLECTIONINITPARAMS_H

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

    struct TransferOrderHeadCollectionInitParams
    {
	    basar::Int16 m_BranchNo;

	    TransferOrderHeadCollectionInitParams( const basar::Int16 branchNo ) 
	    : m_BranchNo( branchNo )
	    {
	    }

		TransferOrderHeadCollectionInitParams() 
	    : m_BranchNo( 0 )
	    {
	    }
    };

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_TRANSFERORDERHEADCOLLECTIONINITPARAMS_H
