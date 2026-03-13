//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <partnerpickervc.h>
#include <libtender/domainmodule/tender/tendertypecollection.h>
#include <util.h>

#include "loggerpool/tenderloggerpool.h"

#include "libtender/domainmodule/libtender_properties_definitions.h"

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
    	PartnerPickerVC::PartnerPickerVC( QWidget* parent )
    	:BaseVC( parent ),
    	 m_EventsWiring( "PartnerPickerVC : EventsWiring" )
    	{
    		ui.setupUi( this );
    		wireEvents();

            initPartnerHeaderLabels();

    		setupPartnerTable();
			setupSearchCriteria();
    	}
    
    	/////////////////////////////////////////////////////////////////////////////////////////////
        /*!    \throw  no-throw */
        /////////////////////////////////////////////////////////////////////////////////////////////
		PartnerPickerVC::~PartnerPickerVC()
		{

		}

		void PartnerPickerVC::injectTenderTypeCollection( libtender::domMod::tender::TenderTypeCollectionPtr tenderTypeCollection )
		{
			m_TenderTypeCollection = tenderTypeCollection;

			using namespace libtender::domMod::tender;
			TenderTypeCollection::const_iterator iter = m_TenderTypeCollection->begin();
			for( ; iter != m_TenderTypeCollection->end(); ++iter )
			{
			    ui.cbBusinessType->addItem( QString::fromLocal8Bit( iter->second.c_str() ), QVariant( iter->first ) );
			}
		}

		void PartnerPickerVC::wireEvents()
		{
			if( m_EventsWiring.isSet() )
			{
				return;
			}

			connect( ui.btnCancel,   SIGNAL( clicked()                      ),
			this,                    SLOT  ( on_DialogClose_Requested()     ));

			connect( ui.btnSearch,   SIGNAL( clicked()                      ),
			this,                    SLOT  ( on_Search_Requested()          ));

			connect( ui.twPartner,   SIGNAL( activated(const QModelIndex &) ),
            this,                    SLOT  ( on_TakeOver_Requested()        ));

			connect( ui.btnTakeOver, SIGNAL( clicked()                      ),
			this,                    SLOT  ( on_TakeOver_Requested()        ));

			m_EventsWiring.set();
		}

		void PartnerPickerVC::matchFromPartnerCollection( basar::db::aspect::AccessorPropertyTableRef propTab )
		{
			METHODNAME_DEF( PartnerPickerVC, matchFromPartnerCollection )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.twPartner->setDisabled( true );
			basar::Int32 rowCount = initMatcher(propTab);
			libutil::gui::SignalBlocker signalblocker( ui.twPartner );
            ui.twPartner->setRowCount( rowCount );
			m_PartnerMatcher.RightToLeft( 0, ui.twPartner->rowCount() );
			signalblocker.unblock();

			ui.twPartner->resizeColumnsToContents();

			if( rowCount )
            {
                ui.twPartner->setEnabled( true );

                ui.twPartner->selectRow(0);
                ui.twPartner->setFocus();

				ui.btnTakeOver->setEnabled( true );
            }
		}

		void PartnerPickerVC::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
		{
			METHODNAME_DEF( PartnerPickerVC, matchToSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( !ui.lePartnerNo->text().isEmpty() )
			{
				yitSearch.setInt32( properties::PARTNER_NO, ui.lePartnerNo->text().toInt() );
			}

			if( !ui.lePartnerName->text().isEmpty() )
			{
				yitSearch.setString( properties::PARTNER_NAME, ui.lePartnerName->text().toStdString() );
			}

			if( ui.cbBusinessType->currentIndex() <= 3 )
			{
				yitSearch.setInt32( properties::TENDER_TYPE, ui.cbBusinessType->currentData().toInt() );
			}
		}

		void PartnerPickerVC::focusInEvent( QFocusEvent* event )
        {
            event;
			ui.lePartnerNo->setFocus();
        }

		void PartnerPickerVC::setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearchCriteria )
		{
			METHODNAME_DEF( PartnerPickerVC, setSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( yitSearchCriteria.isContainedAndSet(properties::PARTNER_NO) )
			{
				ui.lePartnerNo->setText( QString::number(yitSearchCriteria.getInt32(properties::PARTNER_NO )) );
			}
			else
			{
				ui.lePartnerNo->setText( "" );
			}

			if( yitSearchCriteria.isContainedAndSet( properties::TENDER_TYPE ) )
			{	
				ComboBox::setCurrentIndex( ui.cbBusinessType, yitSearchCriteria.getInt32( properties::TENDER_TYPE ) );
			}
			else
			{
				ui.cbBusinessType->addItem("All", ui.cbBusinessType->count() );
				ui.cbBusinessType->setCurrentIndex( ui.cbBusinessType->count() - 1 );
			}	
		}

		basar::db::aspect::AccessorPropertyTable_YIterator PartnerPickerVC::getPickedPartner()
		{
            METHODNAME_DEF( PartnerPickerVC, getPickedPartner )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			return m_PickedPartner;
		}

		basar::gui::tie::WidgetReturnEnum PartnerPickerVC::show()
        {
            METHODNAME_DEF( PartnerPickerVC, show )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
			
			m_PickedPartner.clear();

            return basar::gui::tie::getWidgetReturnType( exec() );
        }

		void PartnerPickerVC::clear()
		{
			METHODNAME_DEF( PartnerPickerVC, clear )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.twPartner->setRowCount( 0 );
			ui.twPartner->setDisabled( true );

			ui.lePartnerNo->setText  ( "" );
			ui.lePartnerName->setText( "" );

			ui.btnTakeOver->setDisabled( true );
		}

		void PartnerPickerVC::setupPartnerTable()
		{
			METHODNAME_DEF( PartnerPickerVC, setupPartnerTable )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

		    ui.twPartner->setABCVerticalHeaderPolicy  ();
            ui.twPartner->setColumnCount              ( m_LstLabels.size()                 );
            ui.twPartner->setHorizontalHeaderLabels   ( m_LstLabels                        );
            ui.twPartner->setResizeRowsToContents     ( true                               );
            ui.twPartner->setResizeColumnsToContents  ( true                               );
            ui.twPartner->setVerticalScrollBarPolicy  ( Qt::ScrollBarAlwaysOn              );
            ui.twPartner->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded              );
            ui.twPartner->setSelectionBehavior        ( QAbstractItemView::SelectRows      );
            ui.twPartner->setSelectionMode            ( QAbstractItemView::SingleSelection );
			ui.twPartner->setSortingEnabled           ( false );
			ui.twPartner->setDisabled                 ( true  );

			/*ui.twPartner->setColumnWidth( 0, 85  );
			ui.twPartner->setColumnWidth( 1, 190 );
			ui.twPartner->setColumnWidth( 2, 175 );
			ui.twPartner->setColumnWidth( 3, 140 );*/
		}

		void PartnerPickerVC::initPartnerHeaderLabels()
		{
			METHODNAME_DEF( PartnerPickerVC, initPartnerHeaderLabels )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			m_LstLabels.push_back            ( QApplication::translate("PartnerPickerVC", "Partner No.")      );
			m_LstOriginalAttributes.push_back( properties::PARTNER_NO );

			m_LstLabels.push_back            ( QApplication::translate("PartnerPickerVC", "Partner Name")      );
			m_LstOriginalAttributes.push_back( properties::PARTNER_NAME );

			m_LstLabels.push_back            ( QApplication::translate("PartnerPickerVC", "Street")      );
			m_LstOriginalAttributes.push_back( properties::PARTNER_STREET );

			m_LstLabels.push_back            ( QApplication::translate("PartnerPickerVC", "Location")      );
			m_LstOriginalAttributes.push_back( properties::PARTNER_LOCATION );
		}

		basar::Int32 PartnerPickerVC::initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
        {
            METHODNAME_DEF( PartnerPickerVC, initMatcher )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

            m_PartnerMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twPartner, propTab );
            m_PartnerMatcher.transformProperties( ui.twPartner->horizontalHeaderLabels(), m_LstOriginalAttributes );

			basar::Int32 count = 0;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();
			while (!yit.isEnd())
			{
				++count;
				++yit;
			}
			return count;
        }

		void PartnerPickerVC::on_DialogClose_Requested()
		{
			METHODNAME_DEF( PartnerPickerVC, on_DialogClose_Requested )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			this->reject();
		}

		void PartnerPickerVC::on_Search_Requested()
		{
			METHODNAME_DEF( PartnerPickerVC, on_Search_Requested )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( ui.lePartnerNo->text().isEmpty() && ui.lePartnerName->text().isEmpty() )
			{
				showWarningMessage( QApplication::translate("PartnerPickerVC", "No search criteria.").toLocal8Bit().constData() );
				ui.lePartnerNo->setFocus();

				return;
			}

			if( ui.lePartnerNo->text().isEmpty() &&
				3 > ui.lePartnerName->text().count() )
			{
				showInfoMessage( QApplication::translate("PartnerPickerVC", "Please enter at least 3 characters into the partner name field.").toLocal8Bit().constData() );
				ui.lePartnerName->setFocus();
				ui.lePartnerName->selectAll();

				return;
			}


			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "SearchPartnerRequested" );
		}

		void PartnerPickerVC::on_TakeOver_Requested()
		{
			METHODNAME_DEF( PartnerPickerVC, on_TakeOver_Requested )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			m_PickedPartner = m_PartnerMatcher.getCurrentRight();
			this->reject();
		}

		void PartnerPickerVC::displayNoPartnerFound()
		{
			showInfoMessage( QApplication::translate("PartnerPickerVC", "No partner found.").toLocal8Bit().constData() );

			ui.btnTakeOver->setDisabled( true );

			if( !ui.lePartnerNo->text().isEmpty() )
			{
				ui.lePartnerNo->setFocus();
				ui.lePartnerNo->selectAll();
				return;
			}

			if( !ui.lePartnerName->text().isEmpty() )
			{
				ui.lePartnerName->setFocus();
				ui.lePartnerName->selectAll();
			}
		}

		void PartnerPickerVC::setupSearchCriteria()
		{
			METHODNAME_DEF( PartnerPickerVC, setupSearchCriteria )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.lePartnerNo->setAccessibleName( properties::PARTNER_NO.getName().c_str() );

			QRegExp rxPartnerNo( "^[1-9][0-9]{1,7}$" );
			QValidator *partnerNoValidator = new QRegExpValidator( rxPartnerNo, ui.lePartnerNo );
            ui.lePartnerNo->setValidator( partnerNoValidator );

			ui.lePartnerName->setAccessibleName( properties::PARTNER_NAME.getName().c_str() );

			QRegExp rxPartnerName( "^([a-zA-Z0-9]+\\s)*$" );
			QValidator *partnerNameValidator = new QRegExpValidator( rxPartnerName, ui.lePartnerName );
			ui.lePartnerName->setValidator( partnerNameValidator );
		}

		void PartnerPickerVC::reject()
		{
			clear();
			QDialog::reject();
		}

	} // end namespace viewConn
} // end namespace tender
