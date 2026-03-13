#ifndef GUARD_LIBABBAUW_LOCK_ORDERLOCKFACTORYPTR_H
#define GUARD_LIBABBAUW_LOCK_ORDERLOCKFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace lock
{
    class OrderLockFactory;
    typedef boost::shared_ptr< OrderLockFactory > OrderLockFactoryPtr;

} // end namespace lock
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_LOCK_ORDERLOCKFACTORYPTR_H
