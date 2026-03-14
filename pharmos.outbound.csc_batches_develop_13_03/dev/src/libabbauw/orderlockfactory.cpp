#include "orderlockfactory.h"

#include <lockentry.h>
#include <lockentryptr.h>
#include <ilockguardfactory.h>
#include <key.h>
#include <keyptr.h>

#include <boost/make_shared.hpp>

namespace libabbauw
{
namespace lock
{
    OrderLockFactory::OrderLockFactory()
    {
    }

    OrderLockFactory::~OrderLockFactory()
    {
    }

    void OrderLockFactory::setSession( libutil::infrastructure::session::ISessionPtr session )
    {
        m_Session = session;
    }

    void OrderLockFactory::injectLockGuardFactory( libLockManager::ILockGuardFactoryPtr lockGuardFactory )
    {
        m_LockGuardFactory = lockGuardFactory;
    }

    libLockManager::ILockEntryPtr OrderLockFactory::createLockEntry( basar::Int32 orderNo )
    {
        libLockManager::LockEntryPtr lockEntry = boost::make_shared< libLockManager::LockEntry >( createKey( orderNo ) );
        lockEntry->setUserName( m_Session->getUser()->getUserName() );

        return lockEntry;
    }

    libLockManager::ILockGuardPtr OrderLockFactory::createLockGuard( basar::Int32 orderNo )
    {
        return m_LockGuardFactory->createLockGuard( createLockEntry( orderNo ) );
    }

    libLockManager::IKeyPtr OrderLockFactory::createKey( basar::Int32 orderNo )
    {
        libLockManager::KeyPtr key = boost::make_shared< libLockManager::Key >( orderNo, m_Session->getUser()->getAreaID() );
        key->setIdentifierName( "orderno ");

        return key;
    }

} // end namespace lock
} // end namespace libabbauw
