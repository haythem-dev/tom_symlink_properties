#include "lockguardfactory.h"

#include "lockguard.h"

#include <boost/make_shared.hpp>

namespace libLockManager
{

    LockGuardFactory::LockGuardFactory()
    {
    }

    LockGuardFactory::~LockGuardFactory()
    {
    }

    void LockGuardFactory::injectLockManager( ILockManagerPtr lockManager )
    {
        m_LockManager = lockManager;
    }

    ILockGuardPtr LockGuardFactory::createLockGuard( ILockEntryPtr lockEntry )
    {
        return boost::make_shared< LockGuard >( m_LockManager, lockEntry );
    }

} // end namespace libLockManager
