#include "copybatchordersdm.h"
#include <libbasardbaspect.h>
#include "loggerpool.h"
#include "searchyiterator.h"

namespace domMod
{
	CopyBatchOrdersDM::CopyBatchOrdersDM() : m_Initialized(0) 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::CopyBatchOrdersDM().");
	}

	CopyBatchOrdersDM::~CopyBatchOrdersDM()
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::~CopyBatchOrdersDM().");
	}

	void CopyBatchOrdersDM::init(const basar::db::aspect::ConnectionRef activeConn) 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::init().");	

		try 
		{		
			if(0 == m_Initialized) 
			{
				m_CopyBatchOrdersACC = basar::db::aspect::Manager::getInstance().createAccessorInstance("AccCopyBatchOrderInstance", "CopyBatchOrdersAcc", activeConn );  
			}

			++m_Initialized;
		}
		catch(...) 
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::init()."); 
			throw; 
		}
	}

	void CopyBatchOrdersDM::shutdown() 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::shutdown().");
		
		try
		{		
			// release accessor instance, if instance was created WITH storing (default: without) in instance list
			// basar::db::aspect::Manager::getInstance().releaseAccessorInstance("AccInstance");
			
			if (0 == m_Initialized)
			{
				return;
			}
			
			--m_Initialized;
			if (0 == m_Initialized) 
			{
				if (!m_CopyBatchOrdersACC.getPropertyTable().isNull())
				{
					m_CopyBatchOrdersACC.getPropertyTable().clear();
				}

				m_CopyBatchOrdersACC.reset();
			}
		}
		catch(...)
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::shutdown()."); 
			throw;
		}
	}

	bool CopyBatchOrdersDM::copyBatchOrders(const basar::db::aspect::AccessorPropertyTable_YIterator yit) 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::copyBatchOrders().");

		try 
		{		
			m_CopyBatchOrdersACC.beginTransaction();

			basar::db::aspect::ExecuteResultInfo res;
			
			// PAY ATTENTION HERE! It is absolutely necessary to copy (and delete) the positions before copying and deleting the order heads!
			//--- copy order positions:
			basar::db::aspect::ExecuteResultInfo resPos = m_CopyBatchOrdersACC.execute("saveArchiveBatchOrderPos", yit);
			
			if(resPos.hasError()) 
			{  // error
				m_CopyBatchOrdersACC.rollbackTransaction();
				return false;
			}

 			res = m_CopyBatchOrdersACC.execute("deleteBatchOrderPos", yit);
			if(res.hasError() != resPos.hasError()) 
			{
				m_CopyBatchOrdersACC.rollbackTransaction();
				return false;
			}

			//--- copy order heads:
			basar::db::aspect::ExecuteResultInfo resHeads = m_CopyBatchOrdersACC.execute("saveArchiveBatchOrderHeads", yit);
			if(resHeads.hasError()) 
			{  // error
				m_CopyBatchOrdersACC.rollbackTransaction();
				return false;
			}

			if(0==resHeads.getAffectedRows()) 
			{ // nothing else to do
				if(resPos.getAffectedRows()>0) 
				{
					m_CopyBatchOrdersACC.rollbackTransaction();
					return false;
				}
				else 
				{
					m_CopyBatchOrdersACC.commitTransaction();
					return true;
				}
			}

			res = m_CopyBatchOrdersACC.execute("deleteBatchOrderHeads", yit);
			if(res.hasError() != resHeads.hasError()) 
			{
				m_CopyBatchOrdersACC.rollbackTransaction();
				return false;
			}

			// everything was done:
			m_CopyBatchOrdersACC.commitTransaction();
			return true;
		}
		catch(...) 
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::copyBatchOrders()."); 
			throw; 
		}
	}

    basar::db::aspect::AccessorPropertyTableRef CopyBatchOrdersDM::getOrderHeadsWhichShouldBeArchived(const basar::db::aspect::AccessorPropertyTable_YIterator yit)
    {
        BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::getOrderHeadsWhichShouldBeArchived().");

        try
        {
            m_CopyBatchOrdersACC.execute("selectBatchOrderHead", yit);
            return m_CopyBatchOrdersACC.getPropertyTable();
        }
        catch(...)
        {
            BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::getOrderHeadsWhichShouldBeArchived()."); 
			throw; 
        }
    }

    basar::db::aspect::AccessorPropertyTableRef CopyBatchOrdersDM::getOrderPositionsWhichShouldBeArchived(const basar::db::aspect::AccessorPropertyTable_YIterator yit)
    {
        BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::getOrderPositionsWhichShouldBeArchived().");

        try
        {
            m_CopyBatchOrdersACC.execute("selectBatchOrderPos", yit);
            return m_CopyBatchOrdersACC.getPropertyTable();
        }
        catch(...)
        {
            BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::getOrderPositionsWhichShouldBeArchived()."); 
			throw; 
        }
    }
	
	basar::db::aspect::AccessorPropertyTable_YIterator CopyBatchOrdersDM::getSearchYit()
    {
        BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::getSearchYit().");

        return getSearchYIteratorPtr()->get();
    }

	searchyiterator::SearchYIteratorPtr CopyBatchOrdersDM::getSearchYIteratorPtr()
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::getSearchYIteratorPtr().");

        if( 0 == m_SearchYIterator.get() )
		{
            m_SearchYIterator = searchyiterator::SearchYIteratorPtr( 
			    new searchyiterator::SearchYIterator( "int16 branchno; int32 lessthanorderdate",
				cscbatchhistory::LoggerPool::loggerDomModules
                                                    )
            );
        }

        return m_SearchYIterator;
    }

}
