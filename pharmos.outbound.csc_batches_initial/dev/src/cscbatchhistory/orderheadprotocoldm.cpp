#include "orderheadprotocoldm.h"
#include "orderprotocol_definitions.h"

#include "properties_definitions.h"

namespace domMod 
{   
	OrderHeadProtocolDM::OrderHeadProtocolDM() 
        : m_Initialized(0) 
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderHeadProtocolDM::OrderHeadProtocolDM().");
    }

	OrderHeadProtocolDM::~OrderHeadProtocolDM()
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderHeadProtocolDM::~OrderHeadProtocolDM().");
	}

	void OrderHeadProtocolDM::init(const basar::db::aspect::ConnectionRef activeConn) 
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderHeadProtocolDM::init().");		
		try 
        {		
			if(0 == m_Initialized) 
            {
				m_Accessor = basar::db::aspect::Manager::getInstance().createAccessorInstance("AccOrderHeadProtocolInstance", "OrderHeadProtocolAcc", activeConn );  
                m_BackUp = m_Accessor.getPropertyTable();
			}

			++m_Initialized;
		}
		catch(...) 
        {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "OrderHeadProtocolDM::init()."); 
			throw; 
		}
	}

	void OrderHeadProtocolDM::shutdown() 
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderHeadProtocolDM::shutdown().");
		try 
        {
			if(0==m_Initialized)
				return;
			
			--m_Initialized;
			if(0==m_Initialized) 
            {
				if(!m_Accessor.getPropertyTable().isNull())
                {
					m_Accessor.getPropertyTable().clear();
                    m_BackUp.clear();
                }

				m_Accessor.reset();
			}
		}
		catch(...) 
        {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "OrderHeadProtocolDM::shutdown()."); 
			throw;
		}
	}

    bool OrderHeadProtocolDM::saveOrderHeadProtocol()
    {
        BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderHeadProtocolDM::saveOrderHeadProtocol().");

        try 
        {
            if( m_BackUp.isEmpty() ) return false;

            basar::db::aspect::AccessorPropertyTable_YIterator yit = m_BackUp.begin();

            while( !yit.isEnd() )
            {
                m_Accessor.execute("saveOrderHeadProtocol", yit);
                ++yit;
            }

            return true;
        }
		catch(...) 
        {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "OrderHeadProtocolDM::saveOrderHeadProtocol()."); 
			throw;
		}
    }

    bool OrderHeadProtocolDM::deleteOrderHeadProtocol(basar::db::aspect::AccessorPropertyTable_YIterator yit)
    {
        BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderHeadProtocolDM::deleteOrderHeadProtocol().");

        try 
        {
            m_Accessor.beginTransaction();

            basar::db::aspect::ExecuteResultInfo resPos = m_Accessor.execute("deleteOrderHeadProtocol", yit);

            if(resPos.hasError()) 
            { 
				m_Accessor.rollbackTransaction();
				return false;
			}

            m_Accessor.commitTransaction();

            return true;
        }
		catch(...) 
        {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "OrderHeadProtocolDM::saveOrderHeadProtocol()."); 
			throw;
		}
    }

    void OrderHeadProtocolDM::setBackup(basar::db::aspect::AccessorPropertyTableRef propTab)
    {
        if( propTab.isEmpty() ) return;

        basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();

        while( !yit.isEnd() )
        {
            basar::db::aspect::AccessorPropertyTable_YIterator yitTemp = m_BackUp.insert(basar::FOR_INSERT);
            yitTemp.match(yit);

            yitTemp.setString(  "activitytype",     orderProtocol::DELETE              );
            yitTemp.setInt16(   "eventtype",        orderProtocol::NO_EVENT_TYPE       );
            yitTemp.setInt16(   "processedby",      orderProtocol::CSC_BATCHES_HISTORY ); 
            yitTemp.setString(  "changedattribute", ""                                 );
            yitTemp.setString(  "oldvalue",         ""                                 );
            yitTemp.setString(  "newvalue",         ""                                 );
            yitTemp.setInt32(   "splitfrom",        0                                  );
            yitTemp.setInt32(   "splitto",          0                                  );
            yitTemp.setInt32(   "ordernocsc",       0                                  ); 
            yitTemp.setString(  "extratext",        ""                                 );

            ++yit;
        } 
    }

    basar::db::aspect::AccessorPropertyTable_YIterator OrderHeadProtocolDM::getSearchYit()
    {
        BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::getSearchYit().");

        return getSearchYIteratorPtr()->get();
    }

	searchyiterator::SearchYIteratorPtr OrderHeadProtocolDM::getSearchYIteratorPtr()
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "CopyBatchOrdersDM::getSearchYIteratorPtr().");
        
        if( 0 == m_SearchYIterator.get() )
        {
            m_SearchYIterator = searchyiterator::SearchYIteratorPtr( 
				new searchyiterator::SearchYIterator( cscbatchhistory::properties::BRANCHNO.toDescriptionString() + cscbatchhistory::properties::MINAGEFORORDERPROTOCOL.toDescriptionString(),
				cscbatchhistory::LoggerPool::loggerDomModules )
            );
        }

        return m_SearchYIterator;
    }

} 

