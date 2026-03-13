#include "cmndm.h"

#include <libbasar_definitions.h>
#include <libbasardbaspect.h>
#include <libbasarcmnutil_exceptions.h>
#include <libbasardbaspect_exceptions.h>
#include <libbasardbsql_exceptions.h>

#include "orderproposaldm.h"
#include "orderproposaldm_definitions.h"
#include "properties_definitions.h"
#include <libutil/misc/metaclassmacro_definitions.h>
#include "loggerpool.h"
#include "orderproposalacc.h"
#include "resultinfoexception.h"

#include <libutil/version.h>
#include <libutil/accessor/throw_accessor_execution_exception_macro_definition.h>

namespace domMod
{
    namespace OrderProposalTransfer
    {
        using basar::db::aspect::AccessorPropertyTable_YIterator;
        using basar::db::aspect::AccessorPropertyTable_XIterator;
        using basar::db::aspect::AccessorPropertyTableRef;
        using basar::db::aspect::ExecuteResultInfo;
        using basar::db::aspect::AccessorInstanceRef;
    }
}

namespace domMod {
namespace OrderProposalTransfer {

    OrderProposalDM::OrderProposalDM( const basar::db::aspect::ConnectionRef conn )
		: m_Connection( conn )
	{
        METHODNAME_DEF( OrderProposalDM, OrderProposalDM )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
		acc_orderproposal::SelectOrderProposals dummy;
	}

	OrderProposalDM::~OrderProposalDM()
	{
        METHODNAME_DEF( OrderProposalDM, ~OrderProposalDM )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
	}

    void OrderProposalDM::doInit(const OrderProposalInitParams&  params )
    {
	    METHODNAME_DEF( OrderProposalDM, init )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
		
		m_InitParams = params;

		try
		{
			m_Accessor = basar::db::aspect::Manager::getInstance().createAccessorInstance(orderproposaldm::lit::ACC_ORDERPROPOSAL_INST, orderproposaldm::lit::ACC_ORDERPROPOSAL, m_Connection);
		}
		catch (basar::Exception& e)
		{
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun);
			throw e;
		}
    }

    void OrderProposalDM::doShutdown()
    {
	    METHODNAME_DEF( OrderProposalDM, doShutdown )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );

		try 
		{			
			if(!m_Accessor.getPropertyTable().isNull())
			{
				m_Accessor.getPropertyTable().clear();
			}
			m_Accessor.reset();
		}
		catch(basar::Exception& e) {
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
			throw e;
		}
    }

	basar::db::aspect::AccessorPropertyTableRef OrderProposalDM::getTable() const
	{
		METHODNAME_DEF( OrderProposalDM, getTable )
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
	}
	
	basar::db::aspect::AccessorPropertyTableRef OrderProposalDM::findOrderProposals()
	{
		METHODNAME_DEF( OrderProposalDM, getData )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
		
		try 
		{
			m_Accessor.getPropertyTable().clear();
			basar::db::aspect::AccessorPropertyTable_YIterator yit = m_Accessor.getPropertyTable().insert( basar::FOR_CLEAN );
			yit.setDateTime( properties::TRANSFER_TIME, basar::DateTime(basar::Int32(99991231),basar::Int32(235959000)) );

			basar::db::aspect::ExecuteResultInfo executeResultInfo = m_Accessor.execute( orderproposaldm::lit::SELECT_ORDER_PROPOSALS, yit );
			
			if(executeResultInfo.hasError())
			{
				throw libabbauw::ResultInfoException(basar::ExceptInfo()); //own exception
			}

			return m_Accessor.getPropertyTable();
		}
		catch(basar::Exception& e) 
		{
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun + e.what()); 
			throw e;
		}
	}
	
	void OrderProposalDM::updateOrderProposals( basar::db::aspect::AccessorPropertyTableRef ref )
	{
		METHODNAME_DEF( OrderProposalDM, updateFromPuBatchOrderPos )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );

		try {
			//basar::db::aspect::ExecuteResultInfo executeResultInfo;

            for( AccessorPropertyTable_YIterator it = ref.begin(); !it.isEnd(); ++it ) 
			{
				basar::db::aspect::ExecuteResultInfo executeResultInfo;
				try
				{
					executeResultInfo = m_Accessor.execute( orderproposaldm::lit::UPDATE_ORDER_PROPOSALS, it );
                    if( executeResultInfo.hasError() )
                    {
                        THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, orderproposaldm::lit::UPDATE_ORDER_PROPOSALS, executeResultInfo );
                    }
				}
				catch(basar::Exception& e)
				{
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun + e.what());
                }
			}
		}
		catch(basar::Exception&) 
		{
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
			throw;
		}
	}
	
	void OrderProposalDM::updateTransferTime( basar::db::aspect::AccessorPropertyTable_YIterator ref )
	{
		METHODNAME_DEF( OrderProposalDM, updateTransferTime )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );
		
		try 
		{
			basar::db::aspect::ExecuteResultInfo executeResultInfo;

			executeResultInfo = m_Accessor.execute( orderproposaldm::lit::UPDATE_TRANSFER_DATE, ref );
			if(executeResultInfo.hasError())
				throw libabbauw::ResultInfoException(basar::ExceptInfo()); 
		}
		catch(basar::Exception& e)
		{
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun+ e.what()); 
			throw e;
		}
	}

    void OrderProposalDM::updateOrderProposalCompletedFlag()
    {
        METHODNAME_DEF( OrderProposalDM, updateOrderProposalCompletedFlag )
        BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun );

        basar::db::aspect::ExecuteResultInfo executeResultInfo;
		try 
		{
			executeResultInfo = m_Accessor.execute( orderproposaldm::lit::UPDATE_ORDER_PROPOSAL_COMPLETED_FLAG );
            if( executeResultInfo.hasError() )
            {
                THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, orderproposaldm::lit::UPDATE_ORDER_PROPOSAL_COMPLETED_FLAG, executeResultInfo );
            }
		}
		catch(basar::Exception&) 
		{
			BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun); 
			throw;
		}
    }

	const libutil::misc::ClassInfo & OrderProposalDM::getClassInfo()
	{    
		static const libutil::misc::ClassInfo info( "OrderProposalDM", libutil::misc::Version( 1, 0, 0, 0 ) );    
		return info;
	}

	void OrderProposalDM::beginTransaction()
	{
		m_Connection.beginTransaction();
	}
	
	void OrderProposalDM::commitTransaction()
	{
		m_Connection.commitTransaction();
	}

	void OrderProposalDM::rollbackTransaction()
	{
		m_Connection.rollbackTransaction();
	}
	
	bool OrderProposalDM::isInTransaction() const
	{
		return m_Connection.isInTransaction();
	}

} 
} 
