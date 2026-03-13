#ifndef GUARD_CSC_BATCHES_USECASE_UPDATE_ORDER_PROPOSAL_UC_H
#define GUARD_CSC_BATCHES_USECASE_UPDATE_ORDER_PROPOSAL_UC_H

#include <libbasardbaspect.h>
#include "igetpubatchorderposptr.h"
#include "isetorderproposalptr.h"
#include "libbasarappl_systemeventmanager.h"

namespace basar {
    namespace appl {
        class IEventSource;
    }
}


namespace basar {
    namespace db {
        namespace aspect {
            class AccessorPropertyTable_YIterator;
            class Manager;
        }
    }
}


namespace CSC_Batches {
namespace useCase {    

    class UpdateOrderProposalUC
	{        
        public:
            UpdateOrderProposalUC(  );
            ~UpdateOrderProposalUC();
            
			void injectPuBatchOrderPositionFinder( domMod::OrderProposalTransfer::IPuBatchOrderPositionFinderPtr );
			void injectOrderProposalWriter( domMod::OrderProposalTransfer::IOrderProposalWriterPtr );

            basar::appl::EventReturnType run();
            
        private:
            UpdateOrderProposalUC(const UpdateOrderProposalUC& updateOrderProposalUC);
            UpdateOrderProposalUC& operator= (const UpdateOrderProposalUC& updateOrderProposalUC );
            
			domMod::OrderProposalTransfer::IPuBatchOrderPositionFinderPtr		m_getPuBatchOrderPos;
			domMod::OrderProposalTransfer::IOrderProposalWriterPtr				m_setOrderProposal;
			
    };
} // namespace 
} // namespace 

#endif 
