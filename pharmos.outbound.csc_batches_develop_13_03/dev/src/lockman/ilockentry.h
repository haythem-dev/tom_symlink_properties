#ifndef GUARD_LIBLOCKMANAGER_ILOCKENTRY_H
#define GUARD_LIBLOCKMANAGER_ILOCKENTRY_H

#include "iserializable.h"

namespace basar
{
namespace cmnutil
{
    class DateTime;
}
}


namespace libLockManager
{
    //---------------------------------------------------------------------------//
    // forward declaration section
    //---------------------------------------------------------------------------//
    class IKey;

    //---------------------------------------------------------------------------//
    // interface definition section
    //---------------------------------------------------------------------------//
    class ILockEntry : public ISerializable
    {
    public:
        virtual ~ILockEntry() {}

        virtual const IKey&            getKey()       const = 0;
        virtual const basar::DateTime& getTimestamp() const = 0;
    };

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ILOCKENTRY_H
