#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace domMod
	{
		namespace parameter
		{
			class IParameter;
			typedef boost::shared_ptr< IParameter > IParameterPtr;

		} // end namespace parameter
	} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETERPTR_H
