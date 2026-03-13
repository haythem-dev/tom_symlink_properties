//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tendercontractinfobulgaria.h"
#include "loggerpool/tenderloggerpool.h"

#include "domainmodule/libtender_properties_definitions.h"
#include "domainmodule/tender/itender.h"

#include <libtender/domainmodule/tender/tendertypecollection.h>
#include <libtender/domainmodule/tender/tenderstatecollection.h>

#include <libutil/viewconn.h>
#include "util.h"


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
        TenderContractInfoBulgaria::TenderContractInfoBulgaria()
        : m_EventsWiring  ( "TenderContractInfoBulgaria::EventsWiring"   ),
		  m_Logger( libtender::LoggerPool::getLoggerViewConn() ),
		  m_StartSearchAction( NULL )
        {
            //METHODNAME_DEF( TenderContractInfoBulgaria, TenderContractInfoBulgaria )
            //BLOG_TRACE_METHOD( m_Logger, fun );

            ui.setupUi(this);

            wireEvents();
            setupContractInfo();

            resetAllFields();

			disableAll();
		}

        /////////////////////////////////////////////////////////////////////////////////////////////
        /*!    \throw  no-throw */
        /////////////////////////////////////////////////////////////////////////////////////////////
        TenderContractInfoBulgaria::~TenderContractInfoBulgaria()
        {
            //METHODNAME_DEF( TenderContractInfoBulgaria, ~TenderContractInfoBulgaria )
            //BLOG_TRACE_METHOD( m_Logger, fun );

            //TenderSearcherVC::close();
        }

        void TenderContractInfoBulgaria::wireEvents()
        {
            if( m_EventsWiring.isSet() )
            {
                return;
            }
											  							           
			connect( ui.btnCustomerNo,			SIGNAL( clicked()		  ),
			this,								SLOT  ( onStartSearch()	  ));
			
			connect( ui.leCustomerNo,			SIGNAL( returnPressed() ), 
			this,								SLOT  ( onCheckCustomer() ));
			
			connect( ui.leCustomerNo,			SIGNAL( editingFinished() ), 
			this,								SLOT  ( onCheckCustomer() ));
			
			connect( ui.btnCustomerList,		SIGNAL( clicked()				),
			this,								SLOT  ( onCustomerListClicked() ));

			connect( ui.gbContractPeriod,		SIGNAL( clicked(bool)                 ),
            this,								SLOT  ( onContractPeriodClicked(bool) ));

			connect( ui.deContractBeginDate,	SIGNAL( editingFinished()			   ),
            this,								SLOT  ( onContractDateChanged()       ));
					
			connect( ui.deContractEndDate,		SIGNAL( editingFinished()			   ),
            this,								SLOT  ( onContractDateChanged()	   ));

			connect( ui.cbStatus,				SIGNAL( currentIndexChanged(int) 	   ),
			this,								SLOT  ( onStateChanged(int)		   ));

			connect( ui.cbType,					SIGNAL( currentIndexChanged(int)   ),
			this,								SLOT  ( onTypeChanged(int)		   ));


			connect( ui.leContractNo,			SIGNAL( textChanged(const QString &) ),
			this,								SLOT  ( onContractInfoChanged() ) );

			connect( ui.leCustomerNo,			SIGNAL( textChanged(const QString & ) ),
			this,								SLOT  ( onContractInfoChanged() ) );

			connect( ui.leCustomerBranchNo,		SIGNAL( textChanged(const QString & ) ),
			this,								SLOT  ( onContractInfoChanged() ) );

			connect( ui.btnProtocols,			SIGNAL( clicked() ),
			this,								SLOT  ( onShowProtocols() ) );
	
            m_EventsWiring.set();
        }

		void TenderContractInfoBulgaria::onStartSearch()
		{
			METHODNAME_DEF( TenderContractInfoBulgaria, onStartSearch )
			BLOG_TRACE_METHOD( m_Logger, fun );

			if( isCustomerInFocus() )
			{
				libutil::gui::WaitCursor bc( this );
				basar::appl::SystemEventManager::getInstance().fire( "SearchCustomerRequested" );
			}
			//else if( 
			//{
			//	
			//}
		}

		void TenderContractInfoBulgaria::onCustomerListClicked()
		{
			METHODNAME_DEF( TenderContractInfoBulgaria, onCustomerListClicked )
			BLOG_TRACE_METHOD( m_Logger, fun );

			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "CustomerListRequested" );
		}

		void TenderContractInfoBulgaria::onInvoiceClicked()
		{
			METHODNAME_DEF( TenderContractInfoBulgaria, onInvoiceClicked )
			BLOG_TRACE_METHOD( m_Logger, fun );

			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "InvoiceRequested" );
		}

		void TenderContractInfoBulgaria::onCheckCustomer()
		{
			METHODNAME_DEF( TenderContractInfoBulgaria, OnCheckCustomer )
			BLOG_TRACE_METHOD( m_Logger, fun );

			basar::appl::SystemEventManager::getInstance().fire( "CheckCustomerRequested" );
		}

		const libtender::domMod::tender::TenderTypeCollectionPtr TenderContractInfoBulgaria::getTypeMapping()
		{
			return m_TenderTypeCollection;
		}

		const libtender::domMod::tender::TenderStateEnum TenderContractInfoBulgaria::getStateIndex()
		{
			switch( ui.cbStatus->currentIndex() )
			{
			case 0: return libtender::domMod::tender::TS_UNCOMPLETED;
				break;
			case 1: return libtender::domMod::tender::TS_OK;
				break;
			case 8: return libtender::domMod::tender::TS_MANUALLY_CLOSED;
				break;
			case 9: 
			default: return libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED;
				break;
			}
		}

		const libtender::domMod::tender::TenderStateEnum TenderContractInfoBulgaria::getStateData()
		{
			switch( ui.cbStatus->currentData().toInt() )
			{
			case 0: return libtender::domMod::tender::TS_UNCOMPLETED;
				break;
			case 1: return libtender::domMod::tender::TS_OK;
				break;
			case 8: return libtender::domMod::tender::TS_MANUALLY_CLOSED;
				break;
			case 9: 
			default: return libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED;
				break;
			}
		}

		const libtender::domMod::tender::TenderTypeEnum TenderContractInfoBulgaria::getType()
		{
			switch( ui.cbType->currentData().toInt() )
			{
			case 3: return libtender::domMod::tender::TT_PREWHOLESALE;
				break;
			case 4: return libtender::domMod::tender::TT_PLATFORM;
				break;
			case 5: return libtender::domMod::tender::TT_SRA;
				break;
			case 6:
			default: return libtender::domMod::tender::TT_CAP;
				break;
			}
		}
		
		const basar::Date TenderContractInfoBulgaria::getDateTo()
		{
			return basar::cmnutil::DateTimeTools::getDate( ui.deContractEndDate->date().year(), ui.deContractEndDate->date().month(), ui.deContractEndDate->date().day() );
		}

		bool TenderContractInfoBulgaria::isFilled()
		{
			METHODNAME_DEF( TenderContractInfoBulgaria, isFilled )
			BLOG_TRACE_METHOD( m_Logger, fun );

			if( ui.leContractNo->text().isEmpty() ) return false;
			if( ui.leCustomerNo->text().isEmpty() ) return false; 
			if( ui.leCustomerBranchNo->text().isEmpty() ) return false;

			return true;
		}

		bool TenderContractInfoBulgaria::isCustomerSelected()
		{
			METHODNAME_DEF( TenderContractInfoBulgaria, isCustomerSelected )
			BLOG_TRACE_METHOD( m_Logger, fun );

			if(ui.leCustomerNo->text().isEmpty() || ui.leCustomerBranchNo->text().isEmpty())
			{
				return false;
			}
		
			return true;
		}

		bool TenderContractInfoBulgaria::isCustomerInFocus()
		{
			if( ui.leCustomerNo->hasFocus() || 
				ui.btnCustomerNo->hasFocus()  )
			{
				return true;
			}

			return false;
		}

		bool TenderContractInfoBulgaria::isContractPeriodValid()
		{
			if( ui.deContractBeginDate->date() >= ui.deContractEndDate->date() )
			{
				return false;
			}

			return true;
		}

		void TenderContractInfoBulgaria::onContractPeriodClicked( bool isChecked )
		{
			if( isChecked )
			{
				ui.deContractBeginDate->setEnabled( true );
				ui.deContractEndDate->setEnabled  ( true );
			}
			else
			{
				ui.deContractBeginDate->setEnabled( false );
				ui.deContractEndDate->setEnabled  ( false );
			}
		}

		void TenderContractInfoBulgaria::onContractDateChanged()
		{
			if( 1999 >= ui.deContractBeginDate->date().year() )
			{
				QMessageBox msgBox;
				msgBox.setText( "Contract begin date is invalid." );
				msgBox.setIcon( QMessageBox::Warning );
				msgBox.exec();	

				ui.deContractBeginDate->setDate( QDate( QDate::currentDate().year(), 1, 1 ) );
			}

			if( 1999 >= ui.deContractEndDate->date().year() )
			{
				QMessageBox msgBox;
				msgBox.setText( "Contract end date is invalid." );
				msgBox.setIcon( QMessageBox::Warning );
				msgBox.exec();

				ui.deContractEndDate->setDate( QDate( QDate::currentDate().year(), 12, 31 ) );
			}
		}

		void TenderContractInfoBulgaria::onStateChanged( int index )
		{
			METHODNAME_DEF( TenderContractInfoBulgaria, onStateChanged )
			BLOG_TRACE_METHOD( m_Logger, fun );

			if( libtender::domMod::tender::TS_OK  < index )
			{
				disableAllExceptState( true, true );
				basar::appl::SystemEventManager::getInstance().fire( "ChangeStateToClosedRequested" );
			}
			else
			{
				disableAllExceptState( false, true );
				basar::appl::SystemEventManager::getInstance().fire( "ChangeStateFromClosedRequested" );
			}
		}

		void TenderContractInfoBulgaria::onTypeChanged( int index ) 
		{
			METHODNAME_DEF( TenderContractInfoBulgaria, onTypeChanged )
			BLOG_TRACE_METHOD( m_Logger, fun );

			++index;

			if( libtender::domMod::tender::TT_HOSPITAL == index )
			{
				ui.btnCustomerList->setVisible( false );
				ui.btnProtocols->setVisible( false );
			}
			else
			{
				ui.btnCustomerList->setVisible( true );
				ui.btnProtocols->setVisible( true );
			}
		}

		void TenderContractInfoBulgaria::onContractInfoChanged()
		{
			METHODNAME_DEF( TenderContractInfoBulgaria, onTextChanged )
			BLOG_TRACE_METHOD( m_Logger, fun );

			basar::appl::SystemEventManager::getInstance().fire( "ContractInfoChanged" );
		}

		void TenderContractInfoBulgaria::onShowProtocols()
		{
			METHODNAME_DEF( TenderContractInfoBulgaria, onShowProtocols )
			BLOG_TRACE_METHOD( m_Logger, fun );

			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "ProtocolsRequested" );
		}

        void TenderContractInfoBulgaria::matchToTender( basar::db::aspect::AccessorPropertyTable_YIterator yitTender )
        {
            if(!ui.leTenderNo->text().isEmpty())
            {
                yitTender.setInt32 (properties::TENDER_NO  , ui.leTenderNo->text().toInt());
            }

            if(!ui.leContractNo->text().isEmpty())
            {
                yitTender.setString(properties::CONTRACT_NO, ui.leContractNo->text().toLocal8Bit().data());
            }

			if( ui.cbPurchaseOrderProposal->isChecked() )
			{
				yitTender.setInt16( properties::PURCHASE_ORDER_PROPOSAL_FLAG, 1 );
			}
			else
			{
				yitTender.setInt16( properties::PURCHASE_ORDER_PROPOSAL_FLAG, 0 );
			}

			if( -1 < ui.cbType->currentIndex() )
			{
				yitTender.setInt32( properties::TENDER_TYPE, ui.cbType->currentData().toInt() );
			}

			if( -1 < ui.cbStatus->currentIndex() )
			{
				yitTender.setInt16( properties::TENDER_STATE, static_cast<basar::Int16>( ui.cbStatus->currentData().toInt() ) );
			}

            if(!ui.leCustomerNo->text().isEmpty())
            {
				yitTender.setInt32( properties::ACC_CUSTOMER_NO, ui.leCustomerNo->text().toInt() );
				yitTender.setInt16( properties::ACC_BRANCH_NO, ui.leCustomerBranchNo->text().toShort() );
				
				if( 0 == ui.cbType->currentIndex() )
				{
					yitTender.setInt32( properties::CUSTOMER_NO, ui.leCustomerNo->text().toInt() );
					yitTender.setInt16( properties::BRANCH_NO, ui.leCustomerBranchNo->text().toShort() );
				}
				else
				{
					yitTender.setInt32( properties::CUSTOMER_NO, 0 );
					yitTender.setInt16( properties::BRANCH_NO, 0 );
				}

				yitTender.setInt32( properties::CUSTOMER_NO, ui.leCustomerNo->text().toInt() );
				yitTender.setInt16( properties::BRANCH_NO, ui.leCustomerBranchNo->text().toShort() );
            }
			else
			{
				yitTender.setInt32( properties::CUSTOMER_NO, 0 );
				yitTender.setInt16( properties::BRANCH_NO, 0 );
			}

			yitTender.setDate( properties::DATE_FROM, basar::cmnutil::DateTimeTools::getDate(ui.deContractBeginDate->date().year(),
                                                                                                   ui.deContractBeginDate->date().month(),
                                                                                                   ui.deContractBeginDate->date().day()));

			yitTender.setDate( properties::DATE_TO, basar::cmnutil::DateTimeTools::getDate(ui.deContractEndDate->date().year(),
                                                                                                 ui.deContractEndDate->date().month(),
                                                                                                 ui.deContractEndDate->date().day()));

			yitTender.setInt32( properties::PARTNER_NO, 0 );
			yitTender.setInt32( properties::PHARMACY_CHAIN_NO, 0 );
			yitTender.setString( properties::CUSTOMERGROUPID, "00" );
            yitTender.setString( properties::PHARMACY_GROUPID, "000" );//This field only applicable for switzerland
		}

		void TenderContractInfoBulgaria::matchFromTender( basar::db::aspect::AccessorPropertyTable_YIterator yitTender, bool hasTenderArticles )
		{
			if( yitTender.isContainedAndSet( properties::TENDER_NO ) )
			{
				ui.leTenderNo->setText( QString::number( yitTender.getInt32( properties::TENDER_NO ) ) );
			}

			if( yitTender.isContainedAndSet( properties::CONTRACT_NO  ) )
			{
				ui.leContractNo->setText( QString::fromLocal8Bit( yitTender.getString( properties::CONTRACT_NO ).c_str() ) );
			}

			if( yitTender.isContainedAndSet( properties::PURCHASE_ORDER_PROPOSAL_FLAG  ) )
			{
				if( 0 == yitTender.getInt16( properties::PURCHASE_ORDER_PROPOSAL_FLAG ) )
				{
					ui.cbPurchaseOrderProposal->setChecked( false );
				}
				else
				{
					ui.cbPurchaseOrderProposal->setChecked( true );
				}
			}

			if( yitTender.isContainedAndSet( properties::TENDER_STATE  ) )
			{
				ComboBox::setCurrentIndex( ui.cbStatus, yitTender.getInt16( properties::TENDER_STATE ) );
				if(	libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED == yitTender.getInt16( properties::TENDER_STATE ) ||
					libtender::domMod::tender::TS_MANUALLY_CLOSED      == yitTender.getInt16( properties::TENDER_STATE ) )
				{
					disableAllExceptState( true, hasTenderArticles );
				}
				else
				{
					disableAllExceptState( false, hasTenderArticles );
				}
			}
			else
			{
				ui.cbStatus->setCurrentIndex( libtender::domMod::tender::TS_UNCOMPLETED );
			}

			if( yitTender.isContainedAndSet( properties::TENDER_TYPE  ) )
			{
				ComboBox::setCurrentIndex( ui.cbType, yitTender.getInt32( properties::TENDER_TYPE ) );
			}

			if( yitTender.getInt32( properties::TENDER_TYPE ) == libtender::domMod::tender::TT_HOSPITAL )
			{
				if( yitTender.isContainedAndSet( properties::CUSTOMER_NO ) )
				{
					ui.leCustomerNo->setText( QString::number( yitTender.getInt32( properties::CUSTOMER_NO ) ) );
					ui.leCustomerBranchNo->setText( QString::number( yitTender.getInt16( properties::BRANCH_NO ) ) );
					if( yitTender.isContainedAndSet( properties::PHARMACY_NAME ) )
					{
						ui.leCustomerName->setText( QString::fromLocal8Bit( yitTender.getString( properties::PHARMACY_NAME ).c_str() ) );
					}
				}
			}
			else
			{
				if( yitTender.isContainedAndSet( properties::ACC_CUSTOMER_NO ) )
				{
					ui.leCustomerNo->setText( QString::number( yitTender.getInt32( properties::ACC_CUSTOMER_NO ) ) );
					ui.leCustomerBranchNo->setText( QString::number( yitTender.getInt16( properties::ACC_BRANCH_NO ) ) );
					if( yitTender.isContainedAndSet( properties::PHARMACY_NAME ) )
					{
						ui.leCustomerName->setText( QString::fromLocal8Bit( yitTender.getString( properties::PHARMACY_NAME ).c_str() ) );
					}
				}
			}

			if( yitTender.isContainedAndSet( properties::DATE_FROM  ) )
			{
				int day = yitTender.getDate( properties::DATE_FROM ).getDay();
				int month = yitTender.getDate( properties::DATE_FROM ).getMonth();
				int year = yitTender.getDate( properties::DATE_FROM ).getYear();

				ui.deContractBeginDate->setDate( QDate::QDate(year, month, day)  );
			}
			else
			{
				ui.deContractBeginDate->setDate( QDate( QDate::currentDate().year(), 1, 1 )  );
			}

			if( yitTender.isContainedAndSet( properties::DATE_TO  ) )
			{
				int day = yitTender.getDate( properties::DATE_TO ).getDay();
				int month = yitTender.getDate( properties::DATE_TO ).getMonth();
				int year = yitTender.getDate( properties::DATE_TO ).getYear();
				
				ui.deContractEndDate->setDate( QDate::QDate(year, month, day) );
			}
			else
			{
				ui.deContractEndDate->setDate( QDate( QDate::currentDate().year(), 12, 31 ) );
			}
		}

		void TenderContractInfoBulgaria::matchFromCustomer( basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer )
		{
			METHODNAME_DEF( TenderContractInfoBulgaria, matchFromCustomer )
			BLOG_TRACE_METHOD( m_Logger, fun );

			if (yitCustomer.isNull() || yitCustomer.isEnd())
			{
				ui.leCustomerNo->clear();
			}
			else
			{
				ui.leCustomerNo->setText( QString::number( yitCustomer.getInt32( properties::CUSTOMER_NO ) ) );
				ui.leCustomerName->setText( QString::fromLocal8Bit( yitCustomer.getString( properties::PHARMACY_NAME ).c_str() ) );
				ui.leCustomerBranchNo->setText( QString::number( yitCustomer.getInt16( properties::BRANCH_NO ) ) );
			}
		}

		void TenderContractInfoBulgaria::matchToCustomerSearch( basar::db::aspect::AccessorPropertyTable_YIterator customerSearchYit )
		{
			METHODNAME_DEF( TenderContractInfoBulgaria, matchToCustomerSearch )
			BLOG_TRACE_METHOD( m_Logger, fun );

			basar::Int32 customerNo = ui.leCustomerNo->text().toInt();
			basar::Int16 branchNo = ui.leCustomerBranchNo->text().toShort();

			if( 0 < customerNo )
			{
				customerSearchYit.setInt32( properties::CUSTOMER_NO, customerNo );
			}
			if( 0 < branchNo )
			{
				customerSearchYit.setInt16( properties::BRANCH_NO, branchNo );
			}
		}

        void TenderContractInfoBulgaria::setupContractInfo()
        {
            METHODNAME_DEF( TenderContractInfoBulgaria, setupContractInfo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			QRegExp rxTenderNo("^[1-9][0-9]{1,7}$");
			QValidator *tenderNoValidator = new QRegExpValidator( rxTenderNo, ui.leTenderNo );

            ui.leTenderNo->setAccessibleName(properties::TENDER_NO.getName().c_str()  );
            ui.leTenderNo->setValidator     ( tenderNoValidator );

            ui.leContractNo->setAccessibleName(properties::CONTRACT_NO.getName().c_str());

			QRegExp rxContractNo( "^[\\s\\S]{,20}$" );
			QValidator *contractNoValidator = new QRegExpValidator( rxContractNo, ui.leContractNo );
			ui.leContractNo->setValidator( contractNoValidator );

            ui.cbType->setAccessibleName(properties::TENDER_TYPE.getName().c_str());
            ui.cbStatus->setAccessibleName(properties::TENDER_STATE.getName().c_str());

			QRegExp rxCustomerNo("^[1-9][0-9]{1,7}$");
			QValidator *CustomerNoValidator = new QRegExpValidator( rxCustomerNo, ui.leCustomerNo );
			
            ui.leCustomerNo->setAccessibleName( properties::CUSTOMER_NO.getName().c_str());
            ui.leCustomerNo->setValidator     ( CustomerNoValidator );
			
			QRegExp rxBranchNo("^[0-9]{1,2}$");
			QValidator *BranchNoValidator = new QRegExpValidator( rxBranchNo, ui.leCustomerBranchNo );
			
			ui.leCustomerBranchNo->setAccessibleName( properties::BRANCH_NO.getName().c_str() );
			ui.leCustomerBranchNo->setValidator( BranchNoValidator );

            ui.deContractBeginDate->setAccessibleName( properties::DATE_FROM.getName().c_str());
            ui.deContractBeginDate->setDisplayFormat ( "dd.MM.yyyy");

            ui.deContractEndDate->setAccessibleName( properties::DATE_TO.getName().c_str());
            ui.deContractEndDate->setDisplayFormat ( "dd.MM.yyyy");

			ui.deContractBeginDate->setDate( QDate( QDate::currentDate().year(),  1, 1  ));
			ui.deContractEndDate->setDate  ( QDate( QDate::currentDate().year(), 12, 31 ));
        }

        void TenderContractInfoBulgaria::focusInEvent( QFocusEvent* event)
        {
            event;
            ui.leTenderNo->setFocus();
        }

        void TenderContractInfoBulgaria::focusOutEvent( QFocusEvent* event)
        {
            event;
        }

        void TenderContractInfoBulgaria::resetAllFields()
        {
            ui.leTenderNo->clear();
            ui.leContractNo->clear();
           

            ui.cbType->setCurrentIndex(0);
            ui.cbStatus->setCurrentIndex(0);

			ui.leCustomerNo->clear();
			ui.leCustomerBranchNo->clear();
			ui.leCustomerName->clear();

			resetCustomer();
			
			ui.deContractBeginDate->setDate( QDate( QDate::currentDate().year(), 1, 1 ) );
			ui.deContractEndDate->setDate( QDate( QDate::currentDate().year(), 12, 31 ) );

			ui.cbStatus->setItemData(3, tr("Closed auto"), Qt::UserRole-1);
        }

		void TenderContractInfoBulgaria::resetCustomer()
		{
			ui.leCustomerName->clear();
			ui.leCustomerBranchNo->clear();
		}

		void TenderContractInfoBulgaria::setFocusOnFirstEmptyField()
		{
			if( ui.leContractNo->text().isEmpty() )
			{
				ui.leContractNo->setFocus();
				return;
			}

			if( ui.leCustomerNo->text().isEmpty() )
			{
				ui.leCustomerNo->setFocus();
			}
		}

        void TenderContractInfoBulgaria::injectStatusMapping( libtender::domMod::tender::TenderStateCollectionPtr statusMapping )
        {
            m_TenderStateCollection = statusMapping;

			using namespace libtender::domMod::tender;
			TenderStateCollection::const_iterator iter = m_TenderStateCollection->begin();
            for( ; iter != m_TenderStateCollection->end(); ++iter )
            {
				std::string test = iter->second.c_str();
				libtender::domMod::tender::TenderStateEnum enuu = iter->first;

                ui.cbStatus->addItem( QString::fromLocal8Bit( iter->second.c_str() ), QVariant( enuu ) );
            }
        }

        void TenderContractInfoBulgaria::injectTypeMapping( libtender::domMod::tender::TenderTypeCollectionPtr typeMapping )
        {
            m_TenderTypeCollection = typeMapping;

			using namespace libtender::domMod::tender;
			TenderTypeCollection::const_iterator iter = m_TenderTypeCollection->begin();
            for( ; iter != m_TenderTypeCollection->end(); ++iter )
            {
                ui.cbType->addItem( QString::fromLocal8Bit( iter->second.c_str() ), QVariant( iter->first ) );
            }
        }

		void TenderContractInfoBulgaria::setCustomerNo( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( TenderContractInfoBulgaria, setCustomerNo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( yit.isContainedAndSet( properties::CUSTOMER_NO ) )
			{
				ui.leCustomerNo->setText( QString::number( yit.getInt32( properties::CUSTOMER_NO ) ));
			}
		}

		void TenderContractInfoBulgaria::disableAll()
		{
			ui.leContractNo->setDisabled(true);
			ui.cbPurchaseOrderProposal->setDisabled(true);
			ui.cbStatus->setDisabled(true);
			ui.cbType->setDisabled(true);

            ui.leCustomerNo->setDisabled(true);
            ui.btnCustomerNo->setDisabled(true);
            ui.leCustomerBranchNo->setDisabled(true);
            ui.btnCustomerList->setDisabled(true);

            ui.btnProtocols->setDisabled(true);
			ui.gbContractPeriod->setDisabled(true);
		}

		void TenderContractInfoBulgaria::disableAllExceptState(bool disable, bool hasTenderArticles)
		{
			if(disable)
			{
				ui.leContractNo->setDisabled( true );
				ui.cbPurchaseOrderProposal->setDisabled( true );
				ui.cbType->setDisabled( true );
				ui.leCustomerNo->setDisabled( true );
				ui.leCustomerBranchNo->setDisabled( true );
				ui.btnCustomerNo->setDisabled( true );
				ui.btnCustomerList->setDisabled( true );
				ui.btnProtocols->setDisabled( true );
				ui.gbContractPeriod->setDisabled( true );
			}
			else
			{
				ui.leContractNo->setEnabled( true );
				ui.cbPurchaseOrderProposal->setEnabled( true );
				ui.cbStatus->setEnabled( true );

				if( !hasTenderArticles )
				{
					ui.cbType->setEnabled( true );
				}
				else
				{
					ui.cbType->setDisabled( true );
				}
		
				ui.leCustomerNo->setEnabled( true );
				ui.leCustomerBranchNo->setEnabled( true );
				ui.btnCustomerNo->setEnabled( true );
				ui.btnCustomerList->setEnabled( true );
				ui.btnProtocols->setEnabled( true );
				ui.gbContractPeriod->setEnabled( true );
			}
		}

    } // end namespace viewConn
} // end namespace tender
