#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROTOCOLGETTERPTR_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROTOCOLGETTERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace componentManager 
{
    class IOrderProtocolGetter;
    typedef boost::shared_ptr< IOrderProtocolGetter > IOrderProtocolGetterPtr;
	
} // end namespace componentManager
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROTOCOLGETTERPTR_H
