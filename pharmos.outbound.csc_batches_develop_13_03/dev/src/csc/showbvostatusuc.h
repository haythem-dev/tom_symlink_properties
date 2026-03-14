#ifndef GUARD_CSCBATCHES_USECASE_SHOWBVOSTATUSUC_H
#define GUARD_CSCBATCHES_USECASE_SHOWBVOSTATUSUC_H

#include <libutil/irunnable.h>
#include "icsvexportgetterptr.h"

#include "ibvostatusvcptr.h"
#include "libabbauw/domainmodule/orderproposal/iorderproposalcollectionfinderptr.h"
#include "libabbauw/domainmodule/orderproposal/iorderproposalptr.h"
#include "libabbauw/domainmodule/orderprotocol/iorderprotocolptr.h"

#include <libutil/session.h>

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
    class ShowBVOStatusUC : public libutil::useCase::IRunnable
    {
    public:
        ShowBVOStatusUC();
        ~ShowBVOStatusUC();
        
        SYSTEM_EVENT_HANDLER_DECLARATION( FindOrderProposalsRequested );
        SYSTEM_EVENT_HANDLER_DECLARATION( CancelOrderProposal );
		SYSTEM_EVENT_HANDLER_DECLARATION( ExportCSVRequested );

        void setSession( libutil::infrastructure::session::ISessionPtr );

		void injectCSVExportGetter( componentManager::ICSVExportGetterPtr ); 

        void injectVC( viewConn::IBVOStatusVCPtr );

        void injectDM( libabbauw::domMod::orderProposal::IOrderProposalCollectionFinderPtr );
        void injectDM( libabbauw::domMod::orderProposal::IOrderProposalPtr );
        void injectDM( libabbauw::domMod::orderProtocol::IOrderProtocolPtr );

        void init();
        void init( basar::Int32 orderNo, bool isOrderViewActive );

        // from IRunnable interface
        void run();
        
    private:
        ShowBVOStatusUC( const ShowBVOStatusUC& );
        ShowBVOStatusUC& operator = ( const ShowBVOStatusUC& );
        
        void registerEventHandlers(const bool forSave);
		void refresh();

    private:
        libutil::infrastructure::session::ISessionPtr                       m_Session;

		componentManager::ICSVExportGetterPtr								m_CSVExportGetter;

        viewConn::IBVOStatusVCPtr                                           m_BVOStatusVC;
        libabbauw::domMod::orderProposal::IOrderProposalCollectionFinderPtr m_OrderProposalCollectionFinderPtr;
        libabbauw::domMod::orderProposal::IOrderProposalPtr                 m_OrderProposalPtr;
        libabbauw::domMod::orderProtocol::IOrderProtocolPtr                 m_OrderProtocol;

        bool                                                                m_OrdersChanged;
        bool                                                                m_IsOrderViewActive;
    };
    
} // end namespace useCase
} // end namespace CSC_Batches
#endif // GUARD_CSCBATCHES_USECASE_SHOWBVOSTATUSUC_H
