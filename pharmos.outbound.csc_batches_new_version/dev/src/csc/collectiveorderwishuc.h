#ifndef GUARD_CSCBATCHES_USECASE_COLLECTIVEORDERWISHUC_H
#define GUARD_CSCBATCHES_USECASE_COLLECTIVEORDERWISHUC_H

#include <libutil/irunnable.h>

#include "icollectiveorderwishconfirmationvcptr.h"

#include "libabbauw/domainmodule/orderproposal/iorderproposalcollectionptr.h"
#include "libabbauw/domainmodule/orderprotocol/iorderprotocolptr.h"

#include <libbasarappl_eventmacros.h>
#include <libbasarappl_systemeventmanager.h>

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
	class CollectiveOrderWishUC : public libutil::useCase::IRunnable
	{
	public:
		CollectiveOrderWishUC();
		~CollectiveOrderWishUC();
		
        SYSTEM_EVENT_HANDLER_DECLARATION( CreateOrderWishesRequested );
        SYSTEM_EVENT_HANDLER_DECLARATION( RemoveOrderWishRequested );

		void injectVC( viewConn::ICollectiveOrderWishConfirmationVCPtr );
		void injectDM( libabbauw::domMod::orderProposal::IOrderProposalCollectionPtr );
        void injectDM( libabbauw::domMod::orderProtocol::IOrderProtocolPtr );

        void setIsOverViewActive(bool isOrderViewActive);

        // from IRunnable interface
		void run();
		
	private:
		CollectiveOrderWishUC( const CollectiveOrderWishUC& );
		CollectiveOrderWishUC& operator = ( const CollectiveOrderWishUC& );
		
		void registerEventHandlers(const bool forSave);
		
    private:
		viewConn::ICollectiveOrderWishConfirmationVCPtr               m_CollectiveOrderWishConfirmationVC;
		libabbauw::domMod::orderProposal::IOrderProposalCollectionPtr m_OrderProposalCollection;
        libabbauw::domMod::orderProtocol::IOrderProtocolPtr           m_OrderProtocol;

        bool                                                          m_IsOrderViewActive;
	};
	
} // end namespace useCase
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_USECASE_COLLECTIVEORDERWISHUC_H
