#ifndef GUARD_LIBABBAUW_DOMMOD_PARAMETER_IUSERFILTERSETTINGSPTR_H
#define GUARD_LIBABBAUW_DOMMOD_PARAMETER_IUSERFILTERSETTINGSPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod 
{
namespace parameter
{
    class IUserFilterSettings;
    typedef boost::shared_ptr< IUserFilterSettings > IUserFilterSettingsPtr;
	
} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_PARAMETER_IUSERFILTERSETTINGSPTR_H
