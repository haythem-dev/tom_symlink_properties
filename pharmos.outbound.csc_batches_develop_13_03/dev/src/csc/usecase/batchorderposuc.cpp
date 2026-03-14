#include "batchorderposuc.h"

#include "viewconn/ibatchorderpositionvc.h"

#include <libabbauw/domainmodule/batchorder/ibatchorder.h>
#include <libabbauw/icustomerfinder.h>

#include <libabbauw/loggerpool/libabbauw_loggerpool.h>
#include <libabbauw/libabbauw_properties_definitions.h>

#include <libutil/usecase.h>

namespace CSC_Batches
{
namespace useCase
{
    BEGIN_HANDLERS_REGISTRATION( BatchOrderPosUC )
        SYSTEM_EVENT_HANDLER_REGISTRATION( BatchOrderPosUC, CloseBatchOrderPositionsRequested     )
    END_HANDLERS_REGISTRATION()

    BatchOrderPosUC::BatchOrderPosUC()
    {
    }

    BatchOrderPosUC::~BatchOrderPosUC()
    {
    }

    void BatchOrderPosUC::injectBatchOrder( libabbauw::domMod::batchOrder::IBatchOrderPtr dm )
    {
        m_BatchOrder = dm;
    }

    libabbauw::domMod::batchOrder::IBatchOrderPtr BatchOrderPosUC::getBatchOrder()
    {
        CHECK_INSTANCE_EXCEPTION( m_BatchOrder.get() );
        return m_BatchOrder;
    }

    void BatchOrderPosUC::injectCustomer( libabbauw::domMod::customer::ICustomerFinderPtr customer )
    {
        m_Customer = customer;
    }

    libabbauw::domMod::customer::ICustomerFinderPtr BatchOrderPosUC::getCustomer()
    {
        CHECK_INSTANCE_EXCEPTION( m_Customer.get() );
        return m_Customer;
    }

    void BatchOrderPosUC::injectBatchOrderPosVC( viewConn::IBatchOrderPositionVCPtr vc )
    {
        m_BatchOrderPositionVC = vc;
    }

    viewConn::IBatchOrderPositionVCPtr BatchOrderPosUC::getBatchOrderPositionVC()
    {
        CHECK_INSTANCE_EXCEPTION( m_BatchOrderPositionVC.get() );
        return m_BatchOrderPositionVC;
    }

    void BatchOrderPosUC::setSession( libutil::infrastructure::session::ISessionPtr session )
    {
        m_Session = session;
    }

    libutil::infrastructure::session::ISessionPtr BatchOrderPosUC::getSession()
    {
        CHECK_INSTANCE_EXCEPTION( m_Session.get() );
        return m_Session;
    }

    SYSTEM_EVENT_HANDLER_DEFINITION( BatchOrderPosUC, CloseBatchOrderPositionsRequested )
    {
        METHODNAME_DEF( BatchOrderPosUC, CloseBatchOrderPositionsRequested )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, basar::VarString( "SystemEvent " ) + fun );
        rSource;
        yitFromEvSrc;

        basar::appl::EventReturnType result( basar::appl::HANDLER_OK );
        getBatchOrderPositionVC()->hide();

        return result;
    }

    void BatchOrderPosUC::run()
    {
        METHODNAME_DEF( BatchOrderPosUC, run )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerUseCases, fun );

        registerEventHandlers( true );

        getCustomer()->findByKey( getBatchOrder()->getHead().getInt32( libabbauw::properties::PHARMACYNO ) );

        getBatchOrderPositionVC()->matchFromBatchOrderHead( getBatchOrder()->getHead() );
        getBatchOrderPositionVC()->matchFromBatchOrderPositions( getBatchOrder()->getPositionCollection() );
        getBatchOrderPositionVC()->matchFromCustomer( getCustomer()->get() );
        getBatchOrderPositionVC()->show();

        registerEventHandlers( false );
    }
} // namespace useCase
} // namespace CSC_Batches
