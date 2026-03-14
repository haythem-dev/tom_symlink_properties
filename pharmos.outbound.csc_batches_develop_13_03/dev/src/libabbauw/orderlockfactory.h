#ifndef GUARD_LIBABBAUW_LOCK_ORDERLOCKFACTORY_H
#define GUARD_LIBABBAUW_LOCK_ORDERLOCKFACTORY_H

#include "iorderlockfactory.h"

#include <ilockguardfactoryptr.h>
#include <libutil/session.h>

namespace libabbauw
{
namespace lock
{

    class OrderLockFactory : public IOrderLockFactory
    {
        public:
            OrderLockFactory();
            ~OrderLockFactory();

            void                                          setSession( libutil::infrastructure::session::ISessionPtr );
            void                                          injectLockGuardFactory( libLockManager::ILockGuardFactoryPtr );

            // from ITransferOrderLockFactory interface
            libLockManager::ILockEntryPtr                 createLockEntry( basar::Int32 orderNo );
            libLockManager::ILockGuardPtr                 createLockGuard( basar::Int32 orderNo );
            libLockManager::IKeyPtr                       createKey      ( basar::Int32 orderNo );

        private:
            // forbidden
            OrderLockFactory( const OrderLockFactory& );
            OrderLockFactory& operator = ( const OrderLockFactory& );

        private:
            libutil::infrastructure::session::ISessionPtr m_Session;
            libLockManager::ILockGuardFactoryPtr          m_LockGuardFactory;
    };

} // end namespace lock
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_LOCK_ORDERLOCKFACTORY_H
