#ifndef GUARD_LIBLOCKMANAGER_KEYPTR_H
#define GUARD_LIBLOCKMANAGER_KEYPTR_H

#include <boost/shared_ptr.hpp>

namespace libLockManager
{
    class Key;
    typedef boost::shared_ptr< Key > KeyPtr;

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_KEYPTR_H
