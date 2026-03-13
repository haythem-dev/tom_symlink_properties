#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETERDMPTR_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETERDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace domMod
	{
		namespace parameter
		{
			class IParameterDM;
			typedef boost::shared_ptr< IParameterDM > IParameterDMPtr;

		} // end namespace parameter
	} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETERDMPTR_H
