#include "customerpickervc.h"
#include "libtender/domainmodule/libtender_properties_definitions.h"
#include "loggerpool/tenderloggerpool.h"

namespace tender
{
    namespace viewConn
    {
    	CustomerPickerVC::CustomerPickerVC( QWidget* parent )
    	:BaseVC( parent ),
    	 m_EventsWiring( "CustomerPickerVC : EventsWiring" )
    	{
    		ui.setupUi( this );
    		wireEvents();

            initCustomerHeaderLabels();
    		setupCustomerTable();
			setupSearchCriteria();

			ui.btnTakeOver->setDisabled( true );
    	}

		CustomerPickerVC::~CustomerPickerVC()
		{
		}

		void CustomerPickerVC::wireEvents()
		{
			if( m_EventsWiring.isSet() )
			{
				return;
			}
			 
			connect( ui.btnCancel,   SIGNAL( clicked()                      ),
			this,                    SLOT  ( on_DialogClose_Requested()     ));
																		    
			connect( ui.btnSearch,   SIGNAL( clicked()                      ),
			this,                    SLOT  ( on_Search_Requested()          ));
																		    
			connect( ui.twCustomer,  SIGNAL( activated(const QModelIndex &) ),
			this,                    SLOT  ( on_TakeOver_Requested()        ));
																		    
			connect( ui.btnTakeOver, SIGNAL( clicked()                      ),
			this,                    SLOT  ( on_TakeOver_Requested()        ));
 
			m_EventsWiring.set();
		}

		void CustomerPickerVC::matchFromCustomerCollection( basar::db::aspect::AccessorPropertyTableRef propTab )
		{
			METHODNAME_DEF( CustomerPickerVC, matchFromCustomerCollection )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.twCustomer->setDisabled( true );
			basar::Int32 rowCount = initMatcher(propTab);
			libutil::gui::SignalBlocker signalblocker( ui.twCustomer );
            ui.twCustomer->setRowCount( rowCount );
			m_CustomerMatcher.RightToLeft( 0, ui.twCustomer->rowCount() );

			signalblocker.unblock();

			ui.twCustomer->resizeColumnsToContents();

			if( rowCount )
            {
                ui.twCustomer->setEnabled( true );

                ui.twCustomer->selectRow(0);
                ui.twCustomer->setFocus();

				ui.btnTakeOver->setEnabled( true );
            }
		}

		void CustomerPickerVC::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
		{
			METHODNAME_DEF( CustomerPickerVC, matchToSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( !ui.leIdfNo->text().isEmpty() )
			{
				yitSearch.setInt32( properties::CUSTOMER_NO, ui.leIdfNo->text().toInt() );
			}

			if( !ui.lePharmacyName->text().isEmpty() )
			{
				yitSearch.setString( properties::PHARMACY_NAME, ui.lePharmacyName->text().toLocal8Bit().data() );
			}
		}

		void CustomerPickerVC::focusInEvent( QFocusEvent* event )
        {
            event;
			ui.leIdfNo->setFocus();
        }

		void CustomerPickerVC::setCustomerNo( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( CustomerPickerVC, setCustomerNo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( yit.isContainedAndSet(properties::CUSTOMER_NO) )
			{
				ui.leIdfNo->setText( QString::number(yit.getInt32(properties::CUSTOMER_NO )) );
			}
			else 
			{
				ui.leIdfNo->setText( "" );
			}
		}

		basar::db::aspect::AccessorPropertyTable_YIterator CustomerPickerVC::getPickedCustomer()
		{
            METHODNAME_DEF( CustomerPickerVC, getPickedCustomer )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			return m_PickedCustomer;
		}

		basar::gui::tie::WidgetReturnEnum CustomerPickerVC::show()
        {
            METHODNAME_DEF( CustomerPickerVC, show )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
			
			m_PickedCustomer.clear();

            return basar::gui::tie::getWidgetReturnType( exec() );
        }

		void CustomerPickerVC::clear()
		{
			METHODNAME_DEF( CustomerPickerVC, clear )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.twCustomer->setRowCount( 0 );
			ui.twCustomer->setDisabled( true );

			ui.leIdfNo->setText       ( "" );
			ui.lePharmacyName->setText( "" );

			ui.btnTakeOver->setDisabled( true );
		}

		void CustomerPickerVC::setupCustomerTable()
		{
			METHODNAME_DEF( CustomerPickerVC, setupCustomerTable )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

		    ui.twCustomer->setABCVerticalHeaderPolicy  ();
            ui.twCustomer->setColumnCount              ( m_LstLabels.size()                 );
            ui.twCustomer->setHorizontalHeaderLabels   ( m_LstLabels                        );
            ui.twCustomer->setResizeRowsToContents     ( true                               );
            ui.twCustomer->setResizeColumnsToContents  ( true                               );
            ui.twCustomer->setVerticalScrollBarPolicy  ( Qt::ScrollBarAlwaysOn              );
            ui.twCustomer->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded              );
            ui.twCustomer->setSelectionBehavior        ( QAbstractItemView::SelectRows      );
            ui.twCustomer->setSelectionMode            ( QAbstractItemView::SingleSelection );
			ui.twCustomer->setSortingEnabled           ( false );
			ui.twCustomer->setDisabled                 ( true  );
		}

		void CustomerPickerVC::initCustomerHeaderLabels()
		{
			METHODNAME_DEF( CustomerPickerVC, initCustomerHeaderLabels )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			m_LstLabels.push_back            ( tr("Customer No.")			  );
			m_LstOriginalAttributes.push_back( properties::CUSTOMER_NO		  ); 
																			  
			m_LstLabels.push_back            ( tr("Pharmacy Name")			  );
			m_LstOriginalAttributes.push_back( properties::PHARMACY_NAME	  );
																			  
			m_LstLabels.push_back            ( tr("Street")					  );
			m_LstOriginalAttributes.push_back( properties::PHARMACY_STREET	  );
																			  
			m_LstLabels.push_back            ( tr("Location")				  );
			m_LstOriginalAttributes.push_back( properties::PHARMACY_LOCATION  );
			
			m_LstLabels.push_back            ( tr("Branch No")                );
			m_LstOriginalAttributes.push_back( properties::BRANCH_NO		  );

			m_LstLabels.push_back            ( tr("Original Branch No")       );
			m_LstOriginalAttributes.push_back( properties::ORIGINAL_BRANCH_NO );
		}

		basar::Int32 CustomerPickerVC::initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
        {
            METHODNAME_DEF( CustomerPickerVC, initMatcher )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            m_CustomerMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twCustomer, propTab );
            m_CustomerMatcher.transformProperties( ui.twCustomer->horizontalHeaderLabels(), m_LstOriginalAttributes );

			basar::Int32 count = 0;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();
			while (!yit.isEnd())
			{
				++count;
				++yit;
			}
			return count;
        }

