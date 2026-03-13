// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnuc.h" /Fp$(IntDir)/cmnuc.pch

#include "loggerpool.h"
#include "addtopubatchorderposuc.h"
#include <libbasarlogin.h>
#include <libbasardbaspect.h>
#include "libbasardbaspect_exceptions.h"
#include "libbasarappl_systemeventmanager.h"
#include "igetorderproposal.h"
#include "isetorderproposal.h"
#include "isetpubatchorderpos.h"
#include <libutil/misc/metaclassmacro_definitions.h>
//#include <metaclassmacro_definitions.h>

#include "properties_definitions.h"

using basar::db::aspect::AccessorPropertyTableRef;
using basar::appl::EventReturnType;

namespace CSC_Batches {
namespace useCase {
     
    AddToPuBatchOrderPosUC::AddToPuBatchOrderPosUC()
    {
		METHODNAME_DEF( AddToPuBatchOrderPosUC, AddToPuBatchOrderPosUC )
		BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
	}
  
	AddToPuBatchOrderPosUC::~AddToPuBatchOrderPosUC()
	{
		METHODNAME_DEF( AddToPuBatchOrderPosUC, ~UpdateOrderProposalUC )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
	}

	void AddToPuBatchOrderPosUC::injectOrderProposalFinder( domMod::OrderProposalTransfer::IOrderProposalFinderPtr getorderproposal)
	{
		METHODNAME_DEF( AddToPuBatchOrderPosUC, injectGetOrderProposal )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );

		m_getOrderProposal = getorderproposal;
	}

	void AddToPuBatchOrderPosUC::injectOrderProposalWriter( domMod::OrderProposalTransfer::IOrderProposalWriterPtr setorderproposal)
	{
		METHODNAME_DEF( AddToPuBatchOrderPosUC, injectSetOrderProposal )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );

		m_setOrderProposal = setorderproposal;
	}

	void AddToPuBatchOrderPosUC::injectPuBatchOrderPositionWriter( domMod::OrderProposalTransfer::IPuBatchOrderPositionWriterPtr setpubatchorderpos )
	{
		METHODNAME_DEF( AddToPuBatchOrderPosUC, injectSetPuBatchOrderPos )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );

		m_setPuBatchOrderPos = setpubatchorderpos;
	}

	EventReturnType AddToPuBatchOrderPosUC::run()
	{
	    METHODNAME_DEF( AddToPuBatchOrderPosUC, run )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
		
		try
		{
			//Fetching the Information of Orderproposal
			basar::db::aspect::AccessorPropertyTableRef table = m_getOrderProposal->findOrderProposals();
			basar::db::aspect::AccessorPropertyTable_YIterator yit = table.begin();
			//Iterating over every row of the propertytable
			while(!yit.isEnd())
			{
				try
				{
					//using transaction to be sure just to write into the pubatchorderpos when the corresponding update entry in the orderproposal is successfully done
					m_setPuBatchOrderPos->beginTransaction();
					m_setPuBatchOrderPos->savePuBatchOrderPositions(yit);
					m_setOrderProposal->updateTransferTime(yit);
					m_setPuBatchOrderPos->commitTransaction();
				}
				catch(basar::Exception& e)
				{
					//if transfer fails make sure everything is reset
					if(m_setPuBatchOrderPos->isInTransaction())
					{
						m_setPuBatchOrderPos->rollbackTransaction();
					}

					try
					{
						yit.setDateTime("transfertime",basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)));
						m_setOrderProposal->updateTransferTime(yit);
					}
					catch(basar::Exception& e)
					{
						BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun + e.what());
						throw;
					}
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun + e.what());
					throw;
				}
				++yit;
			}
		}
		catch(basar::Exception& e)
		{
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun + e.what()); 
			throw;
		}
        
        basar::appl::EventReturnStruct result;

        return result;
	}

} // namespace 
} // namespace 
