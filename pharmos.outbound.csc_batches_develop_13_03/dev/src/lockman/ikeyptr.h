#ifndef GUARD_LIBLOCKMANAGER_IKEYPTR_H
#define GUARD_LIBLOCKMANAGER_IKEYPTR_H

#include <boost/shared_ptr.hpp>

namespace libLockManager
{
    class IKey;
    typedef boost::shared_ptr< IKey > IKeyPtr;

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_IKEYPTR_H
