#include "orderposprotocoldm.h"
#include "orderprotocol_definitions.h"

#include "properties_definitions.h"

namespace domMod 
{
	OrderPosProtocolDM::OrderPosProtocolDM() 
        : m_Initialized(0) 
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderPosProtocolDM::OrderPosProtocolDM().");
	}

	OrderPosProtocolDM::~OrderPosProtocolDM()
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderPosProtocolDM::~OrderPosProtocolDM().");
	}

	void OrderPosProtocolDM::init(const basar::db::aspect::ConnectionRef activeConn) 
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderPosProtocolDM::init().");		
		try 
        {		
			if(0 == m_Initialized) 
            {
				m_Accessor = basar::db::aspect::Manager::getInstance().createAccessorInstance("AccOrderHeadProtocolInstance", "OrderPosProtocolAcc", activeConn );  
                m_BackUp = m_Accessor.getPropertyTable();
			}

			++m_Initialized;
		}
		catch(...) 
        {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "OrderPosProtocolDM::init()."); 
			throw; 
		}
	}

	void OrderPosProtocolDM::shutdown() 
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderPosProtocolDM::shutdown().");
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
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "OrderPosProtocolDM::shutdown()."); 
			throw;
		}
	}

    bool OrderPosProtocolDM::saveOrderPosProtocol()
    {
        BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderPosProtocolDM::saveOrderPosProtocol().");

        try 
        {
            if( m_BackUp.isNull() || m_BackUp.isEmpty() )
            {
                return false;
            }

            basar::db::aspect::AccessorPropertyTable_YIterator yit = m_BackUp.begin();

            while( !yit.isEnd() )
            {
                m_Accessor.execute("saveOrderPosProtocol", yit);
                ++yit;
            }

            return true;
        }
        catch(...) 
        {
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "OrderPosProtocolDM::saveOrderPosProtocol()."); 
			throw;
		}
    }

    bool OrderPosProtocolDM::deleteOrderPosProtocol(basar::db::aspect::AccessorPropertyTable_YIterator yit)
    {
        BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderPosProtocolDM::deleteOrderPosProtocol().");

        try 
        {
            m_Accessor.beginTransaction();

            basar::db::aspect::ExecuteResultInfo resPos = m_Accessor.execute("deleteOrderPosProtocol", yit);

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
			BLOG_ERROR(cscbatchhistory::LoggerPool::loggerDomModules, "OrderPosProtocolDM::deleteOrderPosProtocol()."); 
			throw;
		}
    }

    void OrderPosProtocolDM::setBackup(basar::db::aspect::AccessorPropertyTableRef propTab)
    {
        if( propTab.isEmpty() ) return;

        basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();

        while( !yit.isEnd() )
        {
            basar::db::aspect::AccessorPropertyTable_YIterator yitTemp = m_BackUp.insert(basar::FOR_INSERT);
            yitTemp.match(yit);
            // add params which will be not provided by batch orders
            yitTemp.setString(  "activitytype",     orderProtocol::DELETE              );
            yitTemp.setInt16(   "eventtype",        orderProtocol::NO_EVENT_TYPE       );
            yitTemp.setInt16(   "processedby",      orderProtocol::CSC_BATCHES_HISTORY );
            yitTemp.setString(  "view",             ""                                 ); 
            yitTemp.setInt32(   "orderrequestqty",  0                                  );
            yitTemp.setString(  "itemtext",         ""                                 );
            yitTemp.setString(  "extratext",        ""                                 );

            ++yit;
        }  
    }

    basar::db::aspect::AccessorPropertyTable_YIterator OrderPosProtocolDM::getSearchYit()
    {
        BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderPosProtocolDM::getSearchYit().");

        return getSearchYIteratorPtr()->get();
    }

	searchyiterator::SearchYIteratorPtr OrderPosProtocolDM::getSearchYIteratorPtr()
    {
		BLOG_TRACE_METHOD(cscbatchhistory::LoggerPool::loggerDomModules, "OrderPosProtocolDM::getSearchYIteratorPtr().");

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

