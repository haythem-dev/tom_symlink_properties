#ifndef GUARD_DOMMOD_MANUFACTURERDM_MANUFACTURERDM_INITPARAMS_H
#define GUARD_DOMMOD_MANUFACTURERDM_MANUFACTURERDM_INITPARAMS_H

namespace libabbauw
{
namespace domMod
{
namespace manufacturerdm
{
    struct ManufacturerInitParams
    {
	    basar::Int16 m_BranchNo;

        ManufacturerInitParams()
			: m_BranchNo( 0 )
        {}

		ManufacturerInitParams( const basar::Int16 branchNo ) 
	    : m_BranchNo( branchNo )
	    {
	    }
	    
    };

} // end namespace 
} // end namespace 
}
#endif //GUARD_DOMMOD_MANUFACTURERDM_MANUFACTURERDM_INITPARAMS_H
