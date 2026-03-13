#ifndef GUARD_CSC_BATCHES_USECASE_ADD_TO_PU_BATCH_ORDER_POS_UC_H
#define GUARD_CSC_BATCHES_USECASE_ADD_TO_PU_BATCH_ORDER_POS_UC_H

#include "igetorderproposalptr.h"
#include "isetpubatchorderposptr.h"
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
            class AccessorPropertyTableRef;
            class Manager;
        }
    }
}

namespace CSC_Batches {
namespace useCase {    

    class AddToPuBatchOrderPosUC
	{        
        public:
            AddToPuBatchOrderPosUC();
            ~AddToPuBatchOrderPosUC();
            
			void injectOrderProposalFinder( domMod::OrderProposalTransfer::IOrderProposalFinderPtr );
			void injectOrderProposalWriter( domMod::OrderProposalTransfer::IOrderProposalWriterPtr );
			void injectPuBatchOrderPositionWriter( domMod::OrderProposalTransfer::IPuBatchOrderPositionWriterPtr );

            basar::appl::EventReturnType run();
            
        private:
            AddToPuBatchOrderPosUC(const AddToPuBatchOrderPosUC& addToPuBatchOrderPos);
            AddToPuBatchOrderPosUC& operator= (const AddToPuBatchOrderPosUC& addToPuBatchOrderPos);
           
			domMod::OrderProposalTransfer::IOrderProposalWriterPtr				m_setOrderProposal;
			domMod::OrderProposalTransfer::IOrderProposalFinderPtr				m_getOrderProposal;
			domMod::OrderProposalTransfer::IPuBatchOrderPositionWriterPtr		m_setPuBatchOrderPos;
    };
} // namespace 
} // namespace 

#endif // GUARD_OBST_USECASE_ARTICLE_SEARCH_UC_H
