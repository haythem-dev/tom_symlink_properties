#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IORDERPARAMETERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IORDERPARAMETERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod 
{
namespace parameter
{
    class IOrderParameter;
    typedef boost::shared_ptr< IOrderParameter > IOrderParameterPtr;
	
} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_DOMMOD_PARAMETER_IORDERPARAMETERPTR_H
