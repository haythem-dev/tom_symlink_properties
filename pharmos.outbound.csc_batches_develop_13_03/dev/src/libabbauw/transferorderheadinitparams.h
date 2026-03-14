#ifndef GUARD_LIBABBAUW_TRANSFERORDER_TRANSFERORDERHEADINITPARAMS_H
#define GUARD_LIBABBAUW_TRANSFERORDER_TRANSFERORDERHEADINITPARAMS_H

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

    struct TransferOrderHeadInitParams
    {
	    basar::Int16     m_BranchNo;

	    TransferOrderHeadInitParams( const basar::Int16 branchNo ) 
	    : m_BranchNo( branchNo )
	    {
	    }

		TransferOrderHeadInitParams() 
	    : m_BranchNo( 0 )
	    {
	    }
    };

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_TRANSFERORDER_TRANSFERORDERHEADINITPARAMS_H
