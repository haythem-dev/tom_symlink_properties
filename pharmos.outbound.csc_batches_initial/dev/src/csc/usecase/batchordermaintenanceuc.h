#ifndef GUARD_CSC_BATCHES_USECASE_BATCHORDERMAINTENANCEUC_H
#define GUARD_CSC_BATCHES_USECASE_BATCHORDERMAINTENANCEUC_H

#include "iusecasegetterptr.h"

#include <libbasarappl_systemeventmanager.h>
#include <libbasarappl_eventmacros.h>

#include "viewconn/ibatchordermaintenancevcptr.h"
#include "viewconn/ibatchordermaintenancevc.h"
#include "iorderreleaseinformationvcptr.h"

#include <libabbauw/domainmodule/batchorder/ibatchordercollectionfinderptr.h>

#include <libutil/irunnable.h>
#include <libutil/session.h>

namespace CSC_Batches
{
namespace useCase
{

	//-------------------------------------------------------------------------------------------------//
	// class declaration
	//-------------------------------------------------------------------------------------------------//
	class BatchOrderMaintenanceUC : public libutil::useCase::IRunnable
	{
	public:
		BatchOrderMaintenanceUC();
		~BatchOrderMaintenanceUC();
		
        void                                                          injectBatchOrderCollectionFinder( libabbauw::domMod::batchOrder::IBatchOrderCollectionFinderPtr );
        void                                                          injectBatchOrderMaintenanceVC( viewConn::IBatchOrderMaintenanceVCPtr );
        void                                                          injectOrderReleaseInformationVC( viewConn::IOrderReleaseInformationVCPtr );

		void                                                          injectUseCaseGetter( componentManager::IUseCaseGetterPtr );
        void                                                          setSession( libutil::infrastructure::session::ISessionPtr );

		// from IRunnable interface
		void                                                          run();
		
		// system event handlers
        SYSTEM_EVENT_HANDLER_DECLARATION( SearchBatchOrdersRequested     );
		SYSTEM_EVENT_HANDLER_DECLARATION( ShowPositionsRequested         );

		SYSTEM_EVENT_HANDLER_DECLARATION( ReleaseSelectedOrdersRequested );
        SYSTEM_EVENT_HANDLER_DECLARATION( ReleaseFilteredOrdersRequested );
		
	private:
        // forbidden
		BatchOrderMaintenanceUC( const BatchOrderMaintenanceUC& );
		BatchOrderMaintenanceUC& operator= ( const BatchOrderMaintenanceUC& );
		
		void registerEventHandlers( const bool );

        componentManager::IUseCaseGetterPtr                           getUseCaseGetter();
        libutil::infrastructure::session::ISessionPtr                 getSession();

        viewConn::IBatchOrderMaintenanceVCPtr                         getBatchOrderMaintenanceVC();
        viewConn::IOrderReleaseInformationVCPtr                       getOrderReleaseInformationVC();

        libabbauw::domMod::batchOrder::IBatchOrderCollectionFinderPtr getBatchOrderCollectionFinder();

        void                                                          searchBatchOrders();
        void                                                          releaseOrder( basar::db::aspect::AccessorPropertyTable_YIterator );
		
	private:
        componentManager::IUseCaseGetterPtr                           m_UseCaseGetter;
        libutil::infrastructure::session::ISessionPtr                 m_Session;

		viewConn::IBatchOrderMaintenanceVCPtr                         m_BatchOrderMaintenanceVC;
        viewConn::IOrderReleaseInformationVCPtr                       m_OrderReleaseInformationVC;
        libabbauw::domMod::batchOrder::IBatchOrderCollectionFinderPtr m_BatchOrderCollectionFinder;
	};
	
} // namespace useCase
} // namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_USECASE_BATCHORDERMAINTENANCEUC_H
