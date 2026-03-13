#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETEREDITPTR_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETEREDITPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace domMod
	{
		namespace parameter
		{
			class IParameterEdit;
			typedef boost::shared_ptr< IParameterEdit > IParameterEditPtr;

		} // end namespace parameter
	} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_DOMMOD_PARAMETER_IPARAMETEREDITPTR_H
