#ifndef GUARD_LIBLOCKMANAGER_ACCESSOR_ILOCKACCESSORPTR_H
#define GUARD_LIBLOCKMANAGER_ACCESSOR_ILOCKACCESSORPTR_H

#include <boost/shared_ptr.hpp>

namespace libLockManager
{
namespace accessor
{
    class ILockAccessor;
    typedef boost::shared_ptr< ILockAccessor > ILockAccessorPtr;

} // end namespace accessor
} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ACCESSOR_ILOCKACCESSORPTR_H