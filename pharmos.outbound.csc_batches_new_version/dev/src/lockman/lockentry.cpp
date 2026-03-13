#include "lockentry.h"
#include "ikey.h"

#include "serializer.h"

namespace libLockManager
{
    LockEntry::LockEntry( IKeyPtr key )
        : m_Key( key )
    {
    }

    LockEntry::~LockEntry()
    {
    }

    const IKey& LockEntry::getKey() const
    {
        return *m_Key;
    }

    const basar::DateTime& LockEntry::getTimestamp() const
    {
        return m_Timestamp;
    }

    void LockEntry::setUserName( const basar::I18nString& userName )
    {
        m_UserName = userName;
    }

    const basar::I18nString& LockEntry::getUserName() const
    {
        return m_UserName;
    }

    LockEntry::DataCollection LockEntry::serialize() const
    {
        serialization::Serializer serializer( m_Key->serialize() );
        serializer.add( "username", m_UserName );
        // timestamp is omitted - it will be automatically set by the backend

        return serializer.serialize();
    }

} // end namespace libLockManager
