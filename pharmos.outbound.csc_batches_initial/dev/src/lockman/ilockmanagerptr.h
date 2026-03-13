#ifndef GUARD_LIBLOCKMANAGER_ILOCKMANAGERPTR_H
#define GUARD_LIBLOCKMANAGER_ILOCKMANAGERPTR_H

#include <boost/shared_ptr.hpp>

namespace libLockManager
{
    class ILockManager;
    typedef boost::shared_ptr< ILockManager > ILockManagerPtr;

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ILOCKMANAGERPTR_H
