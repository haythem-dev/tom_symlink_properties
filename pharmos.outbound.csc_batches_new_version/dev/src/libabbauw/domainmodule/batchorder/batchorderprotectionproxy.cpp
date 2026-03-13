#include "batchorderprotectionproxy.h"

#include "ibatchorder.h"
#include "iorderlockfactory.h"

#include "libabbauw_properties_definitions.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include <log4cplus/logger.h>
#include <libutil/misc/metaclassmacro_definitions.h>

#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>

#include <libutil/util.h>
#include <ilockguard.h>
#include <alreadylockedexception.h>

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning(disable: 4512 )
#endif
#include <QtCore/QCoreApplication>
#ifdef _MSC_VER
#pragma warning (pop)
#endif

namespace libabbauw
{
namespace domMod
{
namespace batchOrder
{
    BatchOrderProtectionProxy::BatchOrderProtectionProxy()
        : m_Logger( libabbauw::LoggerPool::loggerLibAbbaUW )
    {
        METHODNAME_DEF( BatchOrderProtectionProxy, BatchOrderProtectionProxy );
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    BatchOrderProtectionProxy::~BatchOrderProtectionProxy()
    {
        METHODNAME_DEF( BatchOrderProtectionProxy, ~BatchOrderProtectionProxy );
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    const log4cplus::Logger& BatchOrderProtectionProxy::getLogger() const
    {
        return m_Logger;
    }

    void BatchOrderProtectionProxy::injectBatchOrder( IBatchOrderPtr batchOrder )
    {
        m_BatchOrder = batchOrder;
    }

    IBatchOrderPtr BatchOrderProtectionProxy::getBatchOrder() const
    {
        CHECK_INSTANCE_EXCEPTION( m_BatchOrder.get() );
        return m_BatchOrder;
    }

    void BatchOrderProtectionProxy::injectOrderLockFactory( lock::IOrderLockFactoryPtr orderLockFactory )
    {
        m_OrderLockFactory = orderLockFactory;
    }

    lock::IOrderLockFactoryPtr BatchOrderProtectionProxy::getOrderLockFactory() const
    {
        CHECK_INSTANCE_EXCEPTION( m_OrderLockFactory.get() );
        return m_OrderLockFactory;
    }

    basar::db::aspect::AccessorPropertyTable_YIterator BatchOrderProtectionProxy::getHead()
    {
        return getBatchOrder()->getHead();
    }

    basar::db::aspect::AccessorPropertyTableRef BatchOrderProtectionProxy::getPositionCollection()
    {
        return getBatchOrder()->getPositionCollection();
    }

    bool BatchOrderProtectionProxy::isEmpty() const
    {
        return getBatchOrder()->isEmpty();
    }

    void BatchOrderProtectionProxy::resetSearchYit()
    {
        getBatchOrder()->resetSearchYit();
    }

    basar::db::aspect::AccessorPropertyTable_YIterator BatchOrderProtectionProxy::getSearchYit()
    {
        return getBatchOrder()->getSearchYit();
    }

    void BatchOrderProtectionProxy::findByKey( basar::db::aspect::AccessorPropertyTable_YIterator searchYit )
    {
        METHODNAME_DEF( BatchOrderProtectionProxy, findByKey );
        BLOG_TRACE_METHOD( getLogger(), fun );

        const basar::Int32 orderNo = searchYit.getInt32( properties::ORDERNO );

        try
        {
            lock( orderNo );
            getBatchOrder()->findByKey( searchYit );
        }
        catch( libLockManager::exceptions::AlreadyLockedException& )
        {
            // provide better error msg
            basar::VarString msg = QCoreApplication::translate( "BatchOrderProtectionProxy",
                "Order %1 is locked!").arg( orderNo ).toLocal8Bit().constData();
            throw libLockManager::exceptions::AlreadyLockedException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ) );
        }
    }

    void BatchOrderProtectionProxy::release()
    {
        getBatchOrder()->release();
    }

    void BatchOrderProtectionProxy::lock( basar::Int32 orderNo )
    {
        METHODNAME_DEF( BatchOrderProtectionProxy, lock );
        BLOG_TRACE_METHOD( getLogger(), fun );

        // check if lock has already been aquired
        if( m_LockGuard.get() == 0 )
        {
            // lock this order before getting it from DB
            libLockManager::ILockGuardPtr lockGuard = getOrderLockFactory()->createLockGuard( orderNo );
            lockGuard->lock();
            m_LockGuard = lockGuard;
        }
    }

} // end namespace batchOrder
} // end namespace domMod
} // end namespace libabbauw
