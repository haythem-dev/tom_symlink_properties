#ifndef GUARD_LIBABBAUW_LOCK_IORDERLOCKFACTORY_H
#define GUARD_LIBABBAUW_LOCK_IORDERLOCKFACTORY_H

#include <ilockentryptr.h>
#include <ikeyptr.h>
#include <ilockguardptr.h>

#include <libbasar_definitions.h>

namespace libabbauw
{
namespace lock
{

    class IOrderLockFactory
    {
        public:
            virtual ~IOrderLockFactory() {}

            virtual libLockManager::ILockEntryPtr createLockEntry( basar::Int32 orderNo ) = 0;
            virtual libLockManager::ILockGuardPtr createLockGuard( basar::Int32 orderNo ) = 0;
            virtual libLockManager::IKeyPtr       createKey      ( basar::Int32 orderNo ) = 0;
    };

} // end namespace lock
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_LOCK_IORDERLOCKFACTORY_H
