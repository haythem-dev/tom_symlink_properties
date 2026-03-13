#ifndef GUARD_LIBABBAUW_PARAMETER_INIT_PARAMS_H
#define GUARD_LIBABBAUW_PARAMETER_INIT_PARAMS_H

namespace libabbauw
{
namespace domMod
{
namespace parameter
{

    struct ParameterInitParams
    {
	    basar::Int16     m_BranchNo;
		basar::VarString m_ProgName;
		basar::VarString m_DefaultPurpose;

		ParameterInitParams( const basar::Int16 branchNo, const basar::VarString& progName, const basar::VarString& defaultPurpose ) 
			: m_BranchNo( branchNo ), m_ProgName( progName ), m_DefaultPurpose( defaultPurpose )
	    {
	    }

	    ParameterInitParams()
	    : m_BranchNo( 0 )
	    {
	    }
    };

} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_PARAMETER_INIT_PARAMS_H
