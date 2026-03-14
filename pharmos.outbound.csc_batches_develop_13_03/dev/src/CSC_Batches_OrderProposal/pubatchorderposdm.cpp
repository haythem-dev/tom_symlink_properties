#include "cmndm.h"

#include <libbasar_definitions.h>
#include <libbasardbaspect.h>
#include <libbasarcmnutil_exceptions.h>
#include <libbasardbaspect_exceptions.h>
#include <libbasardbsql_exceptions.h>

#include "pubatchorderposdm.h"
#include "pubatchorderposacc.h"
#include "pubatchorderposdm_definitions.h"
#include "properties_definitions.h"
#include <libutil/misc/metaclassmacro_definitions.h>
#include "loggerpool.h"

#include "resultinfoexception.h"
#include <libutil/version.h>

namespace domMod
{
    namespace OrderProposalTransfer
    {
        using basar::db::aspect::AccessorPropertyTable_YIterator;
        using basar::db::aspect::AccessorPropertyTableRef;
        using basar::db::aspect::ExecuteResultInfo;
        using basar::db::aspect::AccessorInstanceRef;
    }
}

namespace domMod {
namespace OrderProposalTransfer {

    PuBatchOrderPosDM::PuBatchOrderPosDM( const basar::db::aspect::ConnectionRef conn ) : m_Connection( conn )
	{
        METHODNAME_DEF( PuBatchOrderPosDM, PuBatchOrderPosDM )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
		acc_pubatchorderpos::SelectPuBatchOrderPositions dummy;
	}

	PuBatchOrderPosDM::~PuBatchOrderPosDM()
	{
        METHODNAME_DEF( PuBatchOrderPosDM, ~PuBatchOrderPosDM )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
	}

    void PuBatchOrderPosDM::doInit( const PuBatchOrderPosInitParams& params)
    {
	    METHODNAME_DEF( PuBatchOrderPosDM, init )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
		
		m_InitParams = params;

		try
		{
			m_Accessor = basar::db::aspect::Manager::getInstance().createAccessorInstance(pubatchorderposdm::lit::ACC_PUBATCHORDERPOS_INST, pubatchorderposdm::lit::ACC_PUBATCHORDERPOS, m_Connection);
		}
		catch (basar::Exception& e)
		{
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun + e.what()); 
			throw;
		}
		catch(...){
			throw;
		}
    }

    void PuBatchOrderPosDM::doShutdown()
    {
	    METHODNAME_DEF( PuBatchOrderPosDM, doShutdown )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );

		try
		{
			if(!m_Accessor.getPropertyTable().isNull())
			{
				m_Accessor.getPropertyTable().clear();
			}

			m_Accessor.reset();
		}
		catch(basar::Exception& e)
		{
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun + e.what()); 
			throw e;
		}
		catch(...)
		{
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
			throw;
		}
    }

	basar::db::aspect::AccessorPropertyTableRef PuBatchOrderPosDM::getTable() const
	{
		METHODNAME_DEF( PuBatchOrderPosDM, getTable )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
		
		try 
		{
			return m_Accessor.getPropertyTable();
		}
		catch(basar::Exception& e) 
		{
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun + e.what()); 
			throw e;
		}
		catch(...) 
		{
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
			throw;
		}
	}

	void PuBatchOrderPosDM::savePuBatchOrderPositions( basar::db::aspect::AccessorPropertyTable_YIterator it )
	{
		METHODNAME_DEF( PuBatchOrderPosDM, addFromOrderProposal )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
		
		try
		{
			basar::db::aspect::ExecuteResultInfo executeResultInfo;

				if(it.getInt32(properties::ORDER_PROPOSAL_QTY)==0)
				{
					throw libabbauw::ResultInfoException(basar::ExceptInfo()); 
				}
				else
				{
					executeResultInfo = m_Accessor.execute( pubatchorderposdm::lit::INSERT_PU_BATCH_ORDER_POSITIONS, it);
					if(executeResultInfo.getError() == basar::db::aspect::SUCCESS) //true if no exception was thrown
					{ 
						it.setDateTime(properties::TRANSFER_TIME, basar::DateTime::getCurrent());
						it.setPropertyState(properties::TRANSFER_TIME, basar::SS_UPDATE);
					}
					else
					{
						throw libabbauw::ResultInfoException(basar::ExceptInfo()); 
					}
				}
		}
		catch(basar::Exception& e) 
		{
			printf(e.what().c_str());
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun + e.what()); 
			throw;
		}
	}

	const basar::db::aspect::AccessorPropertyTableRef PuBatchOrderPosDM::findPuBatchOrderPositions()
	{
		METHODNAME_DEF( PuBatchOrderPosDM, getData )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );

		try
		{
			m_Accessor.getPropertyTable().clear();
			basar::db::aspect::ExecuteResultInfo executeResultInfo = m_Accessor.execute(pubatchorderposdm::lit::SELECT_PU_BATCH_ORDER_POSITIONS);

			if(executeResultInfo.hasError())
			{
				throw libabbauw::ResultInfoException(basar::ExceptInfo()); //own exeception
			}

			return m_Accessor.getPropertyTable();
		}
		catch(basar::Exception& e) 
		{
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun + e.what()); 
			throw;
		}
	}
	const libutil::misc::ClassInfo & PuBatchOrderPosDM::getClassInfo()
	{    
		static const libutil::misc::ClassInfo info( "PuBatchOrderPosDM", libutil::misc::Version( 1, 0, 0, 0 ) );    
		return info;
	}

	void PuBatchOrderPosDM::beginTransaction()
	{
		m_Connection.beginTransaction();
	}
	
	void PuBatchOrderPosDM::commitTransaction()
	{
		m_Connection.commitTransaction();
	}

	void PuBatchOrderPosDM::rollbackTransaction()
	{
		m_Connection.rollbackTransaction();
	}

	bool PuBatchOrderPosDM::isInTransaction() const
	{
		return m_Connection.isInTransaction();
	}

} // namespace 
} // namespace 
