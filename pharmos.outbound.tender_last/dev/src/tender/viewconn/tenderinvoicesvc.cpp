//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tenderinvoicesvc.h"
#include "domainmodule/libtender_properties_definitions.h"

#include <libutil/util.h>
#include <libutil/viewconn.h>

#include "loggerpool/tenderloggerpool.h"


//-------------------------------------------------------------------------------------------------//
// using declaration section
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace viewConn
    {
		/////////////////////////////////////////////////////////////////////////////////////////////
        /*!    \throw  no-throw */
        /////////////////////////////////////////////////////////////////////////////////////////////
        TenderInvoicesVC::TenderInvoicesVC( QWidget* parent )
        : BaseVC( parent ),
          m_EventsWiring  ( "TenderInvoicesVC::EventsWiring" )
        {
            //METHODNAME_DEF( TenderInvoicesVC, TenderInvoicesVC )
            //BLOG_TRACE_METHOD( m_Logger, fun );

            ui.setupUi( this );
            wireEvents();

            //initDeliveryHeaderLabels();
            //setupDeliveryTables();

			ui.teProtocolsForInvoice->setReadOnly( true );
        }

        /////////////////////////////////////////////////////////////////////////////////////////////
        /*!    \throw  no-throw */
        /////////////////////////////////////////////////////////////////////////////////////////////
        TenderInvoicesVC::~TenderInvoicesVC()
        {
            //METHODNAME_DEF( TenderInvoicesVC, ~TenderInvoicesVC )
            //BLOG_TRACE_METHOD( m_Logger, fun );
        }

		void TenderInvoicesVC::wireEvents()
        {
            if( m_EventsWiring.isSet() )
            {
                return;
            }

            connect( ui.btnYes,	SIGNAL( clicked()                   ),
            this,				SLOT  ( onCreateInvoicesRequested() ));

			connect( ui.btnNo,	SIGNAL( clicked()                   ),
            this,				SLOT  ( onDialogCloseRequested()    ));
										     
            m_EventsWiring.set();
        }

		basar::gui::tie::WidgetReturnEnum TenderInvoicesVC::show()
		{
			METHODNAME_DEF( TenderInvoicesVC, show )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

            return basar::gui::tie::getWidgetReturnType( exec() );
		}

		void TenderInvoicesVC::matchFromTenderProtocol( basar::db::aspect::AccessorPropertyTableRef protocol )
		{
			METHODNAME_DEF( TenderInvoicesVC, matchFromTenderProtocol )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			try
			{
				QString ProtocolForInvoice;

				basar::db::aspect::AccessorPropertyTable_YIterator article = protocol.begin();

				for( ; !article.isEnd(); ++article )
				{
					if( article.isContainedAndSet( properties::QUANTITY_TO_CHARGE ) )
					{
						if( 0 < article.getInt32( properties::QUANTITY_TO_CHARGE ) )
						{
							ProtocolForInvoice = QString("%1: %2%3 %4: %5 %6: %7/%8")
							.arg( "Protocol" )
							.arg( article.getDate( properties::ORDER_DATE ).toStrDate().c_str() )
							.arg( article.getInt32( properties::ORDER_NO ) )
							.arg( "Article: " ) 
							.arg( article.getInt32( properties::ARTICLE_NO ) )
							.arg( "Quantity: " )
							.arg( article.getInt32( properties::QUANTITY_TO_CHARGE  ) )
							.arg( article.getInt32( properties::QUANTITY ) );
						
							ui.teProtocolsForInvoice->append( ProtocolForInvoice );
						}
					}
				}
			}
			catch( const basar::Exception & e )
            {
				BLOG_ERROR(  libtender::LoggerPool::getLoggerViewConn(), e.what().c_str() );
				throw;
			}
		}

		void TenderInvoicesVC::onDialogCloseRequested()
		{
			METHODNAME_DEF( TenderInvoicesVC, onDialogCloseRequested )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			this->close();
		}

		void TenderInvoicesVC::onCreateInvoicesRequested()
		{
			METHODNAME_DEF( TenderInvoicesVC, onCreateInvoicesRequested )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			basar::appl::SystemEventManager::getInstance().fire( "CreateTenderInvoiceRequested" );
		}


	} //end namespace viewConn
} //end namespace tender
