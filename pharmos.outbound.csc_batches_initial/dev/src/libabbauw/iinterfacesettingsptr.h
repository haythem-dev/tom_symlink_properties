#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IINTERFACESETTINGSSPTR_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IINTERFACESETTINGSSPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod 
{
namespace parameter
{
    class IInterfaceSettings;
    typedef boost::shared_ptr< IInterfaceSettings > IInterfaceSettingsPtr;
	
} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_PARAMETER_IINTERFACESETTINGSSPTR_H
