#ifndef GUARD_CSC_BATCHES_USECASE_TRANSFERORDERPOSUC_H
#define GUARD_CSC_BATCHES_USECASE_TRANSFERORDERPOSUC_H

#include <libutil/irunnable.h>
#include "iusecasegetterptr.h"

#include <libbasarlogin.h>
#include <libbasarappl_systemeventmanager.h>
#include <libbasarappl_eventmacros.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>

#include "itransferorderposvcptr.h"
#include "libabbauw/itransferorderptr.h"
#include "libabbauw/icustomerfinderptr.h"
#include "libabbauw/domainmodule/orderproposal/iorderproposalptr.h"
#include "libabbauw/domainmodule/orderproposal/iorderproposalcreatorptr.h"
#include "libabbauw/domainmodule/orderprotocol/iorderprotocolptr.h"

#include "libabbauw/itransferordersplitdmptr.h"
#include "libabbauw/iparametereditptr.h"

#include "iprintprocessorptr.h"
#include "itransferorderprintptr.h"

#pragma warning (push)
#pragma warning(disable: 4512 )
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
	class TransferOrderPosUC : public libutil::useCase::IRunnable
	{
        Q_DECLARE_TR_FUNCTIONS( TransferOrderPosUC )

	public:
		TransferOrderPosUC();
		~TransferOrderPosUC();
		
		void                                              injectDM( libabbauw::domMod::transferOrder::ITransferOrderPtr );
		void                                              injectDM( libabbauw::domMod::orderProposal::IOrderProposalPtr        );
        void                                              injectDM( libabbauw::domMod::orderProposal::IOrderProposalCreatorPtr );
		void                                              injectDM( libabbauw::domMod::customer::ICustomerFinderPtr            );
		void                                              injectDM( libabbauw::domMod::orderProtocol::IOrderProtocolPtr        );
        void                                              injectDM( libabbauw::domMod::parameter::IParameterEditPtr            );

        void                                              injectColorSplitDM( libabbauw::domMod::transferOrder::ITransferOrderSplitDMPtr );
        void                                              injectNarcoticsRefrigeratedSplitDM( libabbauw::domMod::transferOrder::ITransferOrderSplitDMPtr );

        void                                              injectPrintProcessor( ::print::IPrintProcessorPtr );
		void                                              injectPrint( print::ITransferOrderPrintPtr );

        void                                              injectVC( viewConn::ITransferOrderPosVCPtr );

		void                                              injectUseCaseGetter( componentManager::IUseCaseGetterPtr );

        void                                              setIsOrderViewActive(bool isOrderviewActive);
		
		// from IRunnable interface
		void                                              run();
		
		// system event handlers
		SYSTEM_EVENT_HANDLER_DECLARATION( ShowORStatusOrder                  );
		SYSTEM_EVENT_HANDLER_DECLARATION( RemoveOrderPosRequested             );
        SYSTEM_EVENT_HANDLER_DECLARATION( PlaceOrderWishShortfallRequested    );
		SYSTEM_EVENT_HANDLER_DECLARATION( PlaceOrderWishRequested             );
		SYSTEM_EVENT_HANDLER_DECLARATION( SplitRedYellowRequested             );
        SYSTEM_EVENT_HANDLER_DECLARATION( SplitNarcoticsRefrigeratedRequested );
		SYSTEM_EVENT_HANDLER_DECLARATION( PrintOrderRequested                 );
        SYSTEM_EVENT_HANDLER_DECLARATION( CheckAllowedUserActionsRequested    );
        SYSTEM_EVENT_HANDLER_DECLARATION( TransferOrderPositionsChanged       );
		
	private:
		TransferOrderPosUC(const TransferOrderPosUC&);
		TransferOrderPosUC& operator= (const TransferOrderPosUC&);
		
		void registerEventHandlers(const bool forSave);
        void searchCustomer();
        void checkAllowedUserActions();
        void refreshView();
        void markOrderChanged();

        bool isReserveQtyDifferent();

        void createOrderProposalForPosition( const basar::Int32 posNo );
        void eraseOrderProposalForPosition ( const basar::Int32 posNo );

        void createOrderProposalShortfall(libabbauw::domMod::transferOrder::ITransferOrderPtr);                                              
		
	private:
        componentManager::IUseCaseGetterPtr                                                 m_UseCaseGetter;
		viewConn::ITransferOrderPosVCPtr                                                    m_TransferOrderPosVC;
		libabbauw::domMod::transferOrder::ITransferOrderPtr                                 m_TransferOrderDMPtr;
		libabbauw::domMod::orderProposal::IOrderProposalPtr                                 m_OrderProposal;
        libabbauw::domMod::orderProposal::IOrderProposalCreatorPtr                          m_OrderProposalCreator;
		libabbauw::domMod::customer::ICustomerFinderPtr                                     m_CustomerFinderDMPtr;

        libabbauw::domMod::transferOrder::ITransferOrderSplitDMPtr                          m_TransferOrderColorSplitDMPtr;
        libabbauw::domMod::transferOrder::ITransferOrderSplitDMPtr                          m_TransferOrderNarcoticsRefrigeratedSplitDMPtr;

		libabbauw::domMod::orderProtocol::IOrderProtocolPtr									m_OrderProtocol;

        libabbauw::domMod::parameter::IParameterEditPtr                                     m_ParameterEdit;

        ::print::IPrintProcessorPtr                                                         m_PrintProcessorPtr;
		print::ITransferOrderPrintPtr                                                       m_TransferOrderPrintPtr;

        bool                                                                                m_OrderChanged;
        bool                                                                                m_IsOrderViewActive;
	};
	
}// namespace useCase
}// namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_USECASE_TRANSFERORDERPOSUC_H
