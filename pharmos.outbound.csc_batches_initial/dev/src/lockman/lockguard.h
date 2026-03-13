#ifndef GUARD_LIBLOCKMANAGER_LOCKGUARD_H
#define GUARD_LIBLOCKMANAGER_LOCKGUARD_H

#include "ilockguard.h"

#include "ilockmanagerptr.h"
#include "ilockentryptr.h"

//---------------------------------------------------------------------------//
// forward declaration section
//---------------------------------------------------------------------------//
namespace log4cplus
{
    class Logger;
}

//---------------------------------------------------------------------------//
// class definition section
//---------------------------------------------------------------------------//
namespace libLockManager
{

    class LockGuard : public ILockGuard
    {
    public:
        LockGuard( ILockManagerPtr, ILockEntryPtr );
        ~LockGuard();

        // from ILockGuard interface
        void lock();
        void unlock();

    private:
        // forbidden
        LockGuard();
        LockGuard( const LockGuard& );
        LockGuard& operator = ( const LockGuard& );

    private:
        const log4cplus::Logger& m_Logger;
        ILockManagerPtr          m_LockManager;
        ILockEntryPtr            m_LockEntry;
        bool                     m_isLocked;
    };

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_LOCKGUARD_H
