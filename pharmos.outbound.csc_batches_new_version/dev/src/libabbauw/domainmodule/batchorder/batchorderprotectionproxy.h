#ifndef GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERPROTECTIONPROXY_H
#define GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERPROTECTIONPROXY_H

#include "ibatchorder.h"
#include "ibatchorderptr.h"

#include "iorderlockfactoryptr.h"

#include <ilockguardptr.h>
#include <libbasar_definitions.h>

//----------------------------------------------------------------------------//
// forward declaration section
//----------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
            class AccessorPropertyTableRef;
        }
    }
}

namespace log4cplus
{
    class Logger;
}

namespace libabbauw
{
namespace domMod
{
namespace batchOrder
{

    class BatchOrderProtectionProxy : public IBatchOrder
    {
    public:
        BatchOrderProtectionProxy();
        ~BatchOrderProtectionProxy();

        void                                                 injectBatchOrder( IBatchOrderPtr );
        void                                                 injectOrderLockFactory( lock::IOrderLockFactoryPtr );

        // from IBatchOrder interface
        basar::db::aspect::AccessorPropertyTable_YIterator   getHead();
        basar::db::aspect::AccessorPropertyTableRef          getPositionCollection();

        bool                                                 isEmpty() const;

        void                                                 resetSearchYit();
        basar::db::aspect::AccessorPropertyTable_YIterator   getSearchYit();

        void                                                 findByKey( basar::db::aspect::AccessorPropertyTable_YIterator );

        void                                                 release();

    private:
        // forbidden
        BatchOrderProtectionProxy( const BatchOrderProtectionProxy& );
        BatchOrderProtectionProxy operator = ( const BatchOrderProtectionProxy& );

        const log4cplus::Logger&                             getLogger() const;
        IBatchOrderPtr                                       getBatchOrder() const;
        lock::IOrderLockFactoryPtr                           getOrderLockFactory() const;

        void                                                 lock( basar::Int32 orderNo );
        
    private:
        const log4cplus::Logger&                             m_Logger;
        IBatchOrderPtr                                       m_BatchOrder;
        lock::IOrderLockFactoryPtr                           m_OrderLockFactory;
        libLockManager::ILockGuardPtr                        m_LockGuard;
    };

} // end namespace batchOrder
} // end namespace domMod
} // end namespace libabbauw

#endif GUARD_LIBABBAUW_DOMMOD_BATCHORDER_BATCHORDERPROTECTIONPROXY_H
