//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tendercontractinfoserbia.h"
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
        TenderContractInfoSerbia::TenderContractInfoSerbia()
        : m_EventsWiring  ( "TenderContractInfoSerbia::EventsWiring"   ),
		  m_Logger( libtender::LoggerPool::getLoggerViewConn() ),
		  m_StartSearchAction( NULL )
        {
            //METHODNAME_DEF( TenderContractInfoSerbia, TenderContractInfoSerbia )
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
        TenderContractInfoSerbia::~TenderContractInfoSerbia()
        {
            //METHODNAME_DEF( TenderContractInfoSerbia, ~TenderContractInfoSerbia )
            //BLOG_TRACE_METHOD( m_Logger, fun );

            //TenderSearcherVC::close();
        }

        void TenderContractInfoSerbia::wireEvents()
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


			connect( ui.btnPharmacyChainNo,		SIGNAL( clicked()			 ),
            this,								SLOT  ( onStartSearch()		 ));

			connect( ui.lePharmacyChainNo,		SIGNAL( returnPressed() ),
			this,								SLOT  ( onCheckPartner() ) );

			connect( ui.lePharmacyChainNo,		SIGNAL( editingFinished() ),
			this,								SLOT  ( onCheckPartner() ) );

			connect( ui.gbContractPeriod,		SIGNAL( clicked(bool)                 ),
            this,								SLOT  ( onContractPeriodClicked(bool) ));

			connect( ui.deContractBeginDate,	SIGNAL( editingFinished()			   ),
            this,								SLOT  ( onContractDateChanged()       ));

			connect( ui.deContractEndDate,		SIGNAL( editingFinished()			   ),
            this,								SLOT  ( onContractDateChanged()	   ));

			connect( ui.cbStatus,				SIGNAL( currentIndexChanged(int) 	   ),
			this,								SLOT  ( onStateChanged(int)		   ));

			connect( ui.leContractNo,			SIGNAL( textChanged(const QString &) ),
			this,								SLOT  ( onContractInfoChanged() ) );

			connect( ui.lePharmacyChainNo,		SIGNAL( textChanged(const QString &)  ),
			this,								SLOT  ( onContractInfoChanged() ) );

			connect( ui.leCustomerGroupID,		SIGNAL( textChanged(const QString &)  ),
			this,								SLOT  ( onContractInfoChanged() ) );

			connect( ui.leCustomerNo,			SIGNAL( textChanged(const QString & ) ),
			this,								SLOT  ( onContractInfoChanged() ) );

			connect( ui.leBranchno,				SIGNAL( textChanged(const QString & ) ),
			this,								SLOT  ( onContractInfoChanged() ) );

			connect( ui.rbCustomer,				SIGNAL( toggled(bool) ),
			this,								SLOT  ( onCustomerChecked(bool) ));

			connect( ui.rbPharmacyChain,		SIGNAL( toggled(bool) ),
			this,								SLOT  ( onPharmacyChainChecked(bool) ));

            m_EventsWiring.set();
        }

		void TenderContractInfoSerbia::onStartSearch()
		{
			METHODNAME_DEF( TenderContractInfoSerbia, onStartSearch )
			BLOG_TRACE_METHOD( m_Logger, fun );

			if( isCustomerInFocus() )
			{
				libutil::gui::WaitCursor bc( this );
				basar::appl::SystemEventManager::getInstance().fire( "SearchCustomerRequested" );
			}
			else if( isPharmacyChainInFocus()  )
			{
				libutil::gui::WaitCursor bc( this );
				basar::appl::SystemEventManager::getInstance().fire( "SearchPharmacyChainRequested" );
			}
		}

		void TenderContractInfoSerbia::onCheckCustomer()
		{
			METHODNAME_DEF( TenderContractInfoSerbia, OnCheckCustomer )
			BLOG_TRACE_METHOD( m_Logger, fun );

			basar::appl::SystemEventManager::getInstance().fire( "CheckCustomerRequested" );
		}

		void TenderContractInfoSerbia::onCheckPharmacyChain()
		{
			METHODNAME_DEF( TenderContractInfoSerbia, onCheckPharmacyChain )
			BLOG_TRACE_METHOD( m_Logger, fun );

			basar::appl::SystemEventManager::getInstance().fire( "CheckPharmacyChainRequested" );
		}

		const libtender::domMod::tender::TenderTypeCollectionPtr TenderContractInfoSerbia::getTypeMapping()
		{
			return m_TenderTypeCollection;
		}

		const libtender::domMod::tender::TenderStateEnum TenderContractInfoSerbia::getStateIndex()
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

		const libtender::domMod::tender::TenderStateEnum TenderContractInfoSerbia::getStateData()
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

		const libtender::domMod::tender::TenderTypeEnum TenderContractInfoSerbia::getType()
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

		const basar::Date TenderContractInfoSerbia::getDateTo()
		{
			return basar::cmnutil::DateTimeTools::getDate( ui.deContractEndDate->date().year(), ui.deContractEndDate->date().month(), ui.deContractEndDate->date().day() );
		}

		bool TenderContractInfoSerbia::isFilled()
		{
			METHODNAME_DEF( TenderContractInfoSerbia, isFilled )
			BLOG_TRACE_METHOD( m_Logger, fun );

			if( ui.leContractNo->text().isEmpty() ) return false;

			if( !ui.leCustomerNo->text().isEmpty() )
			{
				if( !ui.leBranchno->text().isEmpty()	)
				{
					return true;
				}
			}

			if( !ui.lePharmacyChainNo->text().isEmpty() )
			{
				if( !ui.leCustomerGroupID->text().isEmpty() )
				{
					return true;
				}
			}

			return false;
		}

		bool TenderContractInfoSerbia::isCustomerSelected()
		{
			METHODNAME_DEF( TenderContractInfoSerbia, isCustomerSelected )
			BLOG_TRACE_METHOD( m_Logger, fun );

			if((ui.leCustomerNo->text().isEmpty() || ui.leBranchno->text().isEmpty()) && ui.lePharmacyChainNo->text().isEmpty())
			{
				return false;
			}

			return true;
		}

		bool TenderContractInfoSerbia::isPharmacyChainInFocus()
		{
			if( ui.lePharmacyChainNo->hasFocus() ||
				ui.btnPharmacyChainNo->hasFocus()  )
			{
				return true;
			}

			return false;
		}

		bool TenderContractInfoSerbia::isCustomerInFocus()
		{
			if( ui.leCustomerNo->hasFocus() ||
				ui.btnCustomerNo->hasFocus()  )
			{
				return true;
			}

			return false;
		}

		bool TenderContractInfoSerbia::isContractPeriodValid()
		{
			if( ui.deContractBeginDate->date() >= ui.deContractEndDate->date() )
			{
				return false;
			}

			return true;
		}

		void TenderContractInfoSerbia::onContractPeriodClicked( bool isChecked )
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

		void TenderContractInfoSerbia::onContractDateChanged()
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

		void TenderContractInfoSerbia::onStateChanged( int index )
		{
			METHODNAME_DEF( TenderContractInfoSerbia, onStateChanged )
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

		void TenderContractInfoSerbia::onContractInfoChanged()
		{
			METHODNAME_DEF( TenderContractInfoSerbia, onTextChanged )
			BLOG_TRACE_METHOD( m_Logger, fun );

			basar::appl::SystemEventManager::getInstance().fire( "ContractInfoChanged" );
		}

		void TenderContractInfoSerbia::onCustomerChecked( bool checked )
		{
			if( checked )
			{
				setCustomerVisible( true );
			}
			else
			{
				setCustomerVisible( false );
			}
		}

		void TenderContractInfoSerbia::onPharmacyChainChecked( bool checked )
		{
			if( checked )
			{
				setPharmacyChainVisible( true );
			}
			else
			{
				setPharmacyChainVisible( false );
			}
		}

		void TenderContractInfoSerbia::onShowProtocols()
		{
			METHODNAME_DEF( TenderContractInfoSerbia, onShowProtocols )
			BLOG_TRACE_METHOD( m_Logger, fun );

			basar::appl::SystemEventManager::getInstance().fire( "ProtocolsRequested" );
		}

        void TenderContractInfoSerbia::matchToTender( basar::db::aspect::AccessorPropertyTable_YIterator yitTender )
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

            if(!ui.lePharmacyChainNo->text().isEmpty())
            {
				yitTender.setInt32( properties::PHARMACY_CHAIN_NO , ui.lePharmacyChainNo->text().toInt() );
            }
			else
			{
				yitTender.setInt32( properties::PHARMACY_CHAIN_NO, 0 );
			}

			if(!ui.leCustomerGroupID->text().isEmpty())
            {
				yitTender.setString( properties::CUSTOMERGROUPID , ui.leCustomerGroupID->text().toStdString() );
            }
			else
			{
				yitTender.setString( properties::CUSTOMERGROUPID, "00" );
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
                yitTender.setInt32( properties::CUSTOMER_NO, ui.leCustomerNo->text().toInt() );
				yitTender.setInt32( properties::ACC_CUSTOMER_NO, ui.leCustomerNo->text().toInt() );
            }
			else
			{
				yitTender.setInt32( properties::CUSTOMER_NO, 0 );
				yitTender.setInt32( properties::ACC_CUSTOMER_NO, 0 );
			}

			if(!ui.leBranchno->text().isEmpty())
			{
				yitTender.setInt16( properties::BRANCH_NO, ui.leBranchno->text().toShort() );
				yitTender.setInt16( properties::ACC_BRANCH_NO, ui.leBranchno->text().toShort() );
			}
			else
			{
				yitTender.setInt16( properties::BRANCH_NO, 0 );
				yitTender.setInt16( properties::ACC_BRANCH_NO, 0 );
			}

			yitTender.setInt32( properties::PARTNER_NO, 0 );

			yitTender.setDate( properties::DATE_FROM, basar::cmnutil::DateTimeTools::getDate(ui.deContractBeginDate->date().year(),
                                                                                                   ui.deContractBeginDate->date().month(),
                                                                                                   ui.deContractBeginDate->date().day()));

			yitTender.setDate( properties::DATE_TO, basar::cmnutil::DateTimeTools::getDate(ui.deContractEndDate->date().year(),
                                                                                                 ui.deContractEndDate->date().month(),
                                                                                                 ui.deContractEndDate->date().day()));
            yitTender.setString( properties::PHARMACY_GROUPID, "000" );//This field only applicable for switzerland
		}

		void TenderContractInfoSerbia::matchFromTender( basar::db::aspect::AccessorPropertyTable_YIterator yitTender, bool hasTenderArticles )
		{
			if( yitTender.isContainedAndSet( properties::TENDER_NO ) )
			{
				ui.leTenderNo->setText( QString::number( yitTender.getInt32( properties::TENDER_NO ) ) );
			}

			if( yitTender.isContainedAndSet( properties::CONTRACT_NO  ) )
			{
				ui.leContractNo->setText( yitTender.getString( properties::CONTRACT_NO ).c_str() );
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

			if( yitTender.isContainedAndSet( properties::CUSTOMER_NO ) && yitTender.getInt32(properties::CUSTOMER_NO ) != 0 )
			{
				ui.rbCustomer->setChecked( true );
				ui.leCustomerNo->setText( QString::number( yitTender.getInt32( properties::CUSTOMER_NO ) ) );
				ui.leBranchno->setText( QString::number( yitTender.getInt16( properties::BRANCH_NO ) ) );
			}
			else if( yitTender.isContainedAndSet( properties::PHARMACY_CHAIN_NO  ) && yitTender.getInt32(properties::PHARMACY_CHAIN_NO ) != 0 )
			{
				ui.rbPharmacyChain->setChecked( true );
				ui.lePharmacyChainNo->setText( QString::number( yitTender.getInt32( properties::PHARMACY_CHAIN_NO ) ) );
				ui.leCustomerGroupID->setText( QString::fromStdString( yitTender.getString( properties::CUSTOMERGROUPID ) ) );
			}
			else
			{
				ui.rbCustomer->setChecked( true );
			}

			if( yitTender.isContainedAndSet( properties::CUSTOMER_NO  ) )
			{
				ui.leCustomerNo->setText( QString::number( yitTender.getInt32( properties::CUSTOMER_NO ) ) );
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

		void TenderContractInfoSerbia::matchFromCustomer( basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer )
		{
			METHODNAME_DEF( TenderContractInfoSerbia, matchFromCustomer )
			BLOG_TRACE_METHOD( m_Logger, fun );

			if (yitCustomer.isNull() || yitCustomer.isEnd())
			{
				ui.leCustomerNo->clear();
			}
			else
			{
				ui.leCustomerNo->setText( QString::number( yitCustomer.getInt32( properties::CUSTOMER_NO ) ) );
				ui.leBranchno->setText( QString::number( yitCustomer.getInt16( properties::BRANCH_NO ) ) );
			}
		}

		void TenderContractInfoSerbia::matchFromPharmacyChainNo( basar::db::aspect::AccessorPropertyTable_YIterator yitPharmacyChain )
		{
			METHODNAME_DEF( TenderContractInfoSerbia, matchFromPharmacyChainNo )
			BLOG_TRACE_METHOD( m_Logger, fun );

			if( yitPharmacyChain.isContainedAndSet( properties::PHARMACY_CHAIN_NO ) )
			{
				ui.lePharmacyChainNo->setText( QString::number( yitPharmacyChain.getInt32( properties::PHARMACY_CHAIN_NO ) ) );
			}
			else
			{
				ui.lePharmacyChainNo->clear();
			}
		}

		void TenderContractInfoSerbia::matchToCustomerSearch( basar::db::aspect::AccessorPropertyTable_YIterator customerSearchYit )
		{
			METHODNAME_DEF( TenderContractInfoSerbia, matchToCustomerSearch )
			BLOG_TRACE_METHOD( m_Logger, fun );

			basar::Int32 customerNo = ui.leCustomerNo->text().toInt();
			basar::Int16 branchNo = ui.leBranchno->text().toShort();

			if( 0 < customerNo )
			{
				customerSearchYit.setInt32( properties::CUSTOMER_NO, customerNo );

			}
			if( 0 < branchNo )
			{
				customerSearchYit.setInt16( properties::BRANCH_NO, ui.leBranchno->text().toShort() );
			}
		}

		void TenderContractInfoSerbia::matchToPharmacyChainSearch( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyChainSearchYit )
		{
			METHODNAME_DEF( TenderContractInfoSerbia, matchToPartnerSearch )
			BLOG_TRACE_METHOD( m_Logger, fun );

			basar::Int32 pharmacyChainNo = ui.lePharmacyChainNo->text().toInt();
			if( 0 < pharmacyChainNo )
			{
				pharmacyChainSearchYit.setInt32( properties::PHARMACY_CHAIN_NO, pharmacyChainNo );
			}

			if( !ui.leCustomerGroupID->text().isEmpty() )
			{
				pharmacyChainSearchYit.setString( properties::CUSTOMERGROUPID, ui.leCustomerGroupID->text().toStdString() );
			}
		}

        void TenderContractInfoSerbia::setupContractInfo()
        {
            METHODNAME_DEF( TenderContractInfoSerbia, setupContractInfo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			QRegExp rxTenderNo("^[1-9][0-9]{1,7}$");
			QValidator *tenderNoValidator = new QRegExpValidator( rxTenderNo, ui.leTenderNo );

            ui.leTenderNo->setAccessibleName(properties::TENDER_NO.getName().c_str()  );
            ui.leTenderNo->setValidator     ( tenderNoValidator );

            ui.leContractNo->setAccessibleName(properties::CONTRACT_NO.getName().c_str());

			QRegExp rxContractNo( "^[\\s\\S]{,48}$" );
			QValidator *contractNoValidator = new QRegExpValidator( rxContractNo, ui.leContractNo );
			ui.leContractNo->setValidator( contractNoValidator );

			QRegExp rxPharmacyChainNo("^[0-9]{1,7}$");
			QValidator *pharmacyChainNoValidator = new QRegExpValidator( rxPharmacyChainNo, ui.lePharmacyChainNo );

			ui.lePharmacyChainNo->setAccessibleName(properties::PHARMACY_CHAIN_NO.getName().c_str());
            ui.lePharmacyChainNo->setValidator     ( pharmacyChainNoValidator );

            ui.cbType->setAccessibleName(properties::TENDER_TYPE.getName().c_str());
            ui.cbStatus->setAccessibleName(properties::TENDER_STATE.getName().c_str());

			QRegExp rxCustomerNo("^[1-9][0-9]{1,7}$");
			QValidator *CustomerNoValidator = new QRegExpValidator( rxCustomerNo, ui.leCustomerNo );

            ui.leCustomerNo->setAccessibleName( properties::CUSTOMER_NO.getName().c_str());
            ui.leCustomerNo->setValidator     ( CustomerNoValidator );

			QRegExp rxBranchNo("^[0-9]{1,2}$");
			QValidator *BranchNoValidator = new QRegExpValidator( rxBranchNo, ui.leBranchno );

			ui.leBranchno->setAccessibleName( properties::BRANCH_NO.getName().c_str() );
			ui.leBranchno->setValidator( BranchNoValidator );

            ui.deContractBeginDate->setAccessibleName( properties::DATE_FROM.getName().c_str());
            ui.deContractBeginDate->setDisplayFormat ( "dd.MM.yyyy");

            ui.deContractEndDate->setAccessibleName( properties::DATE_TO.getName().c_str());
            ui.deContractEndDate->setDisplayFormat ( "dd.MM.yyyy");

			ui.deContractBeginDate->setDate( QDate( QDate::currentDate().year(),  1, 1  ));
			ui.deContractEndDate->setDate  ( QDate( QDate::currentDate().year(), 12, 31 ));


			ui.lblPharmacyChain->setGeometry( ui.lblCustomerNo->geometry() );
			ui.lePharmacyChainNo->setGeometry( ui.leCustomerNo->geometry() );
			ui.btnPharmacyChainNo->setGeometry( ui.btnCustomerNo->geometry() );

			ui.lblCustomerGroupID->setGeometry( ui.lblBranchNo->geometry() );
			ui.leCustomerGroupID->setGeometry( ui.leBranchno->geometry() );

			QRegExp rxCustomerGroup( tr("^[0-1]{1}([0-9]{1})?$").toLocal8Bit().constData() );
			QValidator *customerGroupValidator = new QRegExpValidator( rxCustomerGroup, ui.leCustomerGroupID );
			ui.leCustomerGroupID->setValidator( customerGroupValidator  );
        }

        void TenderContractInfoSerbia::focusInEvent( QFocusEvent* event)
        {
            event;
            ui.leTenderNo->setFocus();
        }

        void TenderContractInfoSerbia::focusOutEvent( QFocusEvent* event)
        {
            event;
        }

        void TenderContractInfoSerbia::resetAllFields()
        {
            ui.leTenderNo->clear();
            ui.leContractNo->clear();

			ui.lePharmacyChainNo->clear();

            ui.cbType->setCurrentIndex(0);
            ui.cbStatus->setCurrentIndex(0);

			ui.lePharmacyChainNo->clear();
			ui.leCustomerGroupID->clear();

			ui.leBranchno->clear();
			ui.leCustomerNo->clear();

			resetCustomer();
			ui.rbCustomer->setChecked( true );
			setPharmacyChainVisible( false );

			ui.deContractBeginDate->setDate( QDate( QDate::currentDate().year(), 1, 1 ) );
			ui.deContractEndDate->setDate( QDate( QDate::currentDate().year(), 12, 31 ) );

			ui.cbStatus->setItemData(3, tr("Closed auto"), Qt::UserRole-1);
        }

		void TenderContractInfoSerbia::resetCustomer()
		{
			ui.leBranchno->clear();
		}

		void TenderContractInfoSerbia::prepareForCloning()
		{
			ui.rbCustomer->setChecked( true );
		}

		void TenderContractInfoSerbia::setCustomerVisible( bool visible )
		{
			if( visible )
			{
				ui.lblCustomerNo->setVisible( true );
				ui.lblBranchNo->setVisible( true );
				ui.leCustomerNo->setVisible( true );
				ui.leBranchno->setVisible( true );
				ui.btnCustomerNo->setVisible( true );
			}
			else
			{
				ui.lblCustomerNo->setVisible( false );
				ui.lblBranchNo->setVisible( false );
				ui.leCustomerNo->setVisible( false );
				ui.leBranchno->setVisible( false );
				ui.btnCustomerNo->setVisible( false );

				ui.leCustomerNo->clear();
				ui.leBranchno->clear();
			}
		}

		void TenderContractInfoSerbia::setPharmacyChainVisible( bool visible )
		{
			ui.lblPharmacyChain->setVisible(visible);
			ui.lePharmacyChainNo->setVisible(visible);
			ui.btnPharmacyChainNo->setVisible(visible);

			ui.lblCustomerGroupID->setVisible(visible);
			ui.leCustomerGroupID->setVisible(visible);
		}

		void TenderContractInfoSerbia::setFocusOnFirstEmptyField()
		{
			if( ui.leContractNo->text().isEmpty() )
			{
				ui.leContractNo->setFocus();
				return;
			}

			if( ui.lePharmacyChainNo->text().isEmpty() )
			{
				ui.lePharmacyChainNo->setFocus();
				return;
			}

			if( ui.leCustomerNo->text().isEmpty() )
			{
				ui.leCustomerNo->setFocus();
			}
		}

        void TenderContractInfoSerbia::injectStatusMapping( libtender::domMod::tender::TenderStateCollectionPtr statusMapping )
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

        void TenderContractInfoSerbia::injectTypeMapping( libtender::domMod::tender::TenderTypeCollectionPtr typeMapping )
        {
            m_TenderTypeCollection = typeMapping;

			using namespace libtender::domMod::tender;
			TenderTypeCollection::const_iterator iter = m_TenderTypeCollection->begin();
            for( ; iter != m_TenderTypeCollection->end(); ++iter )
            {
                ui.cbType->addItem( QString::fromLocal8Bit( iter->second.c_str() ), QVariant( iter->first ) );
            }
        }

		void TenderContractInfoSerbia::setCustomerNo( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( TenderContractInfoSerbia, setCustomerNo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( yit.isContainedAndSet( properties::CUSTOMER_NO ) )
			{
				ui.leCustomerNo->setText( QString::number( yit.getInt32( properties::CUSTOMER_NO ) ));
			}
		}

		void TenderContractInfoSerbia::setPharmacyChainNo( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
            METHODNAME_DEF( TenderContractInfoSerbia, setPharmacyChainNo )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( yit.isContainedAndSet( properties::PARTNER_NO ) )
			{
				ui.lePharmacyChainNo->setText( QString::number( yit.getInt32( properties::PARTNER_NO ) ));
			}
		}

		void TenderContractInfoSerbia::disableAll()
		{
			ui.leContractNo->setDisabled( true );
			ui.cbPurchaseOrderProposal->setDisabled( true );
			ui.cbStatus->setDisabled( true );
			ui.cbType->setDisabled( true );

			ui.gbTenderFor->setDisabled( true );
			ui.gbContractPeriod->setDisabled( true );
		}

		void TenderContractInfoSerbia::disableAllExceptState( bool disable, bool hasTenderArticles )
		{
			if( disable )
			{
				ui.leContractNo->setDisabled( true );
				ui.cbPurchaseOrderProposal->setDisabled( true );
				ui.cbType->setDisabled( true );
				ui.gbTenderFor->setDisabled( true );
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

				ui.lePharmacyChainNo->setEnabled( true );
				ui.btnPharmacyChainNo->setEnabled( true );
				ui.leCustomerNo->setEnabled( true );
				ui.btnCustomerNo->setEnabled( true );
				ui.gbTenderFor->setEnabled( true );
				ui.gbContractPeriod->setEnabled( true );
			}
		}

    } // end namespace viewConn
} // end namespace tender
