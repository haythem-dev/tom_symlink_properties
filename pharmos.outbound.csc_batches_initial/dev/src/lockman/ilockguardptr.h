#ifndef GUARD_LIBLOCKMANAGER_ILOCKGUARDPTR_H
#define GUARD_LIBLOCKMANAGER_ILOCKGUARDPTR_H

#include <boost/shared_ptr.hpp>

namespace libLockManager
{
    class ILockGuard;
    typedef boost::shared_ptr< ILockGuard > ILockGuardPtr;

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ILOCKGUARDPTR_H
