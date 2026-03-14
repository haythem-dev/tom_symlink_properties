#ifndef GUARD_LIBABBAUW_DOMMOD_CUSTOMER_CUSTOMERINITPARAMS_H
#define GUARD_LIBABBAUW_DOMMOD_CUSTOMER_CUSTOMERINITPARAMS_H

namespace libabbauw
{
namespace domMod
{
namespace customer
{
    struct CustomerInitParams
    {
	    basar::Int16 m_BranchNo;

        CustomerInitParams()
			: m_BranchNo( 0 )
        {}

		CustomerInitParams( const basar::Int16 branchNo ) 
	    : m_BranchNo( branchNo )
	    {
	    }
    };

} // end namespace customer
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_CUSTOMER_CUSTOMERINITPARAMS_H
