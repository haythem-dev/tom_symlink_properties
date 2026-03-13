//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tenderprotocolvc.h"

#include <libutil/util.h>
#include <libutil/viewconn.h>

#include "loggerpool/tenderloggerpool.h"

#include <libtender/domainmodule/tender/itenderprotocol.h>

#include "libtender/domainmodule/libtender_properties_definitions.h"
#include "qvalidatoritemdelegate.h"
#include "rowcolourtransformer.h"

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
        TenderProtocolVC::TenderProtocolVC( QWidget* parent )
        : BaseVC( parent ),
          m_EventsWiring  ( "TenderProtocolVC::EventsWiring" )
        {
            //METHODNAME_DEF( TenderProtocolVC, TenderProtocolVC )
            //BLOG_TRACE_METHOD( m_Logger, fun );

            ui.setupUi( this );

			ui.cbShowInvoicedOrders->insertItem(0, "Show invoiced" );
			ui.cbShowInvoicedOrders->insertItem(1, "Show not invoiced" );
			ui.cbShowInvoicedOrders->setCurrentIndex( 1 );

			m_rowColour= QColor::fromRgb( 178,255,102 );

            wireEvents();

            initProtocolHeaderLabels();
            setupProtocolTable();
        }

        /////////////////////////////////////////////////////////////////////////////////////////////
        /*!    \throw  no-throw */
        /////////////////////////////////////////////////////////////////////////////////////////////
        TenderProtocolVC::~TenderProtocolVC()
        {
            //METHODNAME_DEF( TenderProtocolVC, ~TenderProtocolVC )
            //BLOG_TRACE_METHOD( m_Logger, fun );
        }

		void TenderProtocolVC::wireEvents()
        {
            if( m_EventsWiring.isSet() )
            {
                return;
            }
												
			connect( ui.btnCancel,			  SIGNAL( clicked()						 ),
            this,							  SLOT  ( onDialogCloseRequested()		 ));

			connect( ui.twProtocols,		  SIGNAL( activated(const QModelIndex &) ),
			this,							  SLOT  ( onDoubleClickOrder()           ));

			connect( ui.twProtocols,		  SIGNAL( itemChanged(QTableWidgetItem *) ),
			this,							  SLOT  ( onQuantityToChargeChanged(QTableWidgetItem *) ) );
											  
			connect( ui.btnInvoice,			  SIGNAL( clicked()						 ),
			this,							  SLOT  ( onCreateInvoiceRequested()	 ));
																					 
			connect( ui.cbShowInvoicedOrders, SIGNAL( currentIndexChanged(int)		 ),
			this,							  SLOT  ( onShowCriteriaChanged(int)	 ));

            m_EventsWiring.set();
        }

		void TenderProtocolVC::setupProtocolTable()
		{
			METHODNAME_DEF( TenderProtocolVC, setupProtocolTable )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

		    ui.twProtocols->setABCVerticalHeaderPolicy();
            ui.twProtocols->setColumnCount              ( m_LstLabels.size()				 );
            ui.twProtocols->setHorizontalHeaderLabels   ( m_LstLabels						 );
            ui.twProtocols->setResizeRowsToContents     ( true                               );
            ui.twProtocols->setResizeColumnsToContents  ( true                               );
            ui.twProtocols->setVerticalScrollBarPolicy  ( Qt::ScrollBarAsNeeded              );
            ui.twProtocols->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded              );
            ui.twProtocols->setSelectionBehavior        ( QAbstractItemView::SelectRows      );
            ui.twProtocols->setSelectionMode            ( QAbstractItemView::SingleSelection );
			ui.twProtocols->setEditableColumn           ( 0, false                           );
			ui.twProtocols->setEditableColumn           ( 1, false                           );
			ui.twProtocols->setEditableColumn           ( 2, false                           );
			ui.twProtocols->setEditableColumn           ( 3, false                           );
			ui.twProtocols->setEditableColumn           ( 4, false                           );
			ui.twProtocols->setEditableColumn           ( 5, false                           );
			ui.twProtocols->setEditableColumn           ( 6, false                           );
			ui.twProtocols->setEditableColumn           ( 7, true                            );

            ui.twProtocols->setSortingEnabled(true);
            ui.twProtocols->setEnabled(true);

			const QRegExp rx("^[0]|[1-9][0-9]{0,4}$");
			QValidator *validator = new QRegExpValidator(rx, this);
			
			QValidatorItemDelegate* delegate = new QValidatorItemDelegate(this); 
			delegate->setColumnValidator( COLUMN_QUANTITY_TO_CHARGE, validator ); 
			ui.twProtocols->setItemDelegate(delegate);
		}

		basar::Int32 TenderProtocolVC::initProtocolMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
		{
			METHODNAME_DEF( TenderMaintainVC, initProtocolMatcher )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
		
			m_ProtocolMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twProtocols, propTab );
			m_ProtocolMatcher.transformProperties( ui.twProtocols->horizontalHeaderLabels(), m_LstOriginalAttributes );

			RowColourTransformer rowColourTransformer( properties::QUANTITY_TO_CHARGE.getName(), "Quantity to charge", m_rowColour, ui.twProtocols );
			m_ProtocolMatcher.push_back( rowColourTransformer );

			basar::Int32 count = 0;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();
			while (!yit.isEnd())
			{
				++count;
				++yit;
			}
			return count;
		}

		void TenderProtocolVC::initProtocolHeaderLabels()
		{
			METHODNAME_DEF( TenderProtocolVC, initProtocolHeaderLabels )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );
			
			m_LstLabels.push_back( tr( "Order date" ) );
			m_LstOriginalAttributes.push_back( properties::ORDER_DATE );

			m_LstLabels.push_back( tr( "Order no" ) );
			m_LstOriginalAttributes.push_back( properties::ORDER_NO );

			m_LstLabels.push_back( tr( "Customer" ) );
			m_LstOriginalAttributes.push_back( properties::PHARMACY_NAME );

			m_LstLabels.push_back( tr( "Article Code" ) );
			m_LstOriginalAttributes.push_back( properties::ARTICLE_NO );

			m_LstLabels.push_back( tr( "Article Name" ) );
			m_LstOriginalAttributes.push_back( properties::ARTICLE_NAME );

			m_LstLabels.push_back( tr( "Quantity" ) );
			m_LstOriginalAttributes.push_back( properties::QUANTITY );

			m_LstLabels.push_back( tr( "Charged Quantity" ) );
			m_LstOriginalAttributes.push_back( properties::QUANTITY_CHARGED );

			m_LstLabels.push_back( tr( "Quantity to charge" ) );
			m_LstOriginalAttributes.push_back( properties::QUANTITY_TO_CHARGE );
		}

		basar::gui::tie::WidgetReturnEnum TenderProtocolVC::show()
		{
			METHODNAME_DEF( TenderProtocolVC, show )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.twProtocols->selectRow( 0 );
			ui.twProtocols->setFocus();

            return basar::gui::tie::getWidgetReturnType( exec() );
		}

		int TenderProtocolVC::showInvoicedOrders()
		{
			return ui.cbShowInvoicedOrders->currentIndex();
		}

		basar::db::aspect::AccessorPropertyTable_YIterator TenderProtocolVC::getCurrentOrder( basar::db::aspect::AccessorPropertyTableRef positionPropTab )
		{
			initProtocolMatcher( positionPropTab );
			m_ProtocolMatcher.LeftToRight();

			return m_ProtocolMatcher.getSelectedRight();
		}

		void TenderProtocolVC::matchFromTenderProtocol( basar::db::aspect::AccessorPropertyTableRef protocol )
		{
			METHODNAME_DEF( TenderProtocolVC, matchFromTenderProtocol )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.twProtocols->setDisabled( true );

			basar::Int32 rowCount = initProtocolMatcher( protocol );

            libutil::gui::SignalBlocker signalblocker( ui.twProtocols );
            ui.twProtocols->setRowCount( rowCount );
			m_ProtocolMatcher.RightToLeft( 0, ui.twProtocols->rowCount() );
			ui.twProtocols->setResizeColumnsToContents( true );
            signalblocker.unblock();

            if(rowCount)
			{
				ui.twProtocols->resizeColumnsToContents();
                ui.twProtocols->setEnabled(true);
			
                ui.twProtocols->setFocus();
            }
		}

		void TenderProtocolVC::matchToTenderProtocol( basar::db::aspect::AccessorPropertyTableRef protocol )
		{
			METHODNAME_DEF( TenderProtocolVC, matchToTenderProtocol )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			initProtocolMatcher( protocol );
			m_ProtocolMatcher.LeftToRight();
		}

		void TenderProtocolVC::onDialogCloseRequested()
		{
			METHODNAME_DEF( TenderProtocolVC, onDialogCloseRequested )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			this->close();
		}

		void TenderProtocolVC::setQuantityToCharge( basar::db::aspect::AccessorPropertyTableRef protocols )
		{
			int row = ui.twProtocols->currentRow();
			basar::db::aspect::AccessorPropertyTable_YIterator order =  getCurrentOrder(protocols);
			QString quantity = QString::number( order.getInt32( properties::QUANTITY ) - order.getInt32( properties::QUANTITY_CHARGED ) );

			if( 0 == order.getInt32( properties::QUANTITY_TO_CHARGE ) )
			{
				ui.twProtocols->item( row, COLUMN_QUANTITY_TO_CHARGE )->setText( quantity );
			}
			else
			{
				ui.twProtocols->item( row, COLUMN_QUANTITY_TO_CHARGE )->setText( "0" );
			}	
		}

		void TenderProtocolVC::onQuantityToChargeChanged( int, int column )
		{
			if( COLUMN_QUANTITY_TO_CHARGE == column )
			{
				basar::appl::SystemEventManager::getInstance().fire( "ChangeQuantityToChargeRequested" );
			}
		}

		void TenderProtocolVC::onQuantityToChargeChanged( QTableWidgetItem * item )
		{
			if( COLUMN_QUANTITY_TO_CHARGE == item->column() )
			{
				ui.twProtocols->blockSignals( true );
				basar::appl::SystemEventManager::getInstance().fire( "ChangeQuantityToChargeRequested" );
				ui.twProtocols->blockSignals( false );
			}
		}

		void TenderProtocolVC::onCreateInvoiceRequested()
		{
			METHODNAME_DEF( TenderProtocolVC, onCreateInvoiceRequested )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			basar::appl::SystemEventManager::getInstance().fire( "CreateInvoiceRequested" );
		}

		void TenderProtocolVC::onShowCriteriaChanged( int )
		{
			METHODNAME_DEF( TenderProtocolVC, onShowCriteriaChanged )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
		
			basar::appl::SystemEventManager::getInstance().fire( "SearchTenderProtocols" );
		}

		void TenderProtocolVC::onDoubleClickOrder()
		{
			METHODNAME_DEF( TenderProtocolVC, onDoubleClickOrder )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			basar::appl::SystemEventManager::getInstance().fire( "SelectOrderRequested" );
		}

		void TenderProtocolVC::displayNoProtocolFound()
		{
			METHODNAME_DEF( TenderProtocolVC, displayNoProtocolFound )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			showInfoMessage( tr("No protocols are found for this tender.").toLocal8Bit().constData() );
		}

		void TenderProtocolVC::displayQuantityExceedance( int maxQuantity  )
		{
			METHODNAME_DEF( TenderProtocolVC, displayQuantityExceedance )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			std::stringstream ss;
			ss << maxQuantity;

			showWarningMessage( "Quantity to charge for this article exceeded. \nAvailable quantity is " + ss.str() + ".", "" );
		}

		void TenderProtocolVC::displayQuantityToChargeNotSet()
		{
			METHODNAME_DEF( TenderProtocolVC, displayQuantityToChargeNotSet )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			showInfoMessage( tr("Quantity to charge is not set for any order.").toLocal8Bit().constData() );
		}

	} //end namespace viewConn
} //end namespace tender
