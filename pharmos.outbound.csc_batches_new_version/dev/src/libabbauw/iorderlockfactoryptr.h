#ifndef GUARD_LIBABBAUW_LOCK_IORDERLOCKFACTORYPTR_H
#define GUARD_LIBABBAUW_LOCK_IORDERLOCKFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace lock
{
    class IOrderLockFactory;
    typedef boost::shared_ptr< IOrderLockFactory > IOrderLockFactoryPtr;

} // end namespace lock
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_LOCK_IORDERLOCKFACTORYPTR_H
