#include "archivedm.h"
#include <libbasardbaspect.h>
#include "loggerpool.h"
#include "searchyiterator.h"

namespace domMod
{
	ArchiveDM::ArchiveDM() : m_Initialized(0) 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::ArchiveDM().");
	}

	ArchiveDM::~ArchiveDM()
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::~ArchiveDM().");
	}

	void ArchiveDM::init(const basar::db::aspect::ConnectionRef activeConn) 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::init().");	

		try 
		{		
			if(0 == m_Initialized) 
			{
				m_ArchiveAccessor = basar::db::aspect::Manager::getInstance().createAccessorInstance("AccArchiveInstance", "ArchiveAcc", activeConn );  
			}

			++m_Initialized;
		}
		catch(...) 
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::init()."); 
			throw; 
		}
	}

	void ArchiveDM::shutdown() 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::shutdown().");
		
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
				if (!m_ArchiveAccessor.getPropertyTable().isNull())
				{
					m_ArchiveAccessor.getPropertyTable().clear();
				}

				m_ArchiveAccessor.reset();
			}
		}
		catch(...) 
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::shutdown()."); 
			throw;
		}
	}

	bool ArchiveDM::deleteOrderHeadArchive(const basar::db::aspect::AccessorPropertyTable_YIterator yit) 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::deleteOrderHeadArchive().");

		try 
		{		
			basar::db::aspect::ExecuteResultInfo result = m_ArchiveAccessor.execute("deleteBatchOrderHeadsArchive", yit);
			
			if (result.hasError()) 
			{  
				BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::deleteOrderHeadArchive() result:" + result.getError()); 
				return false;
			}

			return true;
		}
		catch(...) 
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::copyBatchOrders()."); 
			throw; 
		}
	}
	
	bool ArchiveDM::deleteOrderPosArchive(const basar::db::aspect::AccessorPropertyTable_YIterator yit) 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::deleteOrderPosArchive().");

		try
		{
			basar::db::aspect::ExecuteResultInfo result = m_ArchiveAccessor.execute("deleteBatchOrderPosArchive", yit);
			
			if (result.hasError()) 
			{  
				BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::deleteBatchOrderPosArchive() result:" + result.getError()); 
				return false;
			}

			return true;
		}
		catch(...)
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::deleteOrderPosArchive()."); 
			throw; 
		}
	}

	bool ArchiveDM::deleteOrderProposalArchive(const basar::db::aspect::AccessorPropertyTable_YIterator yit) 
	{
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::deleteOrderProposalArchive().");

		try
		{
			basar::db::aspect::ExecuteResultInfo result = m_ArchiveAccessor.execute("deleteBatchOrderProposalsArchive", yit);
			
			if (result.hasError()) 
			{  
				BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::deleteOrderProposalArchive() result:" + result.getError()); 
				return false;
			}

			return true;
		}
		catch(...)
		{
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::deleteOrderProposalArchive()."); 
			throw; 
		}
	}

	basar::db::aspect::AccessorPropertyTable_YIterator ArchiveDM::getSearchYit()
    {
        BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::getSearchYit().");
        return getSearchYiterator()->get();
    }

	searchyiterator::SearchYIteratorPtr ArchiveDM::getSearchYiterator()
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "ArchiveDM::getSearchYiterator().");

        if (0 == m_SearchYit.get())
		{
            m_SearchYit = searchyiterator::SearchYIteratorPtr( 
			    new searchyiterator::SearchYIterator( "int16 branchno; int32 deletedateforarchive",
				cscbatchhistory::LoggerPool::loggerDomModules)
            );
        }

        return m_SearchYit;
    }

}
