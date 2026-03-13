#ifndef GUARD_LIBLOCKMANAGER_ACCESSOR_LOCKACCESSORPTR_H
#define GUARD_LIBLOCKMANAGER_ACCESSOR_LOCKACCESSORPTR_H

#include <boost/shared_ptr.hpp>

namespace libLockManager
{
namespace accessor
{
    class LockAccessor;
    typedef boost::shared_ptr< LockAccessor > LockAccessorPtr;

} // end namespace accessor
} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ACCESSOR_LOCKACCESSORPTR_H