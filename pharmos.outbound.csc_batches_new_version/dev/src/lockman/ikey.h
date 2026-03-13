#ifndef GUARD_LIBLOCKMANAGER_IKEY_H
#define GUARD_LIBLOCKMANAGER_IKEY_H

#include "iserializable.h"

//---------------------------------------------------------------------------//
// interface definition section
//---------------------------------------------------------------------------//
namespace libLockManager
{

    class IKey : public ISerializable
    {
    public:
        virtual ~IKey() {}
    };

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_IKEY_H
