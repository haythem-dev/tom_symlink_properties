#ifndef GUARD_LIBLOCKMANAGER_LOCKMANAGERPTR_H
#define GUARD_LIBLOCKMANAGER_LOCKMANAGERPTR_H

#include <boost/shared_ptr.hpp>

namespace libLockManager
{
    class LockManager;
    typedef boost::shared_ptr< LockManager > LockManagerPtr;

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_LOCKMANAGERPTR_H
