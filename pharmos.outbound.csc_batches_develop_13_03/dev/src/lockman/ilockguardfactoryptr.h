#ifndef GUARD_LIBLOCKMANAGER_ILOCKGUARDFACTORYPTR_H
#define GUARD_LIBLOCKMANAGER_ILOCKGUARDFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libLockManager
{
    class ILockGuardFactory;
    typedef boost::shared_ptr< ILockGuardFactory > ILockGuardFactoryPtr;

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ILOCKGUARDFACTORYPTR_H
