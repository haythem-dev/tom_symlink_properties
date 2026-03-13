#ifndef GUARD_CSCBATCHESHISTORYUC_H
#define GUARD_CSCBATCHESHISTORYUC_H

#include "archivedm.h"
#include "usecasecontroller.h"
#include "copybatchordersdm.h"
#include "orderproposaldm.h"
#include "orderheadprotocoldm.h"
#include "orderposprotocoldm.h"

namespace cscbatchhistory {
namespace useCase {    

	class CscBatchesHistoryUC : public UseCaseController 
	{
	public:

		CscBatchesHistoryUC();
		virtual ~CscBatchesHistoryUC();
		     
		//**************************************************************************************************
		/*! \brief main routine for use case controller
			\param rSource source where event is thrown
			\param yitSearch search criteria from source \n
			basar::Int16 branchno has to be in yit
			basar::Int32 minimumagefororderproposal has to be in yit
			\throw no-throw 
		*/
		//**************************************************************************************************
		virtual basar::appl::EventReturnType run(basar::db::aspect::AccessorPropertyTable_YIterator yitSearch);

	protected:

	private:
		CscBatchesHistoryUC(const CscBatchesHistoryUC&);
		CscBatchesHistoryUC& operator=(const CscBatchesHistoryUC&);
	
		domMod::CopyBatchOrdersDM   m_CopyBatchOrdersDM;
		domMod::OrderProposalDM     m_OrderProposalDM; 
		domMod::ArchiveDM			m_ArchiveDM;

        domMod::OrderHeadProtocolDM m_OrderHeadProtocolDM;
        domMod::OrderPosProtocolDM  m_OrderPosProtocolDM;
	};
}
}

#endif
