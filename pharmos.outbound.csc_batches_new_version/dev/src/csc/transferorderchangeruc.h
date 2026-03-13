#ifndef GUARD_CSCBATCHES_USECASE_TRANSFERORDERCHANGERUC_H
#define GUARD_CSCBATCHES_USECASE_TRANSFERORDERCHANGERUC_H

#include <libutil/irunnable.h>
#include "iusecasegetterptr.h"

#include <libbasarappl_systemeventmanager.h>
#include <libbasarappl_eventmacros.h>

#include "ichangeordersvcptr.h"

#include "libabbauw/itransferorderheadcollectionchangerptr.h"
#include "libabbauw/ifindmanufacturerptr.h"
#include "libabbauw/icustomerfinderptr.h"
#include "libabbauw/domainmodule/orderprotocol/iorderprotocolptr.h"

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
		class TransferOrderChangerUC : public libutil::useCase::IRunnable
		{
		public:
			TransferOrderChangerUC();
			~TransferOrderChangerUC();

			// VCs
			void injectVC(viewConn::IChangeOrdersVCPtr);

			// DMs
			void injectDM(libabbauw::domMod::transferOrder::ITransferOrderHeadCollectionChangerPtr);
			void injectDM(libabbauw::domMod::manufacturerdm::IFindManufacturerPtr);
			void injectDM(libabbauw::domMod::customer::ICustomerFinderPtr);			
			void injectDM(libabbauw::domMod::orderProtocol::IOrderProtocolPtr);
			
			// Usecase Controller
			void injectUseCaseGetter(componentManager::IUseCaseGetterPtr);

			// from IRunnable interface
			void run();

			// event handlers
			SYSTEM_EVENT_HANDLER_DECLARATION(ApplyChangesRequested);
			SYSTEM_EVENT_HANDLER_DECLARATION(SaveOrderRequested);
			SYSTEM_EVENT_HANDLER_DECLARATION(FindPharmacyRequested);
			SYSTEM_EVENT_HANDLER_DECLARATION(UpdatePharmacyRequested);
			SYSTEM_EVENT_HANDLER_DECLARATION(FindManufacturerRequested);
			SYSTEM_EVENT_HANDLER_DECLARATION(UpdateManufacturerRequested);
			SYSTEM_EVENT_HANDLER_DECLARATION(CheckPharmacyNoRequested);
			SYSTEM_EVENT_HANDLER_DECLARATION(CheckManufacturerNoRequested);
			SYSTEM_EVENT_HANDLER_DECLARATION(OrderDiscountRequested);

		private:
			TransferOrderChangerUC(const TransferOrderChangerUC&);
			TransferOrderChangerUC& operator= (const TransferOrderChangerUC&);

			void registerEventHandlers(const bool forSave);
			bool searchBatchOrders();

		private:
			componentManager::IUseCaseGetterPtr                                      m_UseCaseGetterPtr;

			viewConn::IChangeOrdersVCPtr                                             m_ChangeOrdersVC;

			libabbauw::domMod::transferOrder::ITransferOrderHeadCollectionChangerPtr m_TransferOrderHeadCollectionChangerPtr;
			libabbauw::domMod::customer::ICustomerFinderPtr                          m_CustomerFinderPtr;
			libabbauw::domMod::manufacturerdm::IFindManufacturerPtr                  m_FindManufacturerPtr;
			libabbauw::domMod::orderProtocol::IOrderProtocolPtr						 m_OrderProtocol;
		};

	} // end namespace useCase
} // end namespace CSC_Batches
#endif // GUARD_CSCBATCHES_USECASE_TRANSFERORDERCHANGERUC_H
