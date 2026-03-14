#ifndef GUARD_CSCBATCHES_USECASE_TRANSFERORDERCREATORUC_H
#define GUARD_CSCBATCHES_USECASE_TRANSFERORDERCREATORUC_H

#include <libutil/irunnable.h>
#include "iusecasegetterptr.h"

// vc
#include "itransferordercreatorvcptr.h"

// dm
#include "libabbauw/icustomerfinderptr.h"
#include "libabbauw/ifindmanufacturerptr.h"
#include "libabbauw/ifindarticleptr.h"
#include "libabbauw/itransferorderinpreparationdmptr.h"
#include "libabbauw/itransferordercreationdmptr.h"
#include "libabbauw/ifindpurchaseordersupplierptr.h"
#include "libabbauw/domainmodule/orderprotocol/iorderprotocolptr.h"

#include "libabbauw/iordercheckparameterptr.h"

#include <libbasarappl_systemeventmanager.h>
#include <libbasarappl_eventmacros.h>

// translation
#pragma warning (push)
#pragma warning(disable: 4127 4512 )
#include <QtCore/QCoreApplication>
#pragma warning (pop)

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
namespace useCase 
{

	//-------------------------------------------------------------------------------------------------//
	// class declaration
	//-------------------------------------------------------------------------------------------------//
	class TransferOrderCreatorUC : public libutil::useCase::IRunnable
	{
        Q_DECLARE_TR_FUNCTIONS( TransferOrderCreatorUC )

	public:
		TransferOrderCreatorUC();
		~TransferOrderCreatorUC();
		
        void                                              injectVC( viewConn::ITransferOrderCreatorVCPtr );
        void                                              injectUseCaseGetter( componentManager::IUseCaseGetterPtr );
        void                                              injectDM( libabbauw::domMod::customer::ICustomerFinderPtr );
        void                                              injectDM( libabbauw::domMod::manufacturerdm::IFindManufacturerPtr );
        void                                              injectDM( libabbauw::domMod::article::IFindArticlePtr dm );
        void                                              injectDM( libabbauw::domMod::transferOrder::ITransferOrderInPreparationDMPtr );
        void                                              injectDM( libabbauw::domMod::transferOrder::ITransferOrderCreationDMPtr );
        void                                              injectDM( libabbauw::domMod::purchaseordersupplierdm::IFindPurchaseOrderSupplierPtr );
		void											  injectDM( libabbauw::domMod::orderProtocol::IOrderProtocolPtr );
        void                                              injectParameter( libabbauw::domMod::parameter::IOrderCheckParameterPtr );

		// from IRunnable interface
		void                                              run();

		// system event handlers
        SYSTEM_EVENT_HANDLER_DECLARATION( FindArticleRequested                      );
        SYSTEM_EVENT_HANDLER_DECLARATION( UpdateArticleRequested                    );
        SYSTEM_EVENT_HANDLER_DECLARATION( CheckArticleRequested                     );

			SYSTEM_EVENT_HANDLER_DECLARATION(CheckOrderNoMFRequested);
			SYSTEM_EVENT_HANDLER_DECLARATION(CheckManualHeadertextRequested);

        SYSTEM_EVENT_HANDLER_DECLARATION( FindPharmacyRequested                     );
        SYSTEM_EVENT_HANDLER_DECLARATION( UpdatePharmacyRequested                   );
        SYSTEM_EVENT_HANDLER_DECLARATION( CheckPharmacyNoRequested                  );
			SYSTEM_EVENT_HANDLER_DECLARATION(CheckHeadertextRequested);

        SYSTEM_EVENT_HANDLER_DECLARATION( AddPositionRequested                      );
        SYSTEM_EVENT_HANDLER_DECLARATION( ChangePositionRequested                   );
        SYSTEM_EVENT_HANDLER_DECLARATION( RemovePositionRequested                   );

        SYSTEM_EVENT_HANDLER_DECLARATION( SwitchToPosEditModeRequested              );
        SYSTEM_EVENT_HANDLER_DECLARATION( SwitchToPosInputModeRequested             );

        SYSTEM_EVENT_HANDLER_DECLARATION( ResetTransferOrderInPreparationRequested  );
        SYSTEM_EVENT_HANDLER_DECLARATION( CheckSaveStateRequested                   );
        SYSTEM_EVENT_HANDLER_DECLARATION( CheckCreateTransferOrderStateRequested    );
        SYSTEM_EVENT_HANDLER_DECLARATION( SaveTransferOrderInPreparationRequested   );
        SYSTEM_EVENT_HANDLER_DECLARATION( CreateTransferOrderRequested              );
        SYSTEM_EVENT_HANDLER_DECLARATION( DialogCloseRequested                      );
        
	private:
		TransferOrderCreatorUC( const TransferOrderCreatorUC& );
		TransferOrderCreatorUC& operator= ( const TransferOrderCreatorUC& );
		
		void registerEventHandlers(const bool forSave);

        void resetTransferOrderInPreparation();
        void saveTransferOrderInPreparation();

        void matchToArticle();
		
	private:
		viewConn::ITransferOrderCreatorVCPtr                                      m_TransferOrderCreatorVC;
        
        componentManager::IUseCaseGetterPtr                                       m_UseCaseGetter;
        
        libabbauw::domMod::customer::ICustomerFinderPtr                           m_CustomerFinder;
        libabbauw::domMod::manufacturerdm::IFindManufacturerPtr                   m_FindManufacturer;
        libabbauw::domMod::article::IFindArticlePtr                               m_FindArticle;
        libabbauw::domMod::transferOrder::ITransferOrderInPreparationDMPtr        m_TransferOrderInPreparation;
        libabbauw::domMod::transferOrder::ITransferOrderCreationDMPtr             m_TransferOrderCreation;
        libabbauw::domMod::purchaseordersupplierdm::IFindPurchaseOrderSupplierPtr m_PurchaseOrderSupplier;
		libabbauw::domMod::orderProtocol::IOrderProtocolPtr						  m_OrderProtocol;

        libabbauw::domMod::parameter::IOrderCheckParameterPtr                     m_OrderCheckParameter;
    };
	
}// namespace useCase
}// namespace CSC_Batches

#endif // GUARD_CSCBATCHES_USECASE_TRANSFERORDERCREATORUC_H
