#include "lockmanager.h"

#include "ilockaccessor.h"
#include "instancechecker_macro_definition.h"
#include "alreadylockedexception.h"

namespace libLockManager
{

    LockManager::LockManager()
    {
    }

    LockManager::~LockManager()
    {
    }

    void LockManager::injectAccessor( accessor::ILockAccessorPtr accessor )
    {
        m_Accessor = accessor;
    }

    accessor::ILockAccessorPtr LockManager::getAccessor() const
    {
        CHECK_INSTANCE_EXCEPTION( m_Accessor );
        return m_Accessor;
    }

    void LockManager::lock( const ILockEntry& lockEntry )
    {
        getAccessor()->save( lockEntry );
    }

    bool LockManager::tryLock( const ILockEntry& lockEntry )
    {
        bool isAlreadyLocked = false;
        try
        {
            lock( lockEntry );
        }
        catch( exceptions::AlreadyLockedException& )
        {
            isAlreadyLocked = true;
        }

        return ! isAlreadyLocked;
    }

    void LockManager::unlock( const IKey& key )
    {
        getAccessor()->erase( key );
    }

    bool LockManager::isLocked( const IKey& key ) const
    {
        return getAccessor()->findByKey( key );
    }

    void LockManager::eraseStaleLocks()
    {
        getAccessor()->eraseStaleLocks();
    }

} // end namespace libLockManager
