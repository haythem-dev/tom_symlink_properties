#ifndef GUARD_LIBLOCKMANAGER_LOCKGUARDFACTORYPTR_H
#define GUARD_LIBLOCKMANAGER_LOCKGUARDFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libLockManager
{
    class LockGuardFactory;
    typedef boost::shared_ptr< LockGuardFactory > LockGuardFactoryPtr;

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_LOCKGUARDFACTORYPTR_H
