#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_PARAMETERDMPTR_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_PARAMETERDMPTR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------------------------------------------//
// forward declaration section
//--------------------------------------------------------------------------------------------------//

namespace libabbauw
{
namespace domMod
{
namespace parameter
{
	class ParameterDM;
    typedef boost::shared_ptr< ParameterDM > ParameterDMPtr;
	
} // end namespace manufacturerdm
} // end namespace domMod
} // end namespace libabbauw

#endif //GUARD_LIBABBAUW_DOMMOD_PARAMETER_PARAMETERDMPTR_H
