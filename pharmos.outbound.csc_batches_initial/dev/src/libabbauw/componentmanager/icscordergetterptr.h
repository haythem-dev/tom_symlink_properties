#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ICSCORDERGETTERPTR_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ICSCORDERGETTERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace componentManager 
{
    class ICSCOrderGetter;
    typedef boost::shared_ptr< ICSCOrderGetter > ICSCOrderGetterPtr;
	
} // end namespace componentManager
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_COMPONENTMANAGER_ICSCORDERGETTERPTR_H
