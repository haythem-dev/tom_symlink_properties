//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "customerlistmaintainvc.h"

#include "libtender/domainmodule/customerlist/icustomerlistdm.h"

#include <libutil/util.h>
#include <libutil/viewconn.h>

#include "loggerpool/tenderloggerpool.h"

#include "libtender/domainmodule/libtender_properties_definitions.h"

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
        CustomerListMaintainVC::CustomerListMaintainVC( QWidget* parent )
        : BaseVC( parent ),
          m_EventsWiring  ( "CustomerListMaintainVC::EventsWiring" )
        {
            //METHODNAME_DEF( CustomerListMaintainVC, CustomerListMaintainVC )
            //BLOG_TRACE_METHOD( m_Logger, fun );

			m_StartSearchAction = new QAction( tr( "&Search" ), this );
			m_StartSearchAction->setShortcut( Qt::Key_F2 );  
			this->addAction( m_StartSearchAction );

            ui.setupUi( this );
            wireEvents();

			QRegExp rxCustomerNo("^[1-9][0-9]{1,7}$");
			QValidator *CustomerNoValidator = new QRegExpValidator( rxCustomerNo, ui.leCustomerNo );

            ui.leCustomerNo->setAccessibleName( properties::CUSTOMER_NO.getName().c_str());
            ui.leCustomerNo->setValidator     ( CustomerNoValidator );

			QRegExp rxBranchNo("^[0-9]{1,2}$");
			QValidator *BranchNoValidator = new QRegExpValidator( rxBranchNo, ui.leBranchNo );

            ui.leBranchNo->setAccessibleName( properties::BRANCH_NO.getName().c_str());
            ui.leBranchNo->setValidator     ( BranchNoValidator );

			ui.btnDelete->setDisabled( true );

            initHeaderLabels();
            setupTable();
        }

        /////////////////////////////////////////////////////////////////////////////////////////////
        /*!    \throw  no-throw */
        /////////////////////////////////////////////////////////////////////////////////////////////
        CustomerListMaintainVC::~CustomerListMaintainVC()
        {
            //METHODNAME_DEF( CustomerListMaintainVC, ~CustomerListMaintainVC )
            //BLOG_TRACE_METHOD( m_Logger, fun );
        }

		void CustomerListMaintainVC::wireEvents()
        {
            if( m_EventsWiring.isSet() )
            {
                return;
            }

			connect( ui.btnCustomerPicker,	SIGNAL( clicked()					),
            this,							SLOT  ( onSearchCustomerRequested() ));

			connect( ui.btnAdd,				SIGNAL( clicked()					),
            this,							SLOT  ( onAddCustomerRequested()	));

			connect( ui.btnDelete,			SIGNAL( clicked()					),
            this,							SLOT  ( onDeleteCustomerRequested()	));
											
			connect( ui.btnClose,			SIGNAL( clicked()					),
            this,							SLOT  ( onCancelRequested()			));

			connect( ui.leCustomerNo,		SIGNAL( returnPressed()				), 
			this,							SLOT  ( onCheckCustomerRequested()	));
			
			connect( ui.leCustomerNo,		SIGNAL( editingFinished()			), 
			this,							SLOT  ( onCheckCustomerRequested()	));

			connect( ui.twCustomerList,		SIGNAL( activated(const QModelIndex &) ),
			this,							SLOT  ( onEditCustomerRequested()      ));

			connect( m_StartSearchAction,	SIGNAL( triggered()					), 
			this,							SLOT  ( onSearchCustomerRequested() ));
										     
            m_EventsWiring.set();
        }

		void CustomerListMaintainVC::setupTable()
		{
			METHODNAME_DEF( CustomerListMaintainVC, setupTable )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

		    ui.twCustomerList->setABCVerticalHeaderPolicy();
            ui.twCustomerList->setColumnCount              ( m_LstLabels.size()					);
            ui.twCustomerList->setHorizontalHeaderLabels   ( m_LstLabels						);
            ui.twCustomerList->setResizeRowsToContents     ( true                               );
            ui.twCustomerList->setResizeColumnsToContents  ( true                               );
            ui.twCustomerList->setVerticalScrollBarPolicy  ( Qt::ScrollBarAsNeeded              );
            ui.twCustomerList->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded              );
            ui.twCustomerList->setSelectionBehavior        ( QAbstractItemView::SelectRows      );
            ui.twCustomerList->setSelectionMode            ( QAbstractItemView::SingleSelection );
            ui.twCustomerList->setSortingEnabled(false);
            ui.twCustomerList->setEnabled(true);
		}

		basar::Int32 CustomerListMaintainVC::initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
		{
			METHODNAME_DEF( CustomerListMaintainVC, initMatcher )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
		
			m_CustomerCollectionMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twCustomerList, propTab );
			m_CustomerCollectionMatcher.setMatchingPolicy(
					basar::gui::tie::INSERT | 
					basar::gui::tie::SKIP |
					basar::gui::tie::CONVERTTYPE |
					basar::gui::tie::DISREGARD_DELETED );
			m_CustomerCollectionMatcher.transformProperties( ui.twCustomerList->horizontalHeaderLabels(), m_LstOriginalAttributes );

			basar::Int32 count = 0;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();
			while( !yit.isEnd() )
			{
				if( yit.getState(properties::CUSTOMER_NO ) != basar::SS_DELETE)
					++count;
				++yit;
			}
			return count;
		}

		void CustomerListMaintainVC::initHeaderLabels()
		{
			METHODNAME_DEF( CustomerListMaintainVC, initHeaderLabels )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );
			
			m_LstLabels.push_back( tr( "Customer No" ) );
			m_LstOriginalAttributes.push_back( properties::CUSTOMER_NO );

			m_LstLabels.push_back( tr( "Customer Name" ) );
			m_LstOriginalAttributes.push_back( properties::PHARMACY_NAME );

			m_LstLabels.push_back( tr( "Branch No" ) );
			m_LstOriginalAttributes.push_back( properties::BRANCH_NO );
		}

		basar::db::aspect::AccessorPropertyTable_YIterator CustomerListMaintainVC::getSelectedCustomer( basar::db::aspect::AccessorPropertyTableRef positionPropTab )
		{
			initMatcher( positionPropTab );
			m_CustomerCollectionMatcher.LeftToRight();

			return m_CustomerCollectionMatcher.getSelectedRight();
		}

		basar::gui::tie::WidgetReturnEnum CustomerListMaintainVC::show()
		{
			METHODNAME_DEF( CustomerListMaintainVC, show )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			ui.twCustomerList->selectRow( 0 );
			ui.twCustomerList->setFocus();

			resetCustomer();

			ui.leCustomerNo->setFocus();

            return basar::gui::tie::getWidgetReturnType( exec() );
		}

		void CustomerListMaintainVC::resetCustomer()
		{
			ui.leCustomerNo->clear();
			ui.leCustomerName->clear();
			ui.leBranchNo->clear();
		}

		int CustomerListMaintainVC::getCustomerNoToBeAdded()
		{
			return ui.leCustomerNo->text().toInt();
		}

		int CustomerListMaintainVC::getBranchNo()
		{
			return ui.leBranchNo->text().toInt();
		}

		void CustomerListMaintainVC::matchToCustomerList( basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer )
		{
			METHODNAME_DEF( CustomerListMaintainVC, matchToCustomerList )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

		}

		void CustomerListMaintainVC::matchFromCustomerList( libtender::domMod::customerlist::ICustomerListDMPtr customerList )
		{
			METHODNAME_DEF( CustomerListMaintainVC, matchFromCustomerList )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			basar::db::aspect::AccessorPropertyTableRef customerListPropTab = customerList->get();
			basar::Int32 rowCount = initMatcher( customerListPropTab );

			libutil::gui::SignalBlocker signalblocker( ui.twCustomerList );
			ui.twCustomerList->setRowCount( rowCount );
			m_CustomerCollectionMatcher.RightToLeft( 0, ui.twCustomerList->rowCount() );
			ui.twCustomerList->setResizeColumnsToContents( true );
			signalblocker.unblock();
			
			ui.twCustomerList->setEnabled( true );
			ui.btnAdd->setDefault( true );
			ui.leCustomerNo->setFocus();
		}
		
		void CustomerListMaintainVC::matchToCustomer( basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer )
		{
			METHODNAME_DEF( CustomerListMaintainVC, matchFromCustomer )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( !ui.leCustomerNo->text().isEmpty() )
			{
				yitCustomer.setInt32( properties::CUSTOMER_NO, ui.leCustomerNo->text().toInt() );
			}

			if( !ui.leCustomerName->text().isEmpty() )
			{
				yitCustomer.setString( properties::PHARMACY_NAME, ui.leCustomerName->text().toLocal8Bit().data() );
			}

			if( !ui.leBranchNo->text().isEmpty() )
			{
				yitCustomer.setInt16( properties::BRANCH_NO, ui.leBranchNo->text().toShort() );
			}
		}

		void CustomerListMaintainVC::matchFromCustomer( basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer )
		{
			METHODNAME_DEF( CustomerListMaintainVC, matchToCustomer )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( yitCustomer.isNull() ||yitCustomer.isEnd() )
			{
				return;
			}

			if( yitCustomer.isContainedAndSet( properties::CUSTOMER_NO ) )
			{
				ui.leCustomerNo->setText( QString::number( yitCustomer.getInt32( properties::CUSTOMER_NO ) ) );
			}

			if( yitCustomer.isContainedAndSet( properties::PHARMACY_NAME ) )
			{
				ui.leCustomerName->setText( QString::fromLocal8Bit( yitCustomer.getString( properties::PHARMACY_NAME ).c_str() ) );
			}

			if( yitCustomer.isContainedAndSet( properties::BRANCH_NO ) )
			{
				ui.leBranchNo->setText( QString::number( yitCustomer.getInt16( properties::BRANCH_NO ) ) );
			}
		}

		void CustomerListMaintainVC::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer )
		{
			METHODNAME_DEF( CustomerListMaintainVC, matchToSearchCriteria )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			if( !ui.leCustomerNo->text().isEmpty() )
			{
				yitCustomer.setInt32( properties::CUSTOMER_NO, ui.leCustomerNo->text().toInt() );
			}
		}

		void CustomerListMaintainVC::onAddCustomerRequested()
		{
			METHODNAME_DEF( CustomerListMaintainVC, onAddCustomerRequested )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			libutil::gui::WaitCursor bc( this );

			ui.btnAdd->setText( "Add" );
			ui.btnDelete->setDisabled( true );

            basar::appl::SystemEventManager::getInstance().fire( "AddCustomerRequested" );
		}

		void CustomerListMaintainVC::onEditCustomerRequested()
		{
			METHODNAME_DEF( CustomerListMaintainVC, onEditCustomerRequested )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			libutil::gui::WaitCursor bc( this );

			ui.btnAdd->setText( "Edit" );
			ui.btnDelete->setEnabled( true );

            basar::appl::SystemEventManager::getInstance().fire( "EditCustomerRequested" );
		}

		void CustomerListMaintainVC::onDeleteCustomerRequested()
		{
			METHODNAME_DEF( CustomerListMaintainVC, onDeleteCustomerRequested )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );
			
			libutil::gui::WaitCursor bc( this );
            basar::appl::SystemEventManager::getInstance().fire( "DeleteCustomerRequested" );
		}

		void CustomerListMaintainVC::onSearchCustomerRequested()
		{
			METHODNAME_DEF( CustomerListMaintainVC, onSearchCustomerRequested )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			if( !ui.leCustomerNo->hasFocus() && !ui.btnCustomerPicker->hasFocus() )
			{
				return;
			}

			libutil::gui::WaitCursor bc( this );
            basar::appl::SystemEventManager::getInstance().fire( "SearchCustomerRequested" );
		}

		void CustomerListMaintainVC::onCheckCustomerRequested()
		{
			METHODNAME_DEF( CustomerListMaintainVC, onCheckCustomerRequested )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			libutil::gui::WaitCursor bc( this );
            basar::appl::SystemEventManager::getInstance().fire( "CheckCustomerRequested" );
		}

		void CustomerListMaintainVC::onCancelRequested()
		{
			METHODNAME_DEF( CustomerListMaintainVC, onCancelRequested )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			this->close();
		}

		bool CustomerListMaintainVC::isCustomerAlreadySelected()
		{
			METHODNAME_DEF( CustomerListMaintainVC, isCustomerAlreadySelected )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			if( ui.twCustomerList->findItems( ui.twCustomerList->item( ui.twCustomerList->currentItem()->row(), 0 )->text(), Qt::MatchExactly ).isEmpty() )
			{
				return false;
			}

			return true;
		}

		void CustomerListMaintainVC::displayCustomerAlreadySelected()
		{
			showWarningMessage( tr("Customer is already selected.").toLocal8Bit().constData() );
		}

		void CustomerListMaintainVC::displayInvalidCustomer()
		{
			showWarningMessage( tr("Customer is invalid.").toLocal8Bit().constData() );
		}

	} //end namespace viewConn
} //end namespace tender
