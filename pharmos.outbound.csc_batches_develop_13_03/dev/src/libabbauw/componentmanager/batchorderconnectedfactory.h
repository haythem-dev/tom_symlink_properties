#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_BATCHORDERCONNECTEDFACTORY_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_BATCHORDERCONNECTEDFACTORY_H

#include "ibatchorderconnectedfactory.h"

#include "domainmodule/batchorder/ibatchorderheadfinderptr.h"
#include "domainmodule/batchorder/ibatchorderpositioncollectionfinderptr.h"

#include "icscordergetterptr.h"
#include "iorderlockfactoryptr.h"

#include "domainmodule/batchorder/batchorderdmptr.h"

#include <libutil/accessor.h>
#include <libutil/dbconnection.h>

//------------------------------------------------------------------------------//
// Forward declarations
//------------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
        }
    }
}

namespace libabbauw
{
namespace componentManager
{
    class BatchOrderConnectedFactory : public IBatchOrderConnectedFactory
    {
    public:
        BatchOrderConnectedFactory();
        ~BatchOrderConnectedFactory();

        void                                                       injectDBConnection( libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr );
        void                                                       injectCSCOrderGetter( ICSCOrderGetterPtr );
        void                                                       injectOrderLockFactory( lock::IOrderLockFactoryPtr );

        // from IBatchOrderDisconnectedFactory interface
        domMod::batchOrder::IBatchOrderPtr                         create( basar::db::aspect::AccessorPropertyTable_YIterator ) const;

    private:
        basar::db::aspect::ConnectionRef                           getDBConnection()                       const;
        ICSCOrderGetterPtr                                         getCSCOrderGetter()                     const;
        lock::IOrderLockFactoryPtr                                 getOrderLockFactory()                   const;

        libutil::infrastructure::accessor::IAccessorPtr            getBatchOrderHeadAccessor()             const;
        libutil::infrastructure::accessor::IAccessorPtr            getBatchOrderPosCollectionAccessor()    const;

        domMod::batchOrder::IBatchOrderHeadFinderPtr               getBatchOrderHeadFinder()               const;
        domMod::batchOrder::IBatchOrderPositionCollectionFinderPtr getBatchOrderPositionCollectionFinder() const;

        domMod::batchOrder::BatchOrderDMPtr                        getBatchOrderDM()                       const;
        domMod::batchOrder::IBatchOrderPtr                         getBatchOrderProtectionProxy()          const;

    private:
        // forbidden
        BatchOrderConnectedFactory( const BatchOrderConnectedFactory& );
        BatchOrderConnectedFactory& operator = ( const BatchOrderConnectedFactory& );

        libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr m_DBConnectionDataGetter;
        ICSCOrderGetterPtr                                                    m_CscOrderGetter;
        lock::IOrderLockFactoryPtr                                            m_OrderLockFactory;
    };

} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_BATCHORDERCONNECTEDFACTORY_H
