#ifndef GUARD_LIBLOCKMANAGER_ILOCKGUARD_H
#define GUARD_LIBLOCKMANAGER_ILOCKGUARD_H

#include "ilockentryptr.h"

//---------------------------------------------------------------------------//
// interface definition section
//---------------------------------------------------------------------------//
namespace libLockManager
{

    class ILockGuard
    {
    public:
        virtual ~ILockGuard() {}

        virtual void lock()                = 0;
        virtual void unlock()              = 0;
    };

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ILOCKGUARD_H
