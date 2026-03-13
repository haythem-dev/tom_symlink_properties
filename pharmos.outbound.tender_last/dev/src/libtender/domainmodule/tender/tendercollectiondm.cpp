//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tendercollectiondm.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>
#include <infrastructure/accessor/tender/tendercollectionacc_definitions.h>
#include <componentmanager/itenderconnectedfactory.h>

#include <libbasardbaspect.h>
#include <libutil/exception.h>

//-------------------------------------------------------------------------------------------------//
// using declarations section
//-------------------------------------------------------------------------------------------------//
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
//-------------------------------------------------------------------------------------------------//
// class definition section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
    namespace domMod
    {
        namespace tender
        {
            //--------------------------------------------------------------------------------------------------//
            // static definition section
            //--------------------------------------------------------------------------------------------------//
            const libutil::misc::ClassInfo& TenderCollectionDM::getClassInfo()
            {
                static const libutil::misc::ClassInfo info( "TenderCollectionDM", libutil::misc::Version( 1, 0, 0, 0 ) );
                return info;
            }

            //--------------------------------------------------------------------------------------------------//
            // object definition section
            //--------------------------------------------------------------------------------------------------//
            TenderCollectionDM::TenderCollectionDM()
            {
                METHODNAME_DEF( TenderCollectionDM, TenderCollectionDM )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );
            }

            TenderCollectionDM::~TenderCollectionDM()
            {
                METHODNAME_DEF( TenderCollectionDM, ~TenderCollectionDM )
                BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );
            }

            void TenderCollectionDM::injectTenderAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
            {
                METHODNAME_DEF( TenderCollectionDM, injectTenderAccessor )
                BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );

                m_Accessor = accessor;
            }

            void TenderCollectionDM::injectTenderFactory( libtender::componentManager::ITenderConnectedFactoryPtr  tenderFactory )
            {
                METHODNAME_DEF( TenderCollectionDM, injectTenderFactory )
                BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );

                m_TenderConnectedFactory = tenderFactory;
            }

            libutil::infrastructure::accessor::IAccessorPtr TenderCollectionDM::getTenderAccessor() const
            {
                CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
                return m_Accessor;
            }

            void TenderCollectionDM::findByPattern(AccessorPropertyTable_YIterator yitSearch )
            {
                METHODNAME_DEF( TenderCollectionDM, findByPattern )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                using libtender::infrastructure::accessor::tender::lit::SELECT_TENDER_BY_PATTERN;

                basar::db::aspect::ExecuteResultInfo executeResultInfo;
                try
                {
                    executeResultInfo = m_Accessor->execute( SELECT_TENDER_BY_PATTERN, yitSearch ); 
                }
                catch( basar::Exception & e  )
                {
                    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_TENDER_BY_PATTERN, __FILE__, __LINE__ );
                    throw;
                }
				
                if( executeResultInfo.hasError() )
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_TENDER_BY_PATTERN, executeResultInfo )
                }
            }

             void TenderCollectionDM::findByEndDate(AccessorPropertyTable_YIterator yitSearch )
            {
                METHODNAME_DEF( TenderCollectionDM, findByPattern )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                using libtender::infrastructure::accessor::tender::lit::SELECT_TENDER_BY_ENDDATE;

                basar::db::aspect::ExecuteResultInfo executeResultInfo;
                try
                {
                    executeResultInfo = m_Accessor->execute( SELECT_TENDER_BY_ENDDATE, yitSearch ); 
                }
                catch( basar::Exception & e  )
                {
                    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_TENDER_BY_ENDDATE, __FILE__, __LINE__ );
                    throw;
                }

                if( executeResultInfo.hasError() )
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_TENDER_BY_ENDDATE, executeResultInfo )
                }
            }

            void TenderCollectionDM::findSuccessorTender(AccessorPropertyTable_YIterator yitSearch )
            {
                METHODNAME_DEF( TenderCollectionDM, findSuccessorTender )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                using libtender::infrastructure::accessor::tender::lit::SELECT_SUCCESSOR_TENDER;

                basar::db::aspect::ExecuteResultInfo executeResultInfo;
                try
                {
                    executeResultInfo = m_Accessor->execute( SELECT_SUCCESSOR_TENDER, yitSearch ); 
                }
                catch( basar::Exception & e  )
                {
                    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_SUCCESSOR_TENDER, __FILE__, __LINE__ );
                    throw;
                }

                if( executeResultInfo.hasError() )
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_SUCCESSOR_TENDER, executeResultInfo )
                }
            }

            void TenderCollectionDM::findValidTenders( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch)
            {
                METHODNAME_DEF( TenderCollectionDM, findActiveTenders )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                using libtender::infrastructure::accessor::tender::lit::SELECT_VALID_TENDERS;

                basar::db::aspect::ExecuteResultInfo executeResultInfo;
                try
                {
                    executeResultInfo = m_Accessor->execute( SELECT_VALID_TENDERS, yitSearch, true, true, basar::db::aspect::HOLD_RECORDSET ); 
                }
                catch( basar::Exception & e  )
                {
                    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_VALID_TENDERS, __FILE__, __LINE__ );
                    throw;
                }

                if( executeResultInfo.hasError() )
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_VALID_TENDERS, executeResultInfo )
                }
            }

			void TenderCollectionDM::findExpiredTenders(basar::db::aspect::AccessorPropertyTable_YIterator yitSearch)
			{
				METHODNAME_DEF( TenderCollectionDM, findExpiredTenders )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                using libtender::infrastructure::accessor::tender::lit::SELECT_EXPIRED_TENDERS;

                basar::db::aspect::ExecuteResultInfo executeResultInfo;
                try
                {
                    executeResultInfo = m_Accessor->execute(SELECT_EXPIRED_TENDERS, yitSearch, true, true, basar::db::aspect::HOLD_RECORDSET); 
                }
                catch(basar::Exception & e)
                {
                    libutil::exceptions::ExceptionHandler::processAccessorException(e, LoggerPool::getLoggerDomModules(), fun, SELECT_EXPIRED_TENDERS, __FILE__, __LINE__);
                    throw;
                }

                if(executeResultInfo.hasError())
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION(fun, SELECT_EXPIRED_TENDERS, executeResultInfo)
                }
			}

            AccessorPropertyTableRef TenderCollectionDM::get() const
            {
                METHODNAME_DEF( TenderCollectionDM, get )
                BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );

                return m_Accessor->getPropertyTable();
            }

            void TenderCollectionDM::resetSearchYit()
            {
                METHODNAME_DEF( TenderCollectionDM, resetSearchYit )
                BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );

                getSearchYIterator()->reset();
            }

            AccessorPropertyTable_YIterator TenderCollectionDM::getSearchYit() const
            {
                METHODNAME_DEF( TenderCollectionDM, getSearchYit )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                return getSearchYIterator()->get();
            }

            libutil::domMod::SearchYIteratorPtr TenderCollectionDM::getSearchYIterator() const
            {
                METHODNAME_DEF( TenderCollectionDM, getSearchYIterator )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                if( NULL == m_SearchYIterator.get() )
                {
                    m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
                        new libutil::domMod::SearchYIterator( properties::TENDER_NO.toDescriptionString() +
                                                              properties::CONTRACT_NO.toDescriptionString() +
                                                              properties::PARTNER_NO.toDescriptionString() +
															  properties::PHARMACY_CHAIN_NO.toDescriptionString() +
                                                              properties::TENDER_TYPE.toDescriptionString() +
                                                              properties::TENDER_STATE.toDescriptionString() +
															  properties::ARTICLE_CODE.toDescriptionString() +
                                                              properties::CUSTOMER_NO.toDescriptionString() +
															  properties::PHARMACY_NAME.toDescriptionString() +
															  properties::DATE_FROM.toDescriptionString() +
                                                              properties::DATE_TO.toDescriptionString() +
                                                              properties::BRANCH_NO.toDescriptionString() +
															  properties::NO_OF_DELIVERIES.toDescriptionString() +
															  properties::NO_OF_OPEN_DELIVERIES.toDescriptionString() +
                                                              properties::PHARMACY_GROUPID.toDescriptionString() +
                                                              properties::ARTICLE_NO.toDescriptionString() +
                                                              properties::SUPPLIER_NO.toDescriptionString() 
                                                            )
                    );
                }

                return m_SearchYIterator;
            }

            ITenderPtr TenderCollectionDM::getTender( const basar::db::aspect::AccessorPropertyTable_YIterator yitTender ) const
            {
                METHODNAME_DEF( TenderCollectionDM, getTender )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                return getTenderConnectedFactory()->create( yitTender );
            }

            libtender::componentManager::ITenderConnectedFactoryPtr TenderCollectionDM::getTenderConnectedFactory() const
            {
                METHODNAME_DEF( TenderCollectionDM, getTenderConnectedFactory )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                CHECK_INSTANCE_EXCEPTION( m_TenderConnectedFactory.get() );
                return m_TenderConnectedFactory;
            }

            bool TenderCollectionDM::isEmpty() const
            {
                METHODNAME_DEF( TenderCollectionDM, isEmpty )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );

                if( 0 == getTenderAccessor().get() || m_Accessor->getPropertyTable().isNull() ||
                    m_Accessor->getPropertyTable().isEmpty() )
                {
                    return true;
                }

                return false;
            }

        } // namespace tender
    } // namespace domMod
} // namespace libtender
