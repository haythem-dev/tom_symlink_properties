#include "tenderprogressdm.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>
#include <infrastructure/accessor/tender/tenderprogressacc_definitions.h>

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
        namespace tender
        {
            const libutil::misc::ClassInfo& TenderProgressDM::getClassInfo()
            {
                static const libutil::misc::ClassInfo info( "TenderProgressDM", libutil::misc::Version( 1, 0, 0, 0 ) );
                return info;
            }

            TenderProgressDM::TenderProgressDM()
            {
                METHODNAME_DEF( TenderProgressDM, TenderProgressDM )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );
            }

            TenderProgressDM::~TenderProgressDM()
            {
                METHODNAME_DEF( TenderProgressDM, ~TenderProgressDM )
                BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );
            }

            void TenderProgressDM::injectTenderProgressAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
            {
                METHODNAME_DEF( TenderProgressDM, injectTenderProgressAccessor )
                BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );

                m_Accessor = accessor;
            }

            libutil::infrastructure::accessor::IAccessorPtr TenderProgressDM::getTenderProgressAccessor() const
            {
                CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
                return m_Accessor;
            }

            void TenderProgressDM::findTenderProgressForOMGCalcultion( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
            {
                METHODNAME_DEF( TenderProgressDM, findTenderProgressForOMGCalcultion )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );
                using libtender::infrastructure::accessor::tenderprogress::lit::SELECT_TENDERPROGRESS_FOR_OMGCALCULATION;

                basar::db::aspect::ExecuteResultInfo executeResultInfo;
                try
                {
                    executeResultInfo = m_Accessor->execute( SELECT_TENDERPROGRESS_FOR_OMGCALCULATION, yitSearch );
                }
                catch( basar::Exception & e  )
                {
                    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_TENDERPROGRESS_FOR_OMGCALCULATION, __FILE__, __LINE__ );
                    throw;
                }

                if( executeResultInfo.hasError() )
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_TENDERPROGRESS_FOR_OMGCALCULATION, executeResultInfo )
                }

            }

            void TenderProgressDM::findArticleQuantityByPattern( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
            {
                METHODNAME_DEF( TenderProgressDM, findArticleQuantityByPattern )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );
                using libtender::infrastructure::accessor::tenderprogress::lit::SELECT_TENDERPROGRESS_BY_ARTICLEQUANTITY;

                basar::db::aspect::ExecuteResultInfo executeResultInfo;
                try
                {
                    executeResultInfo = m_Accessor->execute( SELECT_TENDERPROGRESS_BY_ARTICLEQUANTITY, yitSearch );
                }
                catch( basar::Exception & e  )
                {
                    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_TENDERPROGRESS_BY_ARTICLEQUANTITY, __FILE__, __LINE__ );
                    throw;
                }

                if( executeResultInfo.hasError() )
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_TENDERPROGRESS_BY_ARTICLEQUANTITY, executeResultInfo )
                }

            }

            void TenderProgressDM::findByPattern( AccessorPropertyTable_YIterator yitSearch )
            {
                METHODNAME_DEF( TenderProgressDM, findByPattern )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                using libtender::infrastructure::accessor::tenderprogress::lit::SELECT_TENDERPROGRESS_BY_PATTERN;

                basar::db::aspect::ExecuteResultInfo executeResultInfo;
                try
                {
                    executeResultInfo = m_Accessor->execute( SELECT_TENDERPROGRESS_BY_PATTERN, yitSearch );
                }
                catch( basar::Exception & e  )
                {
                    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_TENDERPROGRESS_BY_PATTERN, __FILE__, __LINE__ );
                    throw;
                }

                if( executeResultInfo.hasError() )
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_TENDERPROGRESS_BY_PATTERN, executeResultInfo )
                }
            }

			void TenderProgressDM::findByTender( AccessorPropertyTable_YIterator yitSearch )
            {
                METHODNAME_DEF( TenderProgressDM, findByTender )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

				using libtender::infrastructure::accessor::tenderprogress::lit::SELECT_TENDERPROGRESS_BY_TENDER;

                basar::db::aspect::ExecuteResultInfo executeResultInfo;
                try
                {
                    executeResultInfo = m_Accessor->execute( SELECT_TENDERPROGRESS_BY_TENDER, yitSearch );
                }
                catch( basar::Exception & e  )
                {
                    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_TENDERPROGRESS_BY_TENDER, __FILE__, __LINE__ );
                    throw;
                }

                if( executeResultInfo.hasError() )
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_TENDERPROGRESS_BY_TENDER, executeResultInfo )
                }
            }

            void TenderProgressDM::save( basar::db::aspect::AccessorPropertyTable_YIterator yIterator)
            {
                METHODNAME_DEF( TenderProgressDM, save )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                using libtender::infrastructure::accessor::tenderprogress::lit::SAVE_TENDER_PROGRESS;

                basar::db::aspect::ExecuteResultInfo executeResultInfo;
                try
                {
                    executeResultInfo = m_Accessor->execute( SAVE_TENDER_PROGRESS ,yIterator );
                }
                catch( basar::Exception& e )
                {
                    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SAVE_TENDER_PROGRESS, __FILE__, __LINE__ );
                }
                if( executeResultInfo.hasError() )
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SAVE_TENDER_PROGRESS, executeResultInfo )
                }
            }

            AccessorPropertyTableRef TenderProgressDM::get() const
            {
                METHODNAME_DEF( TenderProgressDM, get )
                BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );

                return m_Accessor->getPropertyTable();
            }

            void TenderProgressDM::resetSearchYit()
            {
                METHODNAME_DEF( TenderProgressDM, resetSearchYit )
                BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );

                getSearchYIterator()->reset();
            }

            AccessorPropertyTable_YIterator TenderProgressDM::getSearchYit() const
            {
                METHODNAME_DEF( TenderProgressDM, getSearchYit )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                return getSearchYIterator()->get();
            }

            libutil::domMod::SearchYIteratorPtr TenderProgressDM::getSearchYIterator() const
            {
                METHODNAME_DEF( TenderProgressDM, getSearchYIterator )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                if( NULL == m_SearchYIterator.get() )
                {
                    m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
                        new libutil::domMod::SearchYIterator( properties::TENDER_NO.toDescriptionString() +
															  properties::ARTICLE_NO.toDescriptionString() +
															  properties::QUANTITY_DELIVERED.toDescriptionString() +
															  properties::SHOW_INVOICED_ORDERS.toDescriptionString() +
                                                              properties::ORDER_NO.toDescriptionString() +
                                                              properties::ORDER_DATE.toDescriptionString() 
                                                            )
                    );
                }

                return m_SearchYIterator;
            }

            bool TenderProgressDM::isEmpty() const
            {
                METHODNAME_DEF( TenderProgressDM, isEmpty )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                if( 0 == getTenderProgressAccessor().get() || m_Accessor->getPropertyTable().isNull() ||
                    m_Accessor->getPropertyTable().isEmpty() )
                {
                    return true;
                }

                return false;
            }

        } // namespace tender
    }	  // namespace domMod
}		  // namespace libtender
