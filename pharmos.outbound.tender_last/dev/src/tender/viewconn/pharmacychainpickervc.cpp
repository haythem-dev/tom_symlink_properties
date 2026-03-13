#include "pharmacychainpickervc.h"
#include "loggerpool/tenderloggerpool.h"
#include "libtender/domainmodule/libtender_properties_definitions.h"

#include <util.h>


namespace tender
{
    namespace viewConn
    {
    	PharmacyChainPickerVC::PharmacyChainPickerVC( QWidget* parent )
    	:BaseVC( parent ),
    	 m_EventsWiring( "PharmacyChainPickerVC : EventsWiring" )
    	{
    		ui.setupUi( this );
    		wireEvents();

            initPharmacyChainHeaderLabels();
    		setupPharmacyChainTable();
			setupSearchCriteria();

			ui.btnTakeOver->setDisabled( true );
		}

		PharmacyChainPickerVC::~PharmacyChainPickerVC()
		{

		}

		void PharmacyChainPickerVC::wireEvents()
		{
			if( m_EventsWiring.isSet() )
			{
				return;
			}

			connect(ui.btnCancel,			SIGNAL( clicked()                      ),
			this,							SLOT  ( on_DialogClose_Requested()     ));

			connect( ui.twPharmacyChain,	SIGNAL( activated(const QModelIndex &) ),
            this,							SLOT  ( on_TakeOver_Requested()        ));

			connect(ui.btnSearch,			SIGNAL( clicked()                      ),
			this,							SLOT  ( on_Search_Requested()          ));

			connect(ui.btnTakeOver,			SIGNAL( clicked()                      ),
			this,							SLOT  ( on_TakeOver_Requested()        ));

			m_EventsWiring.set();
		}

		void PharmacyChainPickerVC::matchFromPharmacyChainCollection( basar::db::aspect::AccessorPropertyTableRef propTab )
		{
			METHODNAME_DEF( PharmacyChainPickerVC, matchFromPharmacyChainCollection )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.twPharmacyChain->setDisabled( true );

			basar::Int32 rowCount = initMatcher(propTab);

			libutil::gui::SignalBlocker signalblocker( ui.twPharmacyChain );
            ui.twPharmacyChain->setRowCount( rowCount );
			m_PharmacyChainMatcher.RightToLeft( 0, ui.twPharmacyChain->rowCount() );
			signalblocker.unblock();

			ui.twPharmacyChain->resizeColumnsToContents();

			if(rowCount)
            {
                ui.twPharmacyChain->setEnabled( true );

                ui.twPharmacyChain->selectRow(0);
                ui.twPharmacyChain->setFocus();

				ui.btnTakeOver->setEnabled( true );
            }
		}

		void PharmacyChainPickerVC::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
		{
			METHODNAME_DEF( PharmacyChainPickerVC, matchToSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( !ui.lePharmacyChainNo->text().isEmpty() )
			{
				yitSearch.setInt32( properties::PHARMACY_CHAIN_NO, ui.lePharmacyChainNo->text().toInt() );
			}

			if( !ui.leCustomerNo->text().isEmpty() )
			{
				yitSearch.setInt32( properties::CUSTOMER_NO, ui.leCustomerNo->text().toInt() );
			}

			if( !ui.leCustomerName->text().isEmpty() )
			{
				yitSearch.setString( properties::PHARMACY_NAME, ui.leCustomerName->text().toLocal8Bit().constData() );
			}
		}

		basar::db::aspect::AccessorPropertyTable_YIterator PharmacyChainPickerVC::getPickedPharmacyChain()
		{
			METHODNAME_DEF( PharmacyChainPickerVC, getPickedPharmacyChain )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			return m_PickedPharmacyChain;
		}

		basar::gui::tie::WidgetReturnEnum PharmacyChainPickerVC::show()
        {
            METHODNAME_DEF( PharmacyChainPickerVC, show )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			m_PickedPharmacyChain.clear();

            return basar::gui::tie::getWidgetReturnType( exec() );
        }

		void PharmacyChainPickerVC::clear()
		{
			METHODNAME_DEF( PharmacyChainPickerVC, clear )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.twPharmacyChain->setRowCount( 0 );
			ui.twPharmacyChain->setDisabled( true );

			ui.lePharmacyChainNo->setText  ( "" );
			ui.leCustomerNo->setText( "" );
			ui.leCustomerName->setText( "" );

			ui.btnTakeOver->setDisabled( true );
		}

		void PharmacyChainPickerVC::setupPharmacyChainTable()
		{
			METHODNAME_DEF( PharmacyChainPickerVC, setupPharmacyChainTable )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

		    ui.twPharmacyChain->setABCVerticalHeaderPolicy  ();
            ui.twPharmacyChain->setColumnCount              ( m_LstLabels.size()                 );
            ui.twPharmacyChain->setHorizontalHeaderLabels   ( m_LstLabels                        );
            ui.twPharmacyChain->setResizeRowsToContents     ( true                               );
            ui.twPharmacyChain->setResizeColumnsToContents  ( true                               );
            ui.twPharmacyChain->setVerticalScrollBarPolicy  ( Qt::ScrollBarAlwaysOn              );
            ui.twPharmacyChain->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded              );
            ui.twPharmacyChain->setSelectionBehavior        ( QAbstractItemView::SelectRows      );
            ui.twPharmacyChain->setSelectionMode            ( QAbstractItemView::SingleSelection );
			ui.twPharmacyChain->setSortingEnabled           ( false );
			ui.twPharmacyChain->setDisabled                 ( true  );
		}

