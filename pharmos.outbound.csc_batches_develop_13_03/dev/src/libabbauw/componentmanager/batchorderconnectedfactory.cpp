#include "batchorderconnectedfactory.h"

#include "domainmodule/batchorder/batchorderdm.h"
#include "domainmodule/batchorder/batchorderhead.h"
#include "domainmodule/batchorder/batchorderprotectionproxy.h"
#include "domainmodule/batchorder/batchorderprotectionproxyptr.h"
#include "domainmodule/batchorder/batchorderheadptr.h"
#include "domainmodule/batchorder/batchorderpositioncollection.h"
#include "domainmodule/batchorder/batchorderpositioncollectionptr.h"

#include "icscordergetter.h"

#include "infrastructure/accessor/batchorder/batchorderheadacc.h"
#include "infrastructure/accessor/batchorder/batchorderheadacc_definitions.h"
#include "infrastructure/accessor/batchorder/batchorderpositioncollectionacc.h"
#include "infrastructure/accessor/batchorder/batchorderpositioncollectionacc_definitions.h"

#include <boost/make_shared.hpp>

#include <libbasardbaspect_accessorpropertytable_yiterator.h>

namespace libabbauw
{
namespace componentManager
{

    BatchOrderConnectedFactory::BatchOrderConnectedFactory()
    {
    }

    BatchOrderConnectedFactory::~BatchOrderConnectedFactory()
    {
    }

    libutil::infrastructure::accessor::IAccessorPtr BatchOrderConnectedFactory::getBatchOrderHeadAccessor() const
    {
        infrastructure::accessor::batchOrder::SelectBatchOrderHeadByKey();

        using libutil::infrastructure::accessor::AccessorProxy;
        return boost::make_shared< AccessorProxy >( infrastructure::accessor::batchOrder::lit::ACC_BATCH_ORDER_HEAD, getDBConnection() );
    }

    libutil::infrastructure::accessor::IAccessorPtr BatchOrderConnectedFactory::getBatchOrderPosCollectionAccessor() const
    {
        infrastructure::accessor::batchOrder::SelectBatchOrderPositionCollectionByKey();

        using libutil::infrastructure::accessor::AccessorProxy;
        return boost::make_shared< AccessorProxy >( infrastructure::accessor::batchOrder::lit::ACC_BATCH_ORDER_POSITION_COLLECTION, getDBConnection() );
    }

    domMod::batchOrder::IBatchOrderHeadFinderPtr BatchOrderConnectedFactory::getBatchOrderHeadFinder() const
    {
        domMod::batchOrder::BatchOrderHeadPtr dm = boost::make_shared< domMod::batchOrder::BatchOrderHead >();
        dm->injectBatchOrderHeadAccessor( getBatchOrderHeadAccessor() );

        return dm;
    }
    
    domMod::batchOrder::IBatchOrderPositionCollectionFinderPtr BatchOrderConnectedFactory::getBatchOrderPositionCollectionFinder() const
    {
        domMod::batchOrder::BatchOrderPositionCollectionPtr dm = boost::make_shared< domMod::batchOrder::BatchOrderPositionCollection >();
        dm->injectPositionCollectionAccessor( getBatchOrderPosCollectionAccessor() );

        return dm;
    }

    domMod::batchOrder::BatchOrderDMPtr BatchOrderConnectedFactory::getBatchOrderDM() const
    {
        using libutil::infrastructure::accessor::AccessorProxy;
        domMod::batchOrder::BatchOrderDMPtr dm = boost::make_shared< domMod::batchOrder::BatchOrderDM >();
        domMod::batchOrder::IBatchOrderHeadFinderPtr batchOrderHeadFinder = getBatchOrderHeadFinder();

        dm->injectBatchOrderHeadFinder( batchOrderHeadFinder );
        dm->injectBatchOrderPositionCollectionFinder( getBatchOrderPositionCollectionFinder() );
        dm->injectCscOrderReleaser( getCSCOrderGetter()->getCSCOrderDM( batchOrderHeadFinder ) );

        return dm;
    }

    domMod::batchOrder::IBatchOrderPtr BatchOrderConnectedFactory::getBatchOrderProtectionProxy() const
    {
        domMod::batchOrder::BatchOrderProtectionProxyPtr proxy = boost::make_shared< domMod::batchOrder::BatchOrderProtectionProxy >();
        proxy->injectBatchOrder( getBatchOrderDM() );
        proxy->injectOrderLockFactory( getOrderLockFactory() );

        return proxy;
    }

    void BatchOrderConnectedFactory::injectDBConnection( libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr dbConnDataGetter )
    {
        m_DBConnectionDataGetter = dbConnDataGetter;
    }

    basar::db::aspect::ConnectionRef BatchOrderConnectedFactory::getDBConnection() const
    {
        CHECK_INSTANCE_EXCEPTION( m_DBConnectionDataGetter.get() );
        return m_DBConnectionDataGetter->getCurrentConnection();
    }

    void BatchOrderConnectedFactory::injectCSCOrderGetter( ICSCOrderGetterPtr orderGetter )
    {
        m_CscOrderGetter = orderGetter;
    }

    ICSCOrderGetterPtr BatchOrderConnectedFactory::getCSCOrderGetter() const
    {
        CHECK_INSTANCE_EXCEPTION( m_CscOrderGetter.get() );
        return m_CscOrderGetter;
    }

    void BatchOrderConnectedFactory::injectOrderLockFactory( lock::IOrderLockFactoryPtr orderLockFactory )
    {
        m_OrderLockFactory = orderLockFactory;
    }

    lock::IOrderLockFactoryPtr BatchOrderConnectedFactory::getOrderLockFactory() const
    {
        CHECK_INSTANCE_EXCEPTION( m_OrderLockFactory.get() );
        return m_OrderLockFactory;
    }

    domMod::batchOrder::IBatchOrderPtr BatchOrderConnectedFactory::create( basar::db::aspect::AccessorPropertyTable_YIterator yit ) const
    {
        domMod::batchOrder::IBatchOrderPtr dm = getBatchOrderProtectionProxy();
        dm->resetSearchYit();
        dm->getSearchYit().match( yit );
        dm->findByKey( dm->getSearchYit() );

        return dm;
    }

} // end namespace componentManager
} // end namespace libabbauw
