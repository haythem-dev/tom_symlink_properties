#ifndef GUARD_LIBABBAUW_DOMMOD_CUSTOMERDM_CUSTOMERCOLLECTIONINITPARAMS_H
#define GUARD_LIBABBAUW_DOMMOD_CUSTOMERDM_CUSTOMERCOLLECTIONINITPARAMS_H

namespace libabbauw
{
namespace domMod
{
namespace customer
{
    struct CustomerCollectionInitParams
    {
	    basar::Int16 m_BranchNo;

        CustomerCollectionInitParams()
			: m_BranchNo( 0 )
        {}

		CustomerCollectionInitParams( const basar::Int16 branchNo ) 
	    : m_BranchNo( branchNo )
	    {
	    }
	    
    };

} // end namespace customer
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_CUSTOMERDM_CUSTOMERCOLLECTIONINITPARAMS_H
