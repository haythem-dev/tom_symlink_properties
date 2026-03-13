#include "lockguard.h"

#include "ilockmanager.h"
#include "ilockentry.h"

#include "metaclassmacro_definitions.h"
#include "apimisuseexception.h"

#include "liblockmanager_loggerpool.h"
#include <libbasarcmnutil_logging.h>

namespace libLockManager
{

    LockGuard::LockGuard( ILockManagerPtr lockManager, ILockEntryPtr lockEntry )
        : m_Logger( LoggerPool::loggerLiblockManager ), m_LockManager( lockManager ), m_LockEntry( lockEntry ), m_isLocked( false )
    {
    }

    LockGuard::~LockGuard()
    {
        METHODNAME_DEF( LockGuard, ~LockGuard );

        // all exceptions are catched here because otherwise the program will terminate
        // if this destructor is executed due to another exception
        try
        {
            if( m_isLocked )
            {
                unlock();
            }
        }
        catch( ... )
        {
            BLOG_ERROR( m_Logger, "LockGuard::unlock() failed" );
        }
    }

    void LockGuard::lock()
    {
        METHODNAME_DEF( LockGuard, lock );

        if( ! m_isLocked )
        {
            m_LockManager->lock( *m_LockEntry );
            m_isLocked = true;
        }
        else
        {
            basar::VarString msg = basar::VarString( fun ) + "has already been called before!";
            throw exceptions::APIMisuseException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ) );
        }
    }

    void LockGuard::unlock()
    {
        METHODNAME_DEF( LockGuard, unlock );

        if( m_isLocked )
        {
            m_LockManager->unlock( m_LockEntry->getKey() );
        }
        else
        {
            basar::VarString msg = basar::VarString( fun ) + "has already been called before!";
            throw exceptions::APIMisuseException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ) );
        }
    }

} // end namespace libLockManager
