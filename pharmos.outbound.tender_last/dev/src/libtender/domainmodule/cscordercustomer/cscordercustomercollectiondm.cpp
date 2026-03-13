#include "cscordercustomercollectiondm.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>
#include <infrastructure/accessor/cscordercustomer/cscordercustomercollectionacc_definitions.h>

#include <libbasardbaspect.h>
#include <libutil/exception.h>

namespace libtender
{
    namespace domMod
    {
        namespace tender
        {
            using basar::db::aspect::AccessorPropertyTable_YIterator;
            using basar::db::aspect::AccessorPropertyTableRef;
            using libtender::LoggerPool;
        }
    }
}

namespace libtender
{
    namespace domMod
    {
        namespace cscordercustomer
        {

            CscOrderCustomerCollectionDM::CscOrderCustomerCollectionDM()
            :m_Logger( libtender::LoggerPool::getLoggerDomModules() )
            {
                METHODNAME_DEF( CscOrderCustomerCollectionDM, CscOrderCustomerCollectionDM )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );
            }
            
            CscOrderCustomerCollectionDM::~CscOrderCustomerCollectionDM()
            {
                METHODNAME_DEF( CscOrderCustomerCollectionDM, ~CscOrderCustomerCollectionDM )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );
            }
            
            void CscOrderCustomerCollectionDM::injectCscOrderAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
            {
                METHODNAME_DEF( CscOrderCustomerCollectionDM, injectCscOrderAccessor )
                BLOG_TRACE_METHOD( getLogger(), fun );

                m_Accessor = accessor;
            }

            libutil::infrastructure::accessor::IAccessorPtr CscOrderCustomerCollectionDM::getCscOrderAccessor() const
            {
                CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
                return m_Accessor;
            }

            void CscOrderCustomerCollectionDM::findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
            {
                METHODNAME_DEF( CscOrderCustomerCollectionDM, findByPattern )
                BLOG_TRACE_METHOD( getLogger(), fun );
               
                if (yitSearch.isNull())
                    return;
            }

            basar::db::aspect::AccessorPropertyTableRef CscOrderCustomerCollectionDM::get() const
            {
                METHODNAME_DEF( CscOrderCustomerCollectionDM, get )
                BLOG_TRACE_METHOD( getLogger(), fun );
            
                return m_Accessor->getPropertyTable();
            }

            void CscOrderCustomerCollectionDM::resetSearchYit()
            {
                METHODNAME_DEF( CscOrderCustomerCollectionDM, resetSearchYit )
                BLOG_TRACE_METHOD( getLogger(), fun );
            
                getSearchYIterator()->reset();
            }

            basar::db::aspect::AccessorPropertyTable_YIterator CscOrderCustomerCollectionDM::getSearchYit() const
            {
                METHODNAME_DEF( CscOrderCustomerCollectionDM, getSearchYit )
                BLOG_TRACE_METHOD( getLogger(), fun );
            
                return getSearchYIterator()->get();
            }

            libutil::domMod::SearchYIteratorPtr CscOrderCustomerCollectionDM::getSearchYIterator() const
            {
                METHODNAME_DEF( CscOrderCustomerCollectionDM, getSearchYIterator )
                BLOG_TRACE_METHOD( getLogger(), fun );
                
                if( NULL == m_SearchYIterator )
                {
                    m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
                        new libutil::domMod::SearchYIterator("")
                    );
                }
                return m_SearchYIterator;
            }

            bool CscOrderCustomerCollectionDM::isEmpty() const
            {
                METHODNAME_DEF( CscOrderCustomerCollectionDM, isEmpty )
                BLOG_TRACE_METHOD( getLogger(), fun );
                
                if( 0 == getCscOrderAccessor().get() || m_Accessor->getPropertyTable().isNull() ||
                    m_Accessor->getPropertyTable().isEmpty() )
                {
                    return true;
                }
                return false;
            }
            
            const log4cplus::Logger & CscOrderCustomerCollectionDM::getLogger() const
            {
                METHODNAME_DEF( CscOrderCustomerCollectionDM, getLogger )
                BLOG_TRACE_METHOD( m_Logger, fun );
            
                return m_Logger;
            }

		} //end namespace scheduledDelivery
	}     //end namespace domMod
}         //end namespace libtender
