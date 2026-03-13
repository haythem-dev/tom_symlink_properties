#ifndef GUARD_LIBLOCKMANAGER_ISERIALIZABLE_H
#define GUARD_LIBLOCKMANAGER_ISERIALIZABLE_H

#include <map>
#include <libbasar_definitions.h>
#include <libbasarcmnutil_bstring.h>

//---------------------------------------------------------------------------//
// interface definition section
//---------------------------------------------------------------------------//
namespace libLockManager
{

    class ISerializable
    {
    public:
        virtual ~ISerializable() {}

        typedef std::map< basar::VarString, basar::VarString > DataCollection;
        virtual DataCollection                                 serialize() const     = 0;
    };

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ISERIALIZABLE_H
