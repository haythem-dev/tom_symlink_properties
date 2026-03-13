//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tendersearchcriteriabulgaria.h"
#include "loggerpool/tenderloggerpool.h"
#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/tender/itender.h"

#include <libtender/domainmodule/tender/tenderstatecollection.h>
#include <libtender/domainmodule/tender/tendertypecollection.h>

#include <libutil/viewconn.h>

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
        TenderSearchCriteriaBulgaria::TenderSearchCriteriaBulgaria()
        : m_EventsWiring  ( "TenderSearchCriteriaBulgaria::EventsWiring"   )
        {
            //METHODNAME_DEF( TenderSearcherVC, TenderSearcherVC )
            //BLOG_TRACE_METHOD( m_Logger, fun );

            ui.setupUi( this );
            wireEvents();

			setupSearchCriteria();
			resetAllInputFields();
        }

        /////////////////////////////////////////////////////////////////////////////////////////////
        /*!    \throw  no-throw */
        /////////////////////////////////////////////////////////////////////////////////////////////
        TenderSearchCriteriaBulgaria::~TenderSearchCriteriaBulgaria()
        {
            //METHODNAME_DEF( TenderSearcherVC, ~TenderSearcherVC )
            //BLOG_TRACE_METHOD( m_Logger, fun );

            //TenderSearcherVC::close();
        }

        void TenderSearchCriteriaBulgaria::wireEvents()
        {
            if( m_EventsWiring.isSet() )
            {
                return;
            }

			connect( ui.btnArticleCode,      SIGNAL( clicked()						),
			this,                            SLOT  ( onArticlePickerClicked()		));
											  										
			connect( ui.btnCustomerNo,       SIGNAL( clicked()						),
			this,                            SLOT  ( onCustomerPickerClicked()		));

			connect( ui.btnSearch,			 SIGNAL( clicked()						),
            this,							 SLOT  ( onSearchClicked()				));
										   
			connect( ui.gbContractPeriod,    SIGNAL( clicked(bool)					),
            this,                            SLOT  ( onContractPeriodClicked(bool)	));

			connect( ui.dEContractBeginDate, SIGNAL( editingFinished()				),
            this,                            SLOT  ( onContractDateChanged()		));
					
			connect( ui.dEContractEndDate,   SIGNAL( editingFinished()				),
            this,                            SLOT  ( onContractDateChanged()		));


			QAction* actionPicker = new QAction( this );
		    actionPicker->setShortcut( Qt::Key_F2 );

			connect( actionPicker,			 SIGNAL( triggered()           ),
			this,							 SLOT  ( onShortcutTriggered() ));
			
			this->addAction( actionPicker );

            m_EventsWiring.set();
        }

		void TenderSearchCriteriaBulgaria::onSearchClicked()
		{
            METHODNAME_DEF( TenderSearchCriteriaBulgaria, onSearchClicked )

			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "SearchTenderRequested" );
		}

		void TenderSearchCriteriaBulgaria::onArticlePickerClicked()
		{
			METHODNAME_DEF( TenderSearchCriteriaBulgaria, onArticlePickerClicked )

			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "SearchArticleRequested" );
		}

		void TenderSearchCriteriaBulgaria::onCustomerPickerClicked()
		{
			METHODNAME_DEF( TenderSearchCriteriaBulgaria, onCustomerPickerClicked )

			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "SearchCustomerRequested" );
		}

		void TenderSearchCriteriaBulgaria::onShortcutTriggered()
		{
			METHODNAME_DEF( TenderSearchCriteriaBulgaria, onShortcutTriggered )

			if( properties::ARTICLE_CODE.getName().c_str() == QApplication::focusWidget()->accessibleName() )
			{
				onArticlePickerClicked();
				return;
			}

			if( properties::CUSTOMER_NO.getName().c_str() == QApplication::focusWidget()->accessibleName() )
			{
				onCustomerPickerClicked();
				return;
			}
		}

		void TenderSearchCriteriaBulgaria::onContractDateChanged()
		{
			if( 1999 >= ui.dEContractBeginDate->date().year() )
			{
				QMessageBox msgBox;
				msgBox.setText( "Contract begin date is invalid." );
				msgBox.setIcon( QMessageBox::Warning );
				msgBox.exec();	

				ui.dEContractBeginDate->setDate( QDate( QDate::currentDate().year(), 1, 1 ) );
			}

			if( 1999 >= ui.dEContractEndDate->date().year() )
			{
				QMessageBox msgBox;
				msgBox.setText( "Contract end date is invalid." );
				msgBox.setIcon( QMessageBox::Warning );
				msgBox.exec();

				ui.dEContractEndDate->setDate( QDate( QDate::currentDate().year(), 12, 31 ) );
			}
		}

		void TenderSearchCriteriaBulgaria::onContractPeriodClicked( bool isChecked )
		{
			if( isChecked )
			{
				ui.dEContractBeginDate->setEnabled( true );
				ui.dEContractEndDate->setEnabled  ( true );
			}
			else
			{
				ui.dEContractBeginDate->setEnabled( false );
				ui.dEContractEndDate->setEnabled  ( false );
			}
		}

		void TenderSearchCriteriaBulgaria::focusInEvent( QFocusEvent* event )
		{
			event;
            ui.leTenderNo->setFocus();
            ui.btnSearch->setDefault(true);
		}

        void TenderSearchCriteriaBulgaria::focusOutEvent( QFocusEvent* event )
		{
			event;
            ui.btnSearch->setDefault(false);
		}

		void TenderSearchCriteriaBulgaria::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
        {
			if( !ui.leTenderNo->text().isEmpty() )
            {
                yitSearch.setInt32( properties::TENDER_NO  , ui.leTenderNo->text().toInt() );
            }

			if( !ui.leContractNo->text().isEmpty() )
            {
                yitSearch.setString( properties::CONTRACT_NO, ui.leContractNo->text().toLocal8Bit().data() );
            }

			if( !ui.leCustomerNo->text().isEmpty() )
            {
                yitSearch.setInt32( properties::CUSTOMER_NO, ui.leCustomerNo->text().toInt() );
            }

			if( 0 < ui.cbStatus->currentIndex() )
			{
				yitSearch.setInt16( properties::TENDER_STATE, static_cast<basar::Int16>( ui.cbStatus->currentData().toInt() ) );
			}

			if( 0 < ui.cbType->currentIndex() )
			{
				yitSearch.setInt32( properties::TENDER_TYPE, ui.cbType->currentData().toInt() );
			}

			if( !ui.leArticleCode->text().isEmpty() )
			{
				yitSearch.setString( properties::ARTICLE_CODE, ui.leArticleCode->text().toLocal8Bit().data() );
			}

			if( ui.gbContractPeriod->isChecked()  )
			{
				yitSearch.setDate( properties::DATE_FROM, basar::cmnutil::DateTimeTools::getDate( ui.dEContractBeginDate->date().year(),
                                                                                                  ui.dEContractBeginDate->date().month(),
                                                                                                  ui.dEContractBeginDate->date().day() ) );

				yitSearch.setDate( properties::DATE_TO, basar::cmnutil::DateTimeTools::getDate( ui.dEContractEndDate->date().year(),
                                                                                                ui.dEContractEndDate->date().month(),
                                                                                                ui.dEContractEndDate->date().day() ) );
			}
        }

		void TenderSearchCriteriaBulgaria::setupSearchCriteria()
		{
			METHODNAME_DEF( TenderSearchCriteriaBulgaria, setupSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			QRegExp rxTenderNo("^[1-9][0-9]{1,7}$");
			QValidator *tenderNoValidator = new QRegExpValidator( rxTenderNo, ui.leTenderNo );

            ui.leTenderNo->setAccessibleName(properties::TENDER_NO.getName().c_str()  );
            ui.leTenderNo->setValidator     ( tenderNoValidator );

            ui.leContractNo->setAccessibleName(properties::CONTRACT_NO.getName().c_str());

			QRegExp rxContractNo( "^[\\s\\S]{,20}$" );
			QValidator *contractNoValidator = new QRegExpValidator( rxContractNo, ui.leContractNo );
			ui.leContractNo->setValidator( contractNoValidator );

			QString choose = tr("please choose");

            ui.cbType->setAccessibleName(properties::TENDER_TYPE.getName().c_str());
            ui.cbType->insertItem       (0, choose);

            ui.cbStatus->setAccessibleName(properties::TENDER_STATE.getName().c_str());
            ui.cbStatus->insertItem       (0, choose);

			QRegExp rx("^[a-zA-Z0-9]{1,13}$");
			QValidator *articleValidator = new QRegExpValidator(rx, ui.leArticleCode);

			ui.leArticleCode->setAccessibleName( properties::ARTICLE_CODE.getName().c_str());
			ui.leArticleCode->setValidator( articleValidator );

			QRegExp rxCustomerNo("^[1-9][0-9]{1,7}$");
			QValidator *CustomerNoValidator = new QRegExpValidator( rxCustomerNo, ui.leCustomerNo );

            ui.leCustomerNo->setAccessibleName( properties::CUSTOMER_NO.getName().c_str());
            ui.leCustomerNo->setValidator     ( CustomerNoValidator );

            ui.dEContractBeginDate->setAccessibleName( properties::DATE_FROM.getName().c_str());
            ui.dEContractBeginDate->setDisplayFormat ( "dd.MM.yyyy");

            ui.dEContractEndDate->setAccessibleName( properties::DATE_TO.getName().c_str());
            ui.dEContractEndDate->setDisplayFormat ( "dd.MM.yyyy");

			ui.dEContractBeginDate->setDate( QDate( QDate::currentDate().year(),  1, 1  ));
			ui.dEContractEndDate->setDate  ( QDate( QDate::currentDate().year(), 12, 31 ));
		}

		void TenderSearchCriteriaBulgaria::prepareForNewSearch()
		{
			resetAllInputFields();
		}

		void TenderSearchCriteriaBulgaria::resetAllInputFields()
		{
			ui.leTenderNo->clear();
            ui.leContractNo->clear();
			ui.leCustomerNo->clear();
            ui.cbType->setCurrentIndex(0);
            ui.cbStatus->setCurrentIndex(0);
			ui.leArticleCode->clear();

			ui.gbContractPeriod->setChecked( false );

			ui.dEContractBeginDate->setDisabled( true );
			ui.dEContractBeginDate->setDate( QDate( QDate::currentDate().year(), 1, 1 ) );
			ui.dEContractEndDate->setDisabled( true );
			ui.dEContractEndDate->setDate( QDate( QDate::currentDate().year(), 12, 31 ) );
		}

		void TenderSearchCriteriaBulgaria::injectStatusMapping( libtender::domMod::tender::TenderStateCollectionPtr statusMapping )
		{
		    m_statusMapping = statusMapping;

			using namespace libtender::domMod::tender;
			TenderStateCollection::const_iterator iter = m_statusMapping->begin();
            for( ; iter != m_statusMapping->end(); ++iter )
            {
                ui.cbStatus->addItem( QString::fromLocal8Bit( iter->second.c_str() ), QVariant( iter->first ) );
            }
		}

		void TenderSearchCriteriaBulgaria::injectTypeMapping( libtender::domMod::tender::TenderTypeCollectionPtr typeMapping )
		{
			m_typeMapping = typeMapping;

			using namespace libtender::domMod::tender;
			TenderTypeCollection::const_iterator iter = m_typeMapping->begin();
            for( ; iter != m_typeMapping->end(); ++iter )
            {
                ui.cbType->addItem( QString::fromLocal8Bit( iter->second.c_str() ), QVariant( iter->first ) );
            }
		}

		const libtender::domMod::tender::TenderTypeCollectionPtr TenderSearchCriteriaBulgaria::getTypeMapping()
		{
			return m_typeMapping;
		}

        const libtender::domMod::tender::TenderStateCollectionPtr TenderSearchCriteriaBulgaria::getStatusMapping()
        {
            return m_statusMapping;
        }

		void TenderSearchCriteriaBulgaria::setArticleCode( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( TenderSearchCriteriaBulgaria, setArticleCode )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( yit.isContainedAndSet( properties::ARTICLE_CODE ) )
			{
				ui.leArticleCode->setText( QString::fromStdString( yit.getString(properties::ARTICLE_CODE) ));
			}
		}

		void TenderSearchCriteriaBulgaria::setCustomerNo( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( TenderSearchCriteriaBulgaria, setCustomerNo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( yit.isContainedAndSet( properties::CUSTOMER_NO ) )
			{
				ui.leCustomerNo->setText( QString::number( yit.getInt32( properties::CUSTOMER_NO ) ));
			}
		}

    } // end namespace viewConn
} // end namespace tender