		void CustomerPickerVC::on_DialogClose_Requested()
		{
			METHODNAME_DEF( CustomerPickerVC, on_DialogClose_Requested )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			this->reject();
		}

		void CustomerPickerVC::on_Search_Requested()
		{
			METHODNAME_DEF( CustomerPickerVC, on_Search_Requested )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( ui.leIdfNo->text().isEmpty() && ui.lePharmacyName->text().isEmpty() )
			{
				showWarningMessage( tr("No search criteria.").toLocal8Bit().constData() );
				ui.leIdfNo->setFocus();

				return;
			}

			if( ui.leIdfNo->text().isEmpty() &&
				3 > ui.lePharmacyName->text().count() )
			{
				showInfoMessage( tr("Please enter at least 3 characters into the pharmacy name field.").toLocal8Bit().constData() );
				ui.lePharmacyName->setFocus();
				ui.lePharmacyName->selectAll();

				return;
			}

			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "SearchCustomerRequested" );
		}

		void CustomerPickerVC::on_TakeOver_Requested()
		{
			METHODNAME_DEF( CustomerPickerVC, on_TakeOver_Requested )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			m_PickedCustomer = m_CustomerMatcher.getCurrentRight();
			this->reject();
		}

		void CustomerPickerVC::displayNoCustomerFound()
		{
			showInfoMessage( tr("No customer found.").toLocal8Bit().constData() );

			ui.btnTakeOver->setDisabled( true );

			if( !ui.leIdfNo->text().isEmpty() )
			{
				ui.leIdfNo->setFocus();
				ui.leIdfNo->selectAll();
				return;
			}

			if( !ui.lePharmacyName->text().isEmpty() )
			{
				ui.lePharmacyName->setFocus();
				ui.lePharmacyName->selectAll();
			}
		}

		void CustomerPickerVC::setupSearchCriteria()
		{
			METHODNAME_DEF( CustomerPickerVC, setupSearchCriteria )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.leIdfNo->setAccessibleName( properties::CUSTOMER_NO.getName().c_str()    );

			QRegExp rxCustomerNo( "^[1-9][0-9]{1,7}$" );
			QValidator *customerNoValidator = new QRegExpValidator( rxCustomerNo, ui.leIdfNo );
            ui.leIdfNo->setValidator( customerNoValidator );

			ui.lePharmacyName->setAccessibleName( properties::PHARMACY_NAME.getName().c_str() );

			QRegExp rxPharmacyName( "^([^\\W]+\\s)*" );
			QValidator *pharmacyNameValidator = new QRegExpValidator( rxPharmacyName, ui.lePharmacyName );
			ui.lePharmacyName->setValidator( pharmacyNameValidator );
		}

		void CustomerPickerVC::reject()
		{
			clear();
			QDialog::reject();
		}

	} // end namespace viewConn
} // end namespace tender
