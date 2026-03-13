#ifndef GUARD_LIBLOCKMANAGER_ILOCKGUARDFACTORY_H
#define GUARD_LIBLOCKMANAGER_ILOCKGUARDFACTORY_H

#include "ilockguardptr.h"
#include "ilockentryptr.h"

//---------------------------------------------------------------------------//
// interface definition section
//---------------------------------------------------------------------------//
namespace libLockManager
{

    class ILockGuardFactory
    {
    public:
        virtual ~ILockGuardFactory() {}

        virtual ILockGuardPtr createLockGuard( ILockEntryPtr ) = 0;
    };

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ILOCKGUARDFACTORY_H
