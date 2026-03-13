//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tendersearchcriteriafrance.h"
#include "loggerpool/tenderloggerpool.h"
#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/tender/itender.h"
#include <libtender/domainmodule/tender/tendertypecollection.h>
#include <libtender/domainmodule/tender/tenderstatecollection.h>

#include <QMessageBox> 

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
        TenderSearchCriteriaFrance::TenderSearchCriteriaFrance()
        : m_EventsWiring  ( "TenderSearchCriteriaFrance::EventsWiring"   )
        {
            //METHODNAME_DEF( TenderSearcherVC, TenderSearcherVC )
            //BLOG_TRACE_METHOD( m_Logger, fun );

            ui.setupUi(this);
            wireEvents();

            setupSearchCriteria();
            resetAllInputFields();
		}

        /////////////////////////////////////////////////////////////////////////////////////////////
        /*!    \throw  no-throw */
        /////////////////////////////////////////////////////////////////////////////////////////////
        TenderSearchCriteriaFrance::~TenderSearchCriteriaFrance()
        {
            //METHODNAME_DEF( TenderSearcherVC, ~TenderSearcherVC )
            //BLOG_TRACE_METHOD( m_Logger, fun );

            //TenderSearcherVC::close();
        }

        void TenderSearchCriteriaFrance::wireEvents()
        {
            if( m_EventsWiring.isSet() )
            {
                return;
            }

			connect( ui.btnArticleCode,      SIGNAL( clicked()					   ),
			this,                            SLOT  ( onArticleClicked()			   ));
											  							           
			connect( ui.btnCustomerNo,       SIGNAL( clicked()					   ),
			this,                            SLOT  ( onCustomerClicked()		   ));
											  							           
			connect( ui.btnPartnerNo,        SIGNAL( clicked()					   ),
            this,                            SLOT  ( onPartnerClicked()			   ));
											  							           
			connect( ui.btnSearch,           SIGNAL( clicked()					   ),
            this,                            SLOT  ( onSearchClicked()			   ));
										     
			connect( ui.gbContractPeriod,    SIGNAL( clicked(bool)                 ),
            this,                            SLOT  ( onContractPeriodClicked(bool) ));

			connect( ui.dEContractBeginDate, SIGNAL( editingFinished()			   ),
            this,                            SLOT  ( onContractDateChanged()       ));
					
			connect( ui.dEContractEndDate,   SIGNAL( editingFinished()			   ),
            this,                            SLOT  ( onContractDateChanged()	   ));

			QAction* actionPicker = new QAction( this );
		    actionPicker->setShortcut( Qt::Key_F2 );

			connect( actionPicker,     SIGNAL( triggered()           ),
			this,                      SLOT  ( onShortcutTriggered() ));
			
			this->addAction( actionPicker );


            m_EventsWiring.set();
        }

        void TenderSearchCriteriaFrance::onSearchClicked()
        {
            METHODNAME_DEF( TenderSearchCriteriaFrance, onSearchClicked )

			if( ui.dEContractBeginDate->date() >= ui.dEContractEndDate->date() &&
				ui.gbContractPeriod->isChecked() )
			{
				QMessageBox msgBox;
				msgBox.setText("Contract begin date is greater than or equal to contract end date.");
				msgBox.setIcon( QMessageBox::Warning );
				msgBox.exec();
				return;
			}

            libutil::gui::WaitCursor bc( this );
            basar::appl::SystemEventManager::getInstance().fire( "SearchTenderRequested" );
        }

		void TenderSearchCriteriaFrance::onArticleClicked()
		{
			METHODNAME_DEF( TenderSearchCriteriaFrance, onArticleNoClicked )

			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "SearchArticleRequested" );
		}

		void TenderSearchCriteriaFrance::onCustomerClicked()
		{
			METHODNAME_DEF( TenderSearchCriteriaFrance, onCustomerClicked )

			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "SearchCustomerRequested" );
		}

		void TenderSearchCriteriaFrance::onPartnerClicked()
		{
            METHODNAME_DEF( TenderSearchCriteriaFrance, onPartnerClicked )

			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "SearchPartnerRequested" );
		}
		
        const libtender::domMod::tender::TenderTypeCollectionPtr TenderSearchCriteriaFrance::getTypeMapping()
        {
            return m_typeMapping;
        }

        const libtender::domMod::tender::TenderStateCollectionPtr TenderSearchCriteriaFrance::getStatusMapping()
        {
            return m_statusMapping;
        }

		void TenderSearchCriteriaFrance::onShortcutTriggered()
		{
			METHODNAME_DEF( TenderSearchCriteriaFrance, onShortcutTriggered )

			if( properties::PARTNER_NO.getName().c_str() == QApplication::focusWidget()->accessibleName() )
			{	
				onPartnerClicked();
				return;
			}

			if( properties::ARTICLE_CODE.getName().c_str() == QApplication::focusWidget()->accessibleName() )
			{
				onArticleClicked();
				return;
			}

			if( properties::CUSTOMER_NO.getName().c_str() == QApplication::focusWidget()->accessibleName() )
			{
				onCustomerClicked();
				return;
			}
		}

		void TenderSearchCriteriaFrance::onContractPeriodClicked( bool isChecked )
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

		void TenderSearchCriteriaFrance::onContractDateChanged()
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

        void TenderSearchCriteriaFrance::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
        {
            if(!ui.leTenderNo->text().isEmpty())
            {
                yitSearch.setInt32 (properties::TENDER_NO  , ui.leTenderNo->text().toInt());
            }

            if(!ui.leContractNo->text().isEmpty())
            {
                yitSearch.setString(properties::CONTRACT_NO, ui.leContractNo->text().toLocal8Bit().data());
            }

            if(!ui.lePartnerNo->text().isEmpty())
            {
                yitSearch.setInt32 (properties::PARTNER_NO , ui.lePartnerNo->text().toInt());
            }

			if( 0 < ui.cBTypeOfBusiness->currentIndex() )
			{
				yitSearch.setInt32( properties::TENDER_TYPE, ui.cBTypeOfBusiness->currentData().toInt() );
			}

			if( 0 < ui.cBStatus->currentIndex() )
			{
				yitSearch.setInt16( properties::TENDER_STATE, static_cast<basar::Int16>( ui.cBStatus->currentData().toInt() ) );
			}

			if(!ui.leArticleCode->text().isEmpty())
			{
				yitSearch.setString(properties::ARTICLE_CODE, ui.leArticleCode->text().toStdString()); 
			}

            if(!ui.leCustomerNo->text().isEmpty())
            {
                yitSearch.setInt32(properties::CUSTOMER_NO, ui.leCustomerNo->text().toInt());
            }

			if( ui.gbContractPeriod->isChecked()  )
			{
				yitSearch.setDate( properties::DATE_FROM, basar::cmnutil::DateTimeTools::getDate(ui.dEContractBeginDate->date().year(),
                                                                                                   ui.dEContractBeginDate->date().month(),
                                                                                                   ui.dEContractBeginDate->date().day()));

				yitSearch.setDate( properties::DATE_TO, basar::cmnutil::DateTimeTools::getDate(ui.dEContractEndDate->date().year(),
                                                                                                 ui.dEContractEndDate->date().month(),
                                                                                                 ui.dEContractEndDate->date().day()));
			}
		}

        void TenderSearchCriteriaFrance::setupSearchCriteria()
        {
            METHODNAME_DEF( TenderSearchCriteriaFrance, setupSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			QRegExp rxTenderNo("^[1-9][0-9]{1,7}$");
			QValidator *tenderNoValidator = new QRegExpValidator( rxTenderNo, ui.leTenderNo );

            ui.leTenderNo->setAccessibleName(properties::TENDER_NO.getName().c_str()  );
            ui.leTenderNo->setValidator     ( tenderNoValidator );

            ui.leContractNo->setAccessibleName(properties::CONTRACT_NO.getName().c_str());

			QRegExp rxContractNo( "^[\\s\\S]{,20}$" );
			QValidator *contractNoValidator = new QRegExpValidator( rxContractNo, ui.leContractNo );
			ui.leContractNo->setValidator( contractNoValidator );

			QRegExp rxPartnerNo("^[1-9][0-9]{1,7}$");
			QValidator *partnerNoValidator = new QRegExpValidator( rxPartnerNo, ui.lePartnerNo );

            ui.lePartnerNo->setAccessibleName(properties::PARTNER_NO.getName().c_str());
            ui.lePartnerNo->setValidator     ( partnerNoValidator );

            QString choose = tr("please choose").toLocal8Bit();

            ui.cBTypeOfBusiness->setAccessibleName(properties::TENDER_TYPE.getName().c_str());
            ui.cBTypeOfBusiness->insertItem       (0, choose);

            ui.cBStatus->setAccessibleName(properties::TENDER_STATE.getName().c_str());
            ui.cBStatus->insertItem       (0, choose);

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

        void TenderSearchCriteriaFrance::focusInEvent( QFocusEvent* event)
        {
            event;
            ui.leTenderNo->setFocus();
            ui.btnSearch->setDefault(true);
        }

        void TenderSearchCriteriaFrance::focusOutEvent( QFocusEvent* event)
        {
            event;
            ui.btnSearch->setDefault(false);
        }

        void TenderSearchCriteriaFrance::prepareForNewSearch()
        {
            resetAllInputFields();
        }

        void TenderSearchCriteriaFrance::resetAllInputFields()
        {
            ui.leTenderNo->clear();
            ui.leContractNo->clear();
            ui.lePartnerNo->clear();
            ui.cBTypeOfBusiness->setCurrentIndex(0);
            ui.cBStatus->setCurrentIndex(0);
            ui.leCustomerNo->clear();
			ui.leArticleCode->clear();

			ui.gbContractPeriod->setChecked( false );

			ui.dEContractBeginDate->setDisabled( true );
			ui.dEContractBeginDate->setDate( QDate( QDate::currentDate().year(), 1, 1 ) );
			ui.dEContractEndDate->setDisabled( true );
			ui.dEContractEndDate->setDate( QDate( QDate::currentDate().year(), 12, 31 ) );
        }

        void TenderSearchCriteriaFrance::injectStatusMapping( libtender::domMod::tender::TenderStateCollectionPtr statusMapping )
        {
            m_statusMapping = statusMapping;

			using namespace libtender::domMod::tender;
			TenderStateCollection::const_iterator iter = m_statusMapping->begin();
            for( ; iter != m_statusMapping->end(); ++iter )
            {
                ui.cBStatus->addItem( QString::fromLocal8Bit( iter->second.c_str() ), QVariant( iter->first ) );
            }
        }

        void TenderSearchCriteriaFrance::injectTypeMapping( libtender::domMod::tender::TenderTypeCollectionPtr typeMapping )
        {
            m_typeMapping = typeMapping;

			using namespace libtender::domMod::tender;
			TenderTypeCollection::const_iterator iter = m_typeMapping->begin();
            for( ; iter != m_typeMapping->end(); ++iter )
            {
                ui.cBTypeOfBusiness->addItem( QString::fromLocal8Bit( iter->second.c_str() ), QVariant( iter->first ) );
            }
        }

		void TenderSearchCriteriaFrance::setArticleCode( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( TenderSearchCriteriaFrance, setArticleCode )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( yit.isContainedAndSet( properties::ARTICLE_CODE ) )
			{
				ui.leArticleCode->setText( QString::fromLocal8Bit( yit.getString( properties::ARTICLE_CODE ).c_str() ) );
			}
		}

		void TenderSearchCriteriaFrance::setCustomerNo( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( TenderSearchCriteriaFrance, setCustomerNo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( yit.isContainedAndSet( properties::CUSTOMER_NO ) )
			{
				ui.leCustomerNo->setText( QString::number( yit.getInt32( properties::CUSTOMER_NO ) ));
			}
		}

		void TenderSearchCriteriaFrance::setPartnerNo( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
            METHODNAME_DEF( TenderSearchCriteriaFrance, setPartnerNo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( yit.isContainedAndSet( properties::PARTNER_NO ) )
			{
				ui.lePartnerNo->setText( QString::number( yit.getInt32( properties::PARTNER_NO ) ));
			}
		}

    } // end namespace viewConn
} // end namespace tender
