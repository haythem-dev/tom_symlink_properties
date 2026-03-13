#ifndef GUARD_LIBLOCKMANAGER_LOCKENTRYPTR_H
#define GUARD_LIBLOCKMANAGER_LOCKENTRYPTR_H

#include <boost/shared_ptr.hpp>

namespace libLockManager
{
    class LockEntry;
    typedef boost::shared_ptr< LockEntry > LockEntryPtr;

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_LOCKENTRYPTR_H
