#ifndef GUARD_LIBLOCKMANAGER_LOCKGUARDFACTORY_H
#define GUARD_LIBLOCKMANAGER_LOCKGUARDFACTORY_H

#include "ilockguardfactory.h"
#include "ilockmanagerptr.h"

//---------------------------------------------------------------------------//
// class definition section
//---------------------------------------------------------------------------//
namespace libLockManager
{

    class LockGuardFactory : public ILockGuardFactory
    {
    public:
        LockGuardFactory();
        ~LockGuardFactory();

        void                 injectLockManager( ILockManagerPtr );

        // from ILockGuardFactory interface
        ILockGuardPtr        createLockGuard( ILockEntryPtr );

    private:
        ILockManagerPtr      m_LockManager;
    };

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_LOCKGUARDFACTORY_H
