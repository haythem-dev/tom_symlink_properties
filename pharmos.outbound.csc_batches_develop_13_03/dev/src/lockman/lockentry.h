#ifndef GUARD_LIBLOCKMANAGER_LOCKENTRY_H
#define GUARD_LIBLOCKMANAGER_LOCKENTRY_H

#include "ilockentry.h"
#include "ikeyptr.h"

#include <libbasarcmnutil_i18nstring.h>
#include <libbasarcmnutil_datetime.h>

//---------------------------------------------------------------------------//
// class definition section
//---------------------------------------------------------------------------//
namespace libLockManager
{
    class LockEntry : public ILockEntry
    {
    public:
        LockEntry( IKeyPtr );
        virtual ~LockEntry();

        // from ILockEntry interface
        const IKey&               getKey()       const;
        // no setter for time stamp because it is set automatically by the backend when saved via accessor
        const basar::DateTime&    getTimestamp() const;

        // own methods
        void                      setUserName( const basar::I18nString& );
        const basar::I18nString&  getUserName()  const;

        // from ISerializable interface
        virtual DataCollection    serialize()    const;

    private:
        // forbidden
        LockEntry();

    private:
        IKeyPtr                   m_Key;
        basar::DateTime           m_Timestamp;
        basar::I18nString         m_UserName;
    };

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_LOCKENTRY_H
