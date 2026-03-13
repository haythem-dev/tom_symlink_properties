#ifndef GUARD_CSCBATCHES_USECASE_BATCHORDEREDITIONUC_H
#define GUARD_CSCBATCHES_USECASE_BATCHORDEREDITIONUC_H

#include <libutil/irunnable.h>
#include "iusecasegetterptr.h"
#include "icsvexportgetterptr.h"

#include <libbasarappl_systemeventmanager.h>
#include <libbasarappl_eventmacros.h>

#include "ibatchordereditionvcptr.h"
#include "ibatchordereditionvc.h"
#include "iorderreleaseinformationvcptr.h"

#include "bvostatusdataexportservice.h"

#include "libabbauw/itransferorderheadcollectionfinderptr.h"
#include "libabbauw/domainmodule/orderprotocol/iorderprotocolptr.h"
#include "libabbauw/iparametereditptr.h"

#include "itransferorderptr.h"
#include "usecase/itendercreatorptr.h"
//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace appl
    {
        struct EventReturnStruct;
    }
}
    

namespace CSC_Batches
{
namespace useCase {

	//-------------------------------------------------------------------------------------------------//
	// class declaration
	//-------------------------------------------------------------------------------------------------//
	class BatchOrderEditionUC : public libutil::useCase::IRunnable
	{
	public:
		BatchOrderEditionUC();
		~BatchOrderEditionUC();

		// VCs
        void injectVC( viewConn::IBatchOrderEditionVCPtr );
        void injectVC( viewConn::IOrderReleaseInformationVCPtr );

        // DMs
		void injectDM( libabbauw::domMod::transferOrder::ITransferOrderHeadCollectionFinderPtr );
        void injectDM( libabbauw::domMod::orderProtocol::IOrderProtocolPtr );
        void injectDM( libabbauw::domMod::parameter::IParameterEditPtr );

		// Usecase Controller
        void injectTenderCreatorUC( useCase::ITenderCreatorPtr  );
		void injectUseCaseGetter( componentManager::IUseCaseGetterPtr  );

		// from IRunnable interface
		void run();
		
		// event handlers
		SYSTEM_EVENT_HANDLER_DECLARATION( SearchBatchOrderRequested          );
		SYSTEM_EVENT_HANDLER_DECLARATION( ChangeOrderRequested               );
        SYSTEM_EVENT_HANDLER_DECLARATION( ChangeOrderFilteredRequested       );
        SYSTEM_EVENT_HANDLER_DECLARATION( SendOrderRequestsSelectedRequested );
        SYSTEM_EVENT_HANDLER_DECLARATION( SendOrderRequestsFilteredRequested );
		SYSTEM_EVENT_HANDLER_DECLARATION( ShowOrderPositionsRequested        );
		SYSTEM_EVENT_HANDLER_DECLARATION( ReleaseOrdersRequested             );
        SYSTEM_EVENT_HANDLER_DECLARATION( ReleaseOrdersPostponeRequested     );
		SYSTEM_EVENT_HANDLER_DECLARATION( ReleaseGreenOrdersRequested        );
		SYSTEM_EVENT_HANDLER_DECLARATION( ShowBVOStatusAll                   );
        SYSTEM_EVENT_HANDLER_DECLARATION( TransferOrderChanged               );

	private:
		BatchOrderEditionUC( const BatchOrderEditionUC& );
		BatchOrderEditionUC& operator= ( const BatchOrderEditionUC& );
		
		void registerEventHandlers( const bool forSave );

        viewConn::IBatchOrderEditionVC::YIterators filteredOrdersToYitCollection();
		bool searchBatchOrders();
        basar::appl::EventReturnType releaseOrders( viewConn::IBatchOrderEditionVC::YIterators, bool postpone );
		useCase::ITenderCreatorPtr   getTenderCreatorUC();
        bool isReserveQtyDifferent();
		
	private:
        componentManager::IUseCaseGetterPtr                                     m_UseCaseGetterPtr;

		viewConn::IBatchOrderEditionVCPtr                                       m_BatchOrderEditionVCPtr;
        viewConn::IOrderReleaseInformationVCPtr                                 m_OrderReleaseInformation;
		
		libabbauw::domMod::transferOrder::ITransferOrderHeadCollectionFinderPtr m_TransferOrderHeadCollectionFinderPtr;
        libabbauw::domMod::orderProtocol::IOrderProtocolPtr                     m_OrderProtocol;
        useCase::ITenderCreatorPtr                                              m_tenderCreatorUCPtr;
        libabbauw::domMod::parameter::IParameterEditPtr                         m_ParameterEdit;
	};
	
}// namespace useCase
}// namespace CSC_Batches
#endif // GUARD_CSCBATCHES_USECASE_BATCHORDEREDITIONUC_H
