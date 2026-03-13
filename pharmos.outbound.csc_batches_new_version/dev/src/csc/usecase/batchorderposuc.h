#ifndef GUARD_CSC_BATCHES_USECASE_BATCHORDERPOSUC_H
#define GUARD_CSC_BATCHES_USECASE_BATCHORDERPOSUC_H

#include <libbasarappl_systemeventmanager.h>
#include <libbasarappl_eventmacros.h>

#include "viewconn/ibatchorderpositionvcptr.h"

#include <libabbauw/domainmodule/batchorder/ibatchorderptr.h>
#include <libabbauw/icustomerfinderptr.h>

#include <libutil/irunnable.h>
#include <libutil/session.h>

namespace CSC_Batches
{
namespace useCase
{

	//-------------------------------------------------------------------------------------------------//
	// class declaration
	//-------------------------------------------------------------------------------------------------//
	class BatchOrderPosUC : public libutil::useCase::IRunnable
	{
	public:
		BatchOrderPosUC();
		~BatchOrderPosUC();
		
        void                                                                  injectBatchOrder( libabbauw::domMod::batchOrder::IBatchOrderPtr );
        void                                                                  injectCustomer( libabbauw::domMod::customer::ICustomerFinderPtr );

        void                                                                  injectBatchOrderPosVC( viewConn::IBatchOrderPositionVCPtr );

        void                                                                  setSession( libutil::infrastructure::session::ISessionPtr );

		// from IRunnable interface
		void                                                                  run();
		
        SYSTEM_EVENT_HANDLER_DECLARATION( CloseBatchOrderPositionsRequested );

	private:
        // forbidden
		BatchOrderPosUC( const BatchOrderPosUC& );
		BatchOrderPosUC& operator = ( const BatchOrderPosUC& );
		
		void registerEventHandlers( const bool );

        libutil::infrastructure::session::ISessionPtr                         getSession();

        viewConn::IBatchOrderPositionVCPtr                                    getBatchOrderPositionVC();

        libabbauw::domMod::batchOrder::IBatchOrderPtr                         getBatchOrder();
        libabbauw::domMod::customer::ICustomerFinderPtr                       getCustomer();
		
	private:
        libutil::infrastructure::session::ISessionPtr                         m_Session;

		viewConn::IBatchOrderPositionVCPtr                                    m_BatchOrderPositionVC;
        libabbauw::domMod::batchOrder::IBatchOrderPtr                         m_BatchOrder;
        libabbauw::domMod::customer::ICustomerFinderPtr                       m_Customer;
	};
	
} // namespace useCase
} // namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_USECASE_BATCHORDERPOSUC_H
