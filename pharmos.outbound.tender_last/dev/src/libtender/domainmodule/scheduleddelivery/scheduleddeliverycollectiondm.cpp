#include "scheduleddeliverycollectiondm.h"

#include <domainmodule/libtender_properties_definitions.h>
#include <loggerpool/tenderloggerpool.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliverycollectionacc_definitions.h>
#include <componentmanager/ischeduleddeliveryconnectedfactory.h>

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
		namespace scheduledDelivery
		{

			ScheduledDeliveryCollectionDM::ScheduledDeliveryCollectionDM()
			:m_Logger( libtender::LoggerPool::getLoggerDomModules() )
			{
				METHODNAME_DEF( ScheduledDeliveryCollectionDM, ScheduledDeliveryCollectionDM )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );
			}

			ScheduledDeliveryCollectionDM::~ScheduledDeliveryCollectionDM()
			{
				METHODNAME_DEF( ScheduledDeliveryCollectionDM, ~ScheduledDeliveryCollectionDM )
                BLOG_TRACE_METHOD( LoggerPool::getLoggerDomModules(), fun );
			}

			void ScheduledDeliveryCollectionDM::injectScheduledDeliveryAccessor( libutil::infrastructure::accessor::IAccessorPtr accessor )
            {
                METHODNAME_DEF( ScheduledDeliveryCollectionDM, injectScheduledDeliveryAccessor )
                BLOG_TRACE_METHOD( getLogger(), fun );

                m_Accessor = accessor;
            }

            void ScheduledDeliveryCollectionDM::injectScheduledDeliveryFactory( libtender::componentManager::IScheduledDeliveryConnectedFactoryPtr  factory )
            {
                METHODNAME_DEF( ScheduledDeliveryCollectionDM, injectScheduledDeliveryFactory )
                BLOG_TRACE_METHOD(LoggerPool::getLoggerDomModules(), fun );

                m_ScheduledDeliveryConnectedFactory = factory;
            }

			libutil::infrastructure::accessor::IAccessorPtr ScheduledDeliveryCollectionDM::getScheduledDeliveryAccessor() const
            {
                CHECK_INSTANCE_EXCEPTION( m_Accessor.get() );
                return m_Accessor;
            }

			void ScheduledDeliveryCollectionDM::findByPattern( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
			{
				METHODNAME_DEF( ScheduledDeliveryCollectionDM, findByPattern )
                BLOG_TRACE_METHOD( getLogger(), fun );

				using libtender::infrastructure::accessor::scheduledDelivery::lit::SELECT_SCHEDULEDDELIVERY_BY_PATTERN;
                basar::db::aspect::ExecuteResultInfo executeResultInfo;

                try
                {
					executeResultInfo = m_Accessor->execute(SELECT_SCHEDULEDDELIVERY_BY_PATTERN, yitSearch, true, true, basar::db::aspect::HOLD_RECORDSET);
                }
                catch( basar::Exception & e  )
                {
                    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_SCHEDULEDDELIVERY_BY_PATTERN, __FILE__, __LINE__ );
                    throw;
                }

				if( executeResultInfo.hasError() )
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_SCHEDULEDDELIVERY_BY_PATTERN, executeResultInfo )
                }
			}

			bool ScheduledDeliveryCollectionDM::isExisting( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
			{
				METHODNAME_DEF( ScheduledDeliveryCollectionDM, isExisting )
                BLOG_TRACE_METHOD( getLogger(), fun );

				using libtender::infrastructure::accessor::scheduledDelivery::lit::IS_EXISTING;

                basar::db::aspect::ExecuteResultInfo executeResultInfo;
                try
                {
                    executeResultInfo = m_Accessor->execute( IS_EXISTING, yitSearch );	
                }
                catch( basar::Exception & e  )
                {
                    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, IS_EXISTING, __FILE__, __LINE__ );
                    throw;
                }

                if( executeResultInfo.hasError() )
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, IS_EXISTING, executeResultInfo )
                }

				return !m_Accessor->getPropertyTable().isEmpty();
			}

			void ScheduledDeliveryCollectionDM::getQuantity( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
			{
				METHODNAME_DEF( ScheduledDeliveryCollectionDM, getQuantity )
                BLOG_TRACE_METHOD( getLogger(), fun );

				using libtender::infrastructure::accessor::scheduledDelivery::lit::SELECT_QUANTITY_BY_PATTERN;

                basar::db::aspect::ExecuteResultInfo executeResultInfo;
                try
                {
                    executeResultInfo = m_Accessor->execute( SELECT_QUANTITY_BY_PATTERN, yitSearch );
                }
                catch( basar::Exception & e  )
                {
                    libutil::exceptions::ExceptionHandler::processAccessorException( e, LoggerPool::getLoggerDomModules(), fun, SELECT_QUANTITY_BY_PATTERN, __FILE__, __LINE__ );
                    throw;
                }

                if( executeResultInfo.hasError() )
                {
                    THROW_ACCESSOR_EXECUTION_EXCEPTION( fun, SELECT_QUANTITY_BY_PATTERN, executeResultInfo )
                }
			}


			basar::db::aspect::AccessorPropertyTableRef ScheduledDeliveryCollectionDM::get() const
			{
				METHODNAME_DEF( ScheduledDeliveryCollectionDM, get )
                BLOG_TRACE_METHOD( getLogger(), fun );

				return m_Accessor->getPropertyTable();
			}

			void ScheduledDeliveryCollectionDM::resetSearchYit()
			{
				METHODNAME_DEF( ScheduledDeliveryCollectionDM, resetSearchYit )
                BLOG_TRACE_METHOD( getLogger(), fun );

				getSearchYIterator()->reset();
			}

			basar::db::aspect::AccessorPropertyTable_YIterator ScheduledDeliveryCollectionDM::getSearchYit() const
			{
				METHODNAME_DEF( ScheduledDeliveryCollectionDM, getSearchYit )
                BLOG_TRACE_METHOD( getLogger(), fun );

				return getSearchYIterator()->get();
			}

			libutil::domMod::SearchYIteratorPtr ScheduledDeliveryCollectionDM::getSearchYIterator() const
			{
				METHODNAME_DEF( ScheduledDeliveryCollectionDM, getSearchYIterator )
                BLOG_TRACE_METHOD( getLogger(), fun );

				if( NULL == m_SearchYIterator )
				{
					m_SearchYIterator = libutil::domMod::SearchYIteratorPtr(
                        new libutil::domMod::SearchYIterator(
							properties::TENDER_ID.toDescriptionString() +  
							properties::DELIVERY_DATE.toDescriptionString() +
							properties::DELIVERY_DATE_FROM.toDescriptionString() +
							properties::DELIVERY_DATE_TO.toDescriptionString() +
							properties::CSC_ORDER_NO.toDescriptionString() +
							properties::ARTICLE_CODE.toDescriptionString() +
							properties::TENDER_DELIVERY_SCHEDULE_HEAD_ID.toDescriptionString() 
							)
					);
				}

				return m_SearchYIterator;
			}

			IScheduledDeliveryPtr ScheduledDeliveryCollectionDM::getScheduledDelivery( const basar::db::aspect::AccessorPropertyTable_YIterator yitScheduledDelivery ) const
			{
				METHODNAME_DEF( ScheduledDeliveryCollectionDM, getScheduledDelivery )
				BLOG_TRACE_METHOD( getLogger(), fun );

				return getScheduledDeliveryConnectedFactory()->create( yitScheduledDelivery );
			}

			libtender::componentManager::IScheduledDeliveryConnectedFactoryPtr ScheduledDeliveryCollectionDM::getScheduledDeliveryConnectedFactory() const
			{
				METHODNAME_DEF( ScheduledDeliveryCollectionDM, getScheduledDeliveryConnectedFactory )
				BLOG_TRACE_METHOD( getLogger(), fun );

				CHECK_INSTANCE_EXCEPTION( m_ScheduledDeliveryConnectedFactory.get() );
                return m_ScheduledDeliveryConnectedFactory; 
			}

			bool ScheduledDeliveryCollectionDM::isEmpty() const
			{
				METHODNAME_DEF( ScheduledDeliveryCollectionDM, isEmpty )
				BLOG_TRACE_METHOD( getLogger(), fun );

				if( 0 == getScheduledDeliveryAccessor().get() || m_Accessor->getPropertyTable().isNull() ||
                    m_Accessor->getPropertyTable().isEmpty() )
                {
                    return true;
                }

                return false;
			}

			const log4cplus::Logger & ScheduledDeliveryCollectionDM::getLogger() const
			{
				METHODNAME_DEF( ScheduledDeliveryCollectionDM, getLogger )
                BLOG_TRACE_METHOD( m_Logger, fun );

				return m_Logger;
			}

		} //end namespace scheduledDelivery
	}     //end namespace domMod
}         //end namespace libtender
