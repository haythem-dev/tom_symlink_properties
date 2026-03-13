// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnuc.h" /Fp$(IntDir)/cmnuc.pch

#include "loggerpool.h"
#include <libutil/misc/metaclassmacro_definitions.h>
#include "updateorderproposaluc.h"
#include <libbasarlogin.h>
#include "libbasardbaspect_exceptions.h"
#include "libbasarappl_systemeventmanager.h"
#include "igetorderproposal.h"
#include "isetorderproposal.h"
#include "isetpubatchorderpos.h"
#include "igetpubatchorderpos.h"

using basar::appl::EventReturnType;

namespace CSC_Batches {
namespace useCase {
     
    UpdateOrderProposalUC::UpdateOrderProposalUC()
    {
		METHODNAME_DEF( UpdateOrderProposalUC, UpdateOrderProposalUC )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
	}
    
	UpdateOrderProposalUC::~UpdateOrderProposalUC()
	{
		METHODNAME_DEF( UpdateOrderProposalUC, ~UpdateOrderProposalUC )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
	}

	void UpdateOrderProposalUC::injectOrderProposalWriter( domMod::OrderProposalTransfer::IOrderProposalWriterPtr setorderproposal){
		METHODNAME_DEF( UpdateOrderProposalUC, injectSetOrderProposal )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );

        m_setOrderProposal = setorderproposal;
    }
	
	void UpdateOrderProposalUC::injectPuBatchOrderPositionFinder( domMod::OrderProposalTransfer::IPuBatchOrderPositionFinderPtr getorderproposal){
		METHODNAME_DEF( UpdateOrderProposalUC, injectGetPuBatchOrderPos )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );

        m_getPuBatchOrderPos = getorderproposal;
    }
   
	EventReturnType UpdateOrderProposalUC::run()
	{
	    METHODNAME_DEF( UpdateOrderProposalUC, run )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
        
        basar::appl::EventReturnStruct result;
		try
		{
			//selecting all entries from Pubatchorderpos
			basar::db::aspect::AccessorPropertyTableRef table = m_getPuBatchOrderPos->findPuBatchOrderPositions();
			//Updating the Orderproposal-table
			m_setOrderProposal->updateOrderProposals(table);
            m_setOrderProposal->updateOrderProposalCompletedFlag();
		}
		catch(basar::Exception& e)
		{
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun + e.what());
			throw;
		}

        return result;
	}
    
} // namespace 
} // namespace 
