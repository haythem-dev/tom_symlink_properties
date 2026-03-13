#ifndef GUARD_LIBLOCKMANAGER_ILOCKENTRYPTR_H
#define GUARD_LIBLOCKMANAGER_ILOCKENTRYPTR_H

#include <boost/shared_ptr.hpp>

namespace libLockManager
{
    class ILockEntry;
    typedef boost::shared_ptr< ILockEntry > ILockEntryPtr;

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ILOCKENTRYPTR_H
