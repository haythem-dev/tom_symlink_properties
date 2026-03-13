//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tenderprotocoluc.h"

#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/tender/tenderprotocoldm.h"
#include "domainmodule/tender/tenderprogressdm.h"
#include "domainmodule/tender/itender.h"
#include "domainmodule/tender/itenderpositioncollection.h"
#include "iusecasegetter.h"

#include "tenderprotocolvc.h"

#include <loggerpool/tenderloggerpool.h>

#include <libutil/usecase.h>
#include <libutil/user.h>
#include <libutil/infrastructurecomponent.h>

#include <libbasardbaspect_exceptions.h>
#include <libbasarlogin.h>

//-------------------------------------------------------------------------------------------------//
// namespace sections
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace useCase
    {
        BEGIN_HANDLERS_REGISTRATION( TenderProtocolUC )
			SYSTEM_EVENT_HANDLER_REGISTRATION( TenderProtocolUC, SearchTenderProtocols )
			SYSTEM_EVENT_HANDLER_REGISTRATION( TenderProtocolUC, ChangeQuantityToChargeRequested  )
			SYSTEM_EVENT_HANDLER_REGISTRATION( TenderProtocolUC, CreateInvoiceRequested           )
			SYSTEM_EVENT_HANDLER_REGISTRATION( TenderProtocolUC, SelectOrderRequested			  )
        END_HANDLERS_REGISTRATION()

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // TenderProtocolUC::TenderProtocolUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TenderProtocolUC::TenderProtocolUC()
        : m_Logger( libtender::LoggerPool::getLoggerUseCases() )
        {
            METHODNAME_DEF( TenderProtocolUC, TenderProtocolUC )
            BLOG_TRACE_METHOD( getLogger(), fun );

            registerEventHandlers( true );
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // TenderProtocolUC::~TenderProtocolUC()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TenderProtocolUC::~TenderProtocolUC()
        {
            METHODNAME_DEF( TenderProtocolUC, ~TenderProtocolUC )
            BLOG_TRACE_METHOD( getLogger(), fun );

            registerEventHandlers( false );
        }

        void TenderProtocolUC::injectTenderProtocolVC( tender::viewConn::TenderProtocolVCPtr tenderProtocolVC)
        {
            METHODNAME_DEF( TenderProtocolUC, injectTenderProtocolVC )
            BLOG_TRACE_METHOD( getLogger(), fun );

            m_TenderProtocolVC = tenderProtocolVC;
        }

		void TenderProtocolUC::injectTenderProgressDM( libtender::domMod::tender::ITenderProgressPtr tenderProgress )
		{
			METHODNAME_DEF( TenderProtocolUC, injectTenderProgressDM )
            BLOG_TRACE_METHOD( getLogger(), fun );

			m_TenderProgress = tenderProgress;
		}

		void TenderProtocolUC::injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter )
		{
			METHODNAME_DEF( TenderProtocolUC, injectUseCaseGetter )
            BLOG_TRACE_METHOD( getLogger(), fun );

			m_UseCaseGetter = useCaseGetter;
		}

		void TenderProtocolUC::setTender( libtender::domMod::tender::ITenderPtr tender )
		{
			METHODNAME_DEF( TenderProtocolUC, setTender )
            BLOG_TRACE_METHOD( getLogger(), fun );
		
            m_Tender = tender;
		}

		void TenderProtocolUC::setArea( basar::Int16 area )
		{
			m_Area = area;
		}

        tender::viewConn::TenderProtocolVCPtr TenderProtocolUC::getTenderProtocolVC()
        {
            METHODNAME_DEF( TenderProtocolUC, getTenderProtocolVC )
            BLOG_TRACE_METHOD( getLogger(), fun );

            CHECK_INSTANCE_EXCEPTION( m_TenderProtocolVC.get() );
            return m_TenderProtocolVC;
        }

		libtender::domMod::tender::ITenderProgressPtr TenderProtocolUC::getTenderProgressDM()
		{
			METHODNAME_DEF( TenderProtocolUC, getTenderProgressDM )
            BLOG_TRACE_METHOD( getLogger(), fun );
		
            CHECK_INSTANCE_EXCEPTION( m_TenderProgress.get() );
            return m_TenderProgress;
		}

		 tender::componentManager::IUseCaseGetterPtr TenderProtocolUC::getUseCaseGetter()
		 {
			METHODNAME_DEF( TenderProtocolUC, getUseCaseGetter )
            BLOG_TRACE_METHOD( getLogger(), fun );
		 
			CHECK_INSTANCE_EXCEPTION( m_UseCaseGetter.get() );
			return m_UseCaseGetter;
		 }

        const log4cplus::Logger& TenderProtocolUC::getLogger() const
        {
            METHODNAME_DEF( TenderProtocolUC, getLogger )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return m_Logger;
        }

        void TenderProtocolUC::init()
        {
            METHODNAME_DEF( TenderProtocolUC, init )
            BLOG_TRACE_METHOD( getLogger(), fun );
        }

        void TenderProtocolUC::reset()
        {
            METHODNAME_DEF( TenderProtocolUC, reset )
			BLOG_TRACE_METHOD( getLogger(), fun );

            init();
        }

        void TenderProtocolUC::run()
        {
            METHODNAME_DEF( TenderProtocolUC, run )
            BLOG_TRACE_METHOD( getLogger(), fun );

            init();

			basar::appl::SystemEventManager::getInstance().fire( "SearchTenderProtocols" );
            getTenderProtocolVC()->show();
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SYSTEM_EVENT_HANDLER_DEFINITION( TenderProtocolUC, SearchTenderProtocols )
        {
            METHODNAME_DEF( TenderProtocolUC, SearchTenderProtocolsBGRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;
		
            basar::appl::EventReturnType result( basar::appl::HANDLER_OK );
			
            try
			{
				getTenderProgressDM()->resetSearchYit();
				getTenderProgressDM()->getSearchYit().setInt32( properties::TENDER_NO, m_Tender->getHeader().getInt32( properties::TENDER_NO ) );
				getTenderProgressDM()->getSearchYit().setInt16( properties::SHOW_INVOICED_ORDERS, static_cast<basar::Int16>(getTenderProtocolVC()->showInvoicedOrders() ) );

				getTenderProgressDM()->findByTender( getTenderProgressDM()->getSearchYit() );

				if( getTenderProgressDM()->isEmpty() )
                {
                    result.ret     = basar::appl::HANDLER_NOT_FOUND;
                    result.message = QCoreApplication::translate( "tender::useCase::TenderProtocolUC", "No protocols found!").toLocal8Bit().data();
		
                    getTenderProtocolVC()->displayNoProtocolFound();
                }

                getTenderProtocolVC()->matchFromTenderProtocol( getTenderProgressDM()->get() );
            }
            catch( const basar::Exception & e )
            {
				BLOG_ERROR( m_Logger, e.what().c_str() );
				throw;
			}
		
            return result;
        }

		SYSTEM_EVENT_HANDLER_DEFINITION( TenderProtocolUC, ChangeQuantityToChargeRequested )
        {
            METHODNAME_DEF( TenderProtocolUC, ChangeQuantityToChargeRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;
		
            basar::appl::EventReturnType result( basar::appl::HANDLER_OK );
		
            try
            {
				basar::db::aspect::AccessorPropertyTable_YIterator selectedOrder = getTenderProtocolVC()->getCurrentOrder( m_TenderProgress->get() );

				if( !selectedOrder.isContainedAndSet( properties::QUANTITY_TO_CHARGE ) )
				{
					return result;
				}

				basar::db::aspect::AccessorPropertyTableRef orderCollection = m_TenderProgress->get();
				basar::db::aspect::AccessorPropertyTable_YIterator order = orderCollection.begin();

				for( ; !order.isEnd(); ++order )
				{	
					if( selectedOrder.getInt32( properties::ORDER_NO ) == order.getInt32( properties::ORDER_NO ) &&
						selectedOrder.getDate( properties::ORDER_DATE ) == order.getDate( properties::ORDER_DATE ) &&
						selectedOrder.getInt32( properties::ARTICLE_NO ) == order.getInt32( properties::ARTICLE_NO ) )
					{
						if( selectedOrder.getInt32( properties::QUANTITY_TO_CHARGE ) > (order.getInt32( properties::QUANTITY ) - order.getInt32( properties::QUANTITY_CHARGED )) )
						{
							selectedOrder.setInt32( properties::QUANTITY_TO_CHARGE, (order.getInt32( properties::QUANTITY ) - order.getInt32( properties::QUANTITY_CHARGED )) );

							getTenderProtocolVC()->displayQuantityExceedance( (order.getInt32( properties::QUANTITY ) - order.getInt32( properties::QUANTITY_CHARGED )) );
							getTenderProtocolVC()->matchFromTenderProtocol( m_TenderProgress->get() );
							return result;
						}
					}
				}
			}
            catch( const basar::Exception & e )
            {
				BLOG_ERROR( m_Logger, e.what().c_str() );
				throw;
			}
		
            return result;
        }

		SYSTEM_EVENT_HANDLER_DEFINITION( TenderProtocolUC, CreateInvoiceRequested )
        {
            METHODNAME_DEF( TenderProtocolUC, CreateInvoiceRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;
		
            basar::appl::EventReturnType result( basar::appl::HANDLER_OK );
			
            try
            {
				getTenderProtocolVC()->matchToTenderProtocol( getTenderProgressDM()->get() );
				basar::db::aspect::AccessorPropertyTableRef        orderCollection = m_TenderProgress->get();
				basar::db::aspect::AccessorPropertyTable_YIterator order = orderCollection.begin();

				for( ; !order.isEnd(); ++order )
				{
					if( order.isContainedAndSet( properties::QUANTITY_TO_CHARGE ) &&
						0 != order.getInt32( properties::QUANTITY_TO_CHARGE ) )
					{
						getUseCaseGetter()->getInvoiceUC( getTenderProgressDM() )->run();
						basar::appl::SystemEventManager::getInstance().fire( "SearchTenderProtocols" );

						return result;
					}
				}

				getTenderProtocolVC()->displayQuantityToChargeNotSet();
			}
            catch( const basar::Exception & e )
            {
				BLOG_ERROR( m_Logger, e.what().c_str() );
				throw;
			}
		
            return result;
        }

		SYSTEM_EVENT_HANDLER_DEFINITION( TenderProtocolUC, SelectOrderRequested )
        {
            METHODNAME_DEF( TenderProtocolUC, SelectOrderRequested )
            BLOG_TRACE_METHOD( m_Logger, fun );
            rSource;
		
            basar::appl::EventReturnType result( basar::appl::HANDLER_OK );

            try
            {
				getTenderProtocolVC()->setQuantityToCharge( getTenderProgressDM()->get() );
			}
			catch( const basar::Exception & e )
            {
				BLOG_ERROR( m_Logger, e.what().c_str() );
				throw;
			}
		
            return result;
        }

    } // namespace usecase
} // namespace tender
