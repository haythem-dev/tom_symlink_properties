#ifndef GUARD_LIBABBAUW_PARAMETER_ISTOCKPARAMETERPTR_H
#define GUARD_LIBABBAUW_PARAMETER_ISTOCKPARAMETERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod 
{
namespace parameter
{
    class IStockParameter;
    typedef boost::shared_ptr< IStockParameter > IStockParameterPtr;
	
} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_PARAMETER_ISTOCKPARAMETERPTR_H