		void PharmacyChainPickerVC::initPharmacyChainHeaderLabels()
		{
			METHODNAME_DEF( PharmacyChainPickerVC, initPharmacyChainHeaderLabels )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			m_LstLabels.push_back            ( tr("Pharmacy chain no")			);
			m_LstOriginalAttributes.push_back( properties::PHARMACY_CHAIN_NO	);

			m_LstLabels.push_back            ( tr("Customer no")				);
			m_LstOriginalAttributes.push_back( properties::CUSTOMER_NO			);

			m_LstLabels.push_back            ( tr("Customer name")				);
			m_LstOriginalAttributes.push_back( properties::PHARMACY_NAME		);

			m_LstLabels.push_back            ( tr("Street")						);
			m_LstOriginalAttributes.push_back( properties::PHARMACY_STREET		);

			m_LstLabels.push_back            ( tr("Location")					);
			m_LstOriginalAttributes.push_back( properties::PHARMACY_LOCATION	);
		}

		basar::Int32 PharmacyChainPickerVC::initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
        {
            METHODNAME_DEF( TenderOverviewVC, initMatcher )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            m_PharmacyChainMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twPharmacyChain, propTab );
            m_PharmacyChainMatcher.transformProperties( ui.twPharmacyChain->horizontalHeaderLabels(), m_LstOriginalAttributes );

			basar::Int32 count = 0;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();

			while (!yit.isEnd())
			{
				++count;
				++yit;
			}
			return count;
        }

		void PharmacyChainPickerVC::on_DialogClose_Requested()
		{
			METHODNAME_DEF( PharmacyChainPickerVC, on_DialogClose_Requested )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			this->reject();
		}

		void PharmacyChainPickerVC::on_Search_Requested()
		{
			METHODNAME_DEF( PharmacyChainPickerVC, on_Search_Requested )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( ui.lePharmacyChainNo->text().isEmpty() &&
				ui.leCustomerNo->text().isEmpty() &&
				ui.leCustomerName->text().isEmpty() )
			{
				showWarningMessage( tr("No search criteria.").toLocal8Bit().constData(), "" );
				ui.lePharmacyChainNo->setFocus();

				return;
			}

			if( ui.lePharmacyChainNo->text().isEmpty() &&
				ui.leCustomerNo->text().isEmpty() &&
				3 > ui.leCustomerName->text().count() )
			{
				showInfoMessage( tr("Please enter at least 3 characters into the customer name field.").toLocal8Bit().constData() );
				ui.leCustomerName->setFocus();
				ui.leCustomerName->selectAll();

				return;
			}

			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "SearchPharmacyChainRequested" );
		}

		void PharmacyChainPickerVC::on_TakeOver_Requested()
		{
			METHODNAME_DEF( PharmacyChainPickerVC, on_TakeOver_Requested )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			m_PickedPharmacyChain = m_PharmacyChainMatcher.getCurrentRight();
			this->reject();
		}

		void PharmacyChainPickerVC::focusInEvent( QFocusEvent* event)
        {
            event;
			ui.lePharmacyChainNo->setFocus();
        }

		void PharmacyChainPickerVC::displayNoPharmacyChainFound()
		{
			showInfoMessage( tr("No pharmacy chain found").toLocal8Bit().constData() );

			ui.btnTakeOver->setDisabled( true );

			if( !ui.lePharmacyChainNo->text().isEmpty() )
			{
				ui.lePharmacyChainNo->setFocus();
				ui.lePharmacyChainNo->selectAll();
				return;
			}
		}

		void PharmacyChainPickerVC::setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
            METHODNAME_DEF( PharmacyChainPickerVC, setSearchCriteria )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( yit.isContainedAndSet( properties::PHARMACY_CHAIN_NO ) )
			{
				ui.lePharmacyChainNo->setText( QString::number( yit.getInt32( properties::PHARMACY_CHAIN_NO ) ) );
			}
			else
			{
				ui.lePharmacyChainNo->setText( "" );
			}
		}

		void PharmacyChainPickerVC::setupSearchCriteria()
		{
			METHODNAME_DEF( PharmacyChainPickerVC, setupSearchCriteria )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.lePharmacyChainNo->setAccessibleName( properties::PHARMACY_CHAIN_NO.getName().c_str() );

			QRegExp rxPharmacyChainNo( "^[0-9]{1,7}$" );
			QValidator *pharmacyChainNoValidator = new QRegExpValidator( rxPharmacyChainNo, ui.lePharmacyChainNo );
            ui.lePharmacyChainNo->setValidator( pharmacyChainNoValidator );

			ui.leCustomerNo->setAccessibleName( properties::CUSTOMER_NO.getName().c_str()    );

			QRegExp rxCustomerNo( "^[1-9][0-9]{1,7}$" );
			QValidator *customerNoValidator = new QRegExpValidator( rxCustomerNo, ui.leCustomerNo );
            ui.leCustomerNo->setValidator( customerNoValidator );

			ui.leCustomerName->setAccessibleName( properties::PHARMACY_NAME.getName().c_str() );

			QRegExp rxPharmacyName( "^([a-zA-Z0-9]+\\s)*$" );
			QValidator *pharmacyNameValidator = new QRegExpValidator( rxPharmacyName, ui.leCustomerName );
			ui.leCustomerName->setValidator( pharmacyNameValidator );
		}

		void PharmacyChainPickerVC::reject()
		{
			clear();
			QDialog::reject();
		}

    } //end namespace viewConn
} //end namespace tender
