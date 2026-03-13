#include "scheduleddeliverymaintainvc.h"

#include <libutil/util.h>
#include <libutil/viewconn.h>

#include "loggerpool/tenderloggerpool.h"

#include <libtender/domainmodule/scheduleddelivery/ischeduleddelivery.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryheader.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliverypositioncollection.h>
#include <libtender/domainmodule/tender/itender.h>
#include <libtender/domainmodule/tender/itenderpositioncollection.h>
#include "libtender/domainmodule/libtender_properties_definitions.h"

#include <libtender/domainmodule/constants.h>

#include "qvalidatoritemdelegate.h"
#include "checkboxtabletransformer.h"


namespace tender
{
    namespace viewConn
    {
        ScheduledDeliveryMaintainVC::ScheduledDeliveryMaintainVC( QWidget* parent )
        : IScheduledDeliveryMaintain( parent ),
          m_EventsWiring  ( "ScheduledDeliveryMaintainVC::EventsWiring" )
        {
            ui.setupUi( this );
            wireEvents();

			m_ColumnColour = QColor::fromRgb( 135, 206, 250 );

            initDeliveryHeaderLabels();
            setupDeliveryTables();

			ui.dEDeliveryDate->setAccessibleName( QString::fromLocal8Bit( properties::DELIVERY_DATE.getName().c_str() ) );
			ui.dEDeliveryDate->setDate( QDate::currentDate().addDays( 1 ) );

			ui.leCustomer->setAccessibleName( QString::fromLocal8Bit( properties::CUSTOMER_NO.getName().c_str() ) );
			ui.leBranch->setAccessibleName( QString::fromLocal8Bit( properties::BRANCH_NO.getName().c_str() ) );
        }

        ScheduledDeliveryMaintainVC::~ScheduledDeliveryMaintainVC()
        {

        }

		void ScheduledDeliveryMaintainVC::wireEvents()
        {
            if( m_EventsWiring.isSet() )
            {
                return;
            }

            connect( ui.btnSave,				SIGNAL( clicked()                ),
            this,								SLOT  ( onSaveRequested()        ));
												
			connect( ui.btnCancel,				SIGNAL( clicked()                ),
            this,								SLOT  ( onDialogCloseRequested() ));

            connect( ui.btnCustomerList,		SIGNAL( clicked()                 ),
            this,								SLOT  ( onSelectCustomerRequested() ));

			connect( ui.twAvailableArticles,	SIGNAL( activated(const QModelIndex &) ),
			this,								SLOT  ( onSelectItemRequested()    ));

			connect( ui.btnSelectSingleItem,	SIGNAL( clicked() ),
			this,								SLOT  ( onSelectItemRequested()    ));

			connect( ui.twSelectedArticles,		SIGNAL( activated(const QModelIndex &) ),
			this,								SLOT  ( onDeselectItemRequested()      ));

			connect( ui.btnDeselectSingleItem,  SIGNAL( clicked() ),
			this,								SLOT  ( onDeselectItemRequested()  ));

			connect( ui.btnSelectAll,			SIGNAL( clicked() ),
			this,								SLOT  ( onSelectAllRequested() ) );
												
			connect( ui.btnDeselectAll,			SIGNAL( clicked() ),
			this,								SLOT  ( onDeselectAllRequested() ) );

			connect( ui.twSelectedArticles,		SIGNAL( cellChanged(int, int) ),
			this,								SLOT  ( onSelectedItemsCellChanged(int, int) ) );
										     
            m_EventsWiring.set();
        }

		void ScheduledDeliveryMaintainVC::setupDeliveryTables()
		{
			METHODNAME_DEF( ScheduledDeliveryMaintainVC, setupDeliveryTables )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

		    ui.twAvailableArticles->setABCVerticalHeaderPolicy();
            ui.twAvailableArticles->setColumnCount              ( m_LstLabelsAvailableItems.size()   );
            ui.twAvailableArticles->setHorizontalHeaderLabels   ( m_LstLabelsAvailableItems          );
            ui.twAvailableArticles->setResizeRowsToContents     ( true                               );
            ui.twAvailableArticles->setResizeColumnsToContents  ( true                               );
            ui.twAvailableArticles->setVerticalScrollBarPolicy  ( Qt::ScrollBarAsNeeded              );
            ui.twAvailableArticles->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded              );
            ui.twAvailableArticles->setSelectionBehavior        ( QAbstractItemView::SelectRows      );
            ui.twAvailableArticles->setSelectionMode            ( QAbstractItemView::SingleSelection );
            ui.twAvailableArticles->setSortingEnabled(false);
            ui.twAvailableArticles->setEnabled(true);
		    
		    ui.twSelectedArticles->setABCVerticalHeaderPolicy();
            ui.twSelectedArticles->setColumnCount              ( m_LstLabelsSelectedItems.size()    );
            ui.twSelectedArticles->setHorizontalHeaderLabels   ( m_LstLabelsSelectedItems           );
            ui.twSelectedArticles->setResizeRowsToContents     ( true                               );
            ui.twSelectedArticles->setResizeColumnsToContents  ( true                               );
            ui.twSelectedArticles->setVerticalScrollBarPolicy  ( Qt::ScrollBarAsNeeded              );
            ui.twSelectedArticles->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded              );
            ui.twSelectedArticles->setSelectionBehavior        ( QAbstractItemView::SelectRows      );
            ui.twSelectedArticles->setSelectionMode            ( QAbstractItemView::SingleSelection );
			ui.twSelectedArticles->setEditableColumn           ( 0, false                           );
			ui.twSelectedArticles->setEditableColumn           ( 1, false                           );
			ui.twSelectedArticles->setEditableColumn           ( 2, true                            );
			ui.twSelectedArticles->setEditableColumn           ( 3, false                           );
			ui.twSelectedArticles->setEditableColumn           ( 4, false                           );
			ui.twSelectedArticles->setBackgroundColorColumn    ( 2, m_ColumnColour					);
			ui.twSelectedArticles->setBackgroundColorColumn    ( 3, m_ColumnColour					);
			ui.twSelectedArticles->setBackgroundColorColumn    ( 4, m_ColumnColour					);
			ui.twSelectedArticles->setSortingEnabled(false);
            ui.twSelectedArticles->setEnabled(true);

			const QRegExp rx("[1-9]+[0-9]*");
			QValidator *validator = new QRegExpValidator(rx, this);

			QValidatorItemDelegate* delegate = new QValidatorItemDelegate(this); 
			delegate->setColumnValidator( 2, validator ); 
			ui.twSelectedArticles->setItemDelegate(delegate);
		}

		basar::Int32 ScheduledDeliveryMaintainVC::initAvailableItemsMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
		{
			METHODNAME_DEF( TenderMaintainVC, initAvailableItemsMatcher )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );
		
			m_AvailableItemsMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twAvailableArticles, propTab );
			m_AvailableItemsMatcher.transformProperties( ui.twAvailableArticles->horizontalHeaderLabels(), m_LstOriginalAttributesAvailableItems );

			basar::Int32 count = 0;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();
			while (!yit.isEnd())
			{
				++count;
				++yit;
			}
			return count;
		}

		basar::Int32 ScheduledDeliveryMaintainVC::initSelectedItemsMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
		{
			METHODNAME_DEF( TenderMaintainVC, initSelectedItemsMatcher )
			BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			m_SelectedItemsMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twSelectedArticles, propTab );
			m_SelectedItemsMatcher.setMatchingPolicy(
					basar::gui::tie::INSERT | 
					basar::gui::tie::SKIP |
					basar::gui::tie::CONVERTTYPE |
					basar::gui::tie::DISREGARD_DELETED );
			m_SelectedItemsMatcher.transformProperties( ui.twSelectedArticles->horizontalHeaderLabels(), m_LstOriginalAttributesSelectedItems );

			CheckBoxTableTransformer t(
				ui.twSelectedArticles,
				tr( "Purchase order proposal" ).toStdString(),
				properties::PURCHASE_ORDER_PROPOSAL_FLAG.getName(),
				"1",
				"0");
			m_SelectedItemsMatcher.push_back(t);

			basar::Int32 count = 0;
			basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();
			
			while (!yit.isEnd())
			{
				if (yit.getState(properties::ARTICLE_CODE) != basar::SS_DELETE)
					++count;
				++yit;
			}

			return count;
		}

		void ScheduledDeliveryMaintainVC::initDeliveryHeaderLabels()
		{
			METHODNAME_DEF( ScheduledDeliveryMaintainVC, initDeliveryHeaderLabels )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );
			
			m_LstLabelsAvailableItems.push_back( tr( "Article Code" ) );
			m_LstOriginalAttributesAvailableItems.push_back( properties::ARTICLE_CODE );

			m_LstLabelsAvailableItems.push_back( tr( "Article Name" ) );
			m_LstOriginalAttributesAvailableItems.push_back( properties::ARTICLE_NAME );

			m_LstLabelsAvailableItems.push_back( tr( "Available quantity" ) );
			m_LstOriginalAttributesAvailableItems.push_back( properties::QUANTITY_AVAILABLE );

			//----------------------------------------------------------------------------

			m_LstLabelsSelectedItems.push_back( tr( "Article Code" ) );
			m_LstOriginalAttributesSelectedItems.push_back( properties::ARTICLE_CODE );

			m_LstLabelsSelectedItems.push_back( tr( "Article Name" ) );
			m_LstOriginalAttributesSelectedItems.push_back( properties::ARTICLE_NAME );

			m_LstLabelsSelectedItems.push_back( tr( "Quantity" ) );
			m_LstOriginalAttributesSelectedItems.push_back( properties::QUANTITY );

			m_LstLabelsSelectedItems.push_back( tr( "Purchase order proposal" ) );
			m_LstOriginalAttributesSelectedItems.push_back( properties::PURCHASE_ORDER_PROPOSAL_FLAG );

			m_LstLabelsSelectedItems.push_back( tr( "Branch no for POP" ) );
			m_LstOriginalAttributesSelectedItems.push_back( properties::BRANCH_NO_POP );
		}

		basar::db::aspect::AccessorPropertyTable_YIterator ScheduledDeliveryMaintainVC::getCurrentAvailableArticle( basar::db::aspect::AccessorPropertyTableRef positionPropTab )
		{
			initAvailableItemsMatcher( positionPropTab );
			m_AvailableItemsMatcher.LeftToRight();

			return m_AvailableItemsMatcher.getSelectedRight();
		}

		basar::db::aspect::AccessorPropertyTable_YIterator ScheduledDeliveryMaintainVC::getCurrentSelectedArticle( basar::db::aspect::AccessorPropertyTableRef positionPropTab )
		{
			initSelectedItemsMatcher( positionPropTab );
			m_SelectedItemsMatcher.LeftToRight();

			return m_SelectedItemsMatcher.getSelectedRight();
		}

		basar::Int32 ScheduledDeliveryMaintainVC::getCustomer()
		{
			return ui.leCustomer->text().toInt();
		}

		basar::gui::tie::WidgetReturnEnum ScheduledDeliveryMaintainVC::show()
		{
			METHODNAME_DEF(ScheduledDeliveryMaintainVC, show)
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun);

            if(constants::REGION_FR == m_Region || !m_NeedsCustomerSelection)
            {
                ui.grpDeliveryInfo->resize(211, 111);
                ui.btnCustomerList->setVisible(false);
            }
			else
			{
				ui.grpDeliveryInfo->resize(301, 111);
				ui.btnCustomerList->setVisible(true);
			}

			ui.twAvailableArticles->selectRow(0);
			ui.twAvailableArticles->setFocus();

			//Hide columns if POP functionality is set to FALSE in UADM
			if(false == m_POPFunctionality)
			{
				ui.twSelectedArticles->setColumnHidden(3, true);
				ui.twSelectedArticles->setColumnHidden(4, true);
			}

            return basar::gui::tie::getWidgetReturnType(exec());
		}
		 
		void ScheduledDeliveryMaintainVC::setPOPFunctionality(basar::I18nString popFunctionality)
		{
			if("TRUE" == popFunctionality)
			{
				m_POPFunctionality = true;
			}
			else
			{
				m_POPFunctionality = false;
			}
		}

        void ScheduledDeliveryMaintainVC::setRegion(basar::Int16 region)
        {
            m_Region = region;
        }

		void ScheduledDeliveryMaintainVC::setNeedsCustomerSelection(bool needsCustomerSelection)
		{
			m_NeedsCustomerSelection = needsCustomerSelection;
		}

        void ScheduledDeliveryMaintainVC::setCustomer(basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer)
        {  
            if(!yitCustomer.isNull() && !yitCustomer.isEnd())
            {
                ui.leCustomer->setText(QString::number(yitCustomer.getInt32(properties::CUSTOMER_NO)));
                ui.leBranch->setText(QString::number(yitCustomer.getInt16(properties::BRANCH_NO)));
            }
        }

		void ScheduledDeliveryMaintainVC::matchFromScheduledDelivery( libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery )
		{
			METHODNAME_DEF( ScheduledDeliveryMaintainVC, matchFromScheduledDelivery )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			basar::db::aspect::AccessorPropertyTable_YIterator selectedItem = scheduledDelivery->getPositionCollection()->get().begin();
			basar::db::aspect::AccessorPropertyTable_YIterator availableItem =  m_AvailableItemsMatcher.getRight().begin();

			for( ; !availableItem.isEnd(); ++availableItem )
			{
				for( ; !selectedItem.isEnd(); ++selectedItem )
				{
					if( availableItem.getString( properties::ARTICLE_CODE ) == selectedItem.getString( properties::ARTICLE_CODE ) )
					{
						if( availableItem.getInt32( properties::QUANTITY_AVAILABLE ) < selectedItem.getInt32( properties::QUANTITY ) )
						{
							selectedItem.setInt32( properties::QUANTITY, availableItem.getInt32( properties::QUANTITY_AVAILABLE ) );
						}
					}
				}

				selectedItem = scheduledDelivery->getPositionCollection()->get().begin();
			}

			basar::db::aspect::AccessorPropertyTable_YIterator scheduledDeliveryHeader = scheduledDelivery->getHeader()->get();
			basar::db::aspect::AccessorPropertyTableRef scheduledDeliveryHeaderPropTab = scheduledDeliveryHeader.getPropertyTable();
			
			basar::gui::tie::MatcherRef matcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.grpDeliveryInfo, scheduledDeliveryHeaderPropTab );
			matcher.RightToLeft();

			if( !scheduledDeliveryHeader.isContainedAndSet( properties::DELIVERY_DATE ) )
			{
				ui.dEDeliveryDate->setDate( QDate::currentDate().addDays( 1 ) );
			}

			//--------------------------------------------------------------------------------------------------------------------------

			matchFromPositionCollection(scheduledDelivery->getPositionCollection()->get());
		}

		void ScheduledDeliveryMaintainVC::matchToScheduledDelivery(libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery)
		{
			METHODNAME_DEF(ScheduledDeliveryMaintainVC, matchToScheduledDelivery)
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun);

			basar::db::aspect::AccessorPropertyTable_YIterator scheduledDeliveryHeader = scheduledDelivery->getHeader()->get();

			scheduledDeliveryHeader.setDate(properties::DELIVERY_DATE, basar::Date(ui.dEDeliveryDate->date().year(), ui.dEDeliveryDate->date().month(), ui.dEDeliveryDate->date().day()));
            scheduledDeliveryHeader.setInt16(properties::BRANCH_NO, ui.leBranch->text().toShort());
            scheduledDeliveryHeader.setInt32(properties::CUSTOMER_NO, ui.leCustomer->text().toInt());
            
			matchToPositionCollection(scheduledDelivery->getPositionCollection()->get());
		}

		void ScheduledDeliveryMaintainVC::matchFromTender(libtender::domMod::tender::ITenderPtr tender)
		{
			METHODNAME_DEF(ScheduledDeliveryMaintainVC, matchFromTender)
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun);

			basar::appl::SystemEventManager::getInstance().fire("GetAvailableQuantitiesRequested");

			basar::db::aspect::AccessorPropertyTableRef tenderPositionCollectionPropTab = tender->getPositionCollection()->get();

			basar::Int32 rowCount = initAvailableItemsMatcher(tenderPositionCollectionPropTab);
			
			libutil::gui::SignalBlocker signalblocker(ui.twAvailableArticles);
			ui.twAvailableArticles->setRowCount(rowCount);
			m_AvailableItemsMatcher.RightToLeft(0, ui.twAvailableArticles->rowCount());
			ui.twAvailableArticles->setResizeColumnsToContents( true );
			signalblocker.unblock();

            if( constants::REGION_FR == m_Region || 
			   (constants::REGION_BG == m_Region && 1 == tender->getHeader().getInt32(properties::TENDER_TYPE)) || 
			   (constants::REGION_RS == m_Region && 0 == tender->getHeader().getInt32(properties::PHARMACY_CHAIN_NO)) ) 	
			{
                ui.leCustomer->setText(QString::number(tender->getHeader().getInt32(properties::CUSTOMER_NO)));
                ui.leBranch->setText(QString::number(tender->getHeader().getInt16(properties::BRANCH_NO)));
            }

			ui.dEDeliveryDate->setMinimumDate(QDate::currentDate().addDays(1));
		}

		void ScheduledDeliveryMaintainVC::matchFromPositionCollection( basar::db::aspect::AccessorPropertyTableRef positionPropTab )
		{
			METHODNAME_DEF( ScheduledDeliveryMaintainVC, matchFromPositionCollection )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			basar::Int32 rowCount = initSelectedItemsMatcher( positionPropTab );

			libutil::gui::SignalBlocker signalblocker( ui.twSelectedArticles );
			ui.twSelectedArticles->clearContext();
			ui.twSelectedArticles->setRowCount(rowCount);
			m_SelectedItemsMatcher.RightToLeft(0, rowCount);
			ui.twSelectedArticles->setResizeColumnsToContents(true);
			signalblocker.unblock();
			
			//Block signals while setting flags for the item to prevent an error!!!
			for( int row = 0; row < rowCount; ++row )
			{	
				signalblocker.block();
				setBranchNoCellsEditable(row);
				signalblocker.unblock();
			}
		}

		void ScheduledDeliveryMaintainVC::matchToPositionCollection( basar::db::aspect::AccessorPropertyTableRef positionPropTab )
		{
			METHODNAME_DEF( ScheduledDeliveryMaintainVC, matchToPositionCollection )
            BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

			initSelectedItemsMatcher( positionPropTab );
			m_SelectedItemsMatcher.LeftToRight();
		}

		void ScheduledDeliveryMaintainVC::onSaveRequested()
		{
			METHODNAME_DEF( ScheduledDeliveryMaintainVC, onSaveClicked )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			if( !isSavable() ) return;
			
			libutil::gui::WaitCursor bc( this );
			basar::appl::SystemEventManager::getInstance().fire( "SaveDeliveryRequested" );

			this->close();
		}

		void ScheduledDeliveryMaintainVC::onDialogCloseRequested()
		{
			METHODNAME_DEF( ScheduledDeliveryMaintainVC, onDialogCloseRequested )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

			this->close();
		}

        void ScheduledDeliveryMaintainVC::onSelectCustomerRequested()
        {
            METHODNAME_DEF( ScheduledDeliveryMaintainVC, onSelectCustomerRequested )
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

            basar::appl::SystemEventManager::getInstance().fire("SelectCustomerRequested");
        }

		bool ScheduledDeliveryMaintainVC::isArticleSelected()
		{
			METHODNAME_DEF(ScheduledDeliveryMaintainVC, isArticleSelected)
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun);

			if(ui.twSelectedArticles->findItems(ui.twAvailableArticles->item(ui.twAvailableArticles->currentItem()->row(), 0)->text(), Qt::MatchExactly).isEmpty())
			{
				return false;
			}

			return true;
		}

		bool ScheduledDeliveryMaintainVC::isArticleSelected(int row)
		{
			METHODNAME_DEF(ScheduledDeliveryMaintainVC, isArticleSelected)
            BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun);

			if(ui.twSelectedArticles->findItems(ui.twAvailableArticles->item(row, 0)->text(), Qt::MatchExactly).isEmpty())
			{
				return false;
			}

			return true;
		}

		bool ScheduledDeliveryMaintainVC::isSavable()
		{
			if("" == ui.leCustomer->text() && "" == ui.leBranch->text() )
			{
				displayCustomerNotSelected();
				return false;
			}

			if(0 == ui.twSelectedArticles->rowCount())
			{
				displayNoArticlesSelected();
				return false;
			}
				
			if(!areQuantitiesSet())
			{
				displayQuantitiesNotSet();
				return false;
			}

			if(!basar::appl::EventReturnType(basar::appl::SystemEventManager::getInstance().fire("CheckDateRequested")).isOK())
			{
				return false;
			}

			return true;
		}

		bool ScheduledDeliveryMaintainVC::areQuantitiesSet()
		{
			int columnQuantity = 2;

			for(int row = 0; ui.twSelectedArticles->rowCount() > row; ++row)
			{
				if(!ui.twSelectedArticles->item(row, columnQuantity) || ui.twSelectedArticles->item(row, columnQuantity)->text().toInt() == 0)
				{
					return false;
				}
            }

			return true;
		}

		void ScheduledDeliveryMaintainVC::setBranchNoCellsEditable(int row)
		{
			int columnCheckBoxPOP = 3;
			int columnBranchNoPOP = 4;

			int rows = ui.twSelectedArticles->rowCount();
			int columns = ui.twSelectedArticles->columnCount();

			rows;
			columns;

			QTableWidgetItem* itemCheckBox = ui.twSelectedArticles->item(row, columnCheckBoxPOP);
			QTableWidgetItem* itemBranchNo = ui.twSelectedArticles->item(row, columnBranchNoPOP);

			if( Qt::Checked == itemCheckBox->checkState() )
			{
				itemBranchNo->setFlags(itemBranchNo->flags() | Qt::ItemIsEditable);
			}
			else
			{
				itemBranchNo->setFlags(itemBranchNo->flags() &~ Qt::ItemIsEditable);
			}
		}

		void ScheduledDeliveryMaintainVC::displayArticleAlreadySelected()
		{
			showWarningMessage(tr("Article is already selected.").toLocal8Bit().constData());
		}

		void ScheduledDeliveryMaintainVC::displayDeliveryAlreadyExists()
		{
			showWarningMessage(tr("A delivery with this date and customer already exists.").toLocal8Bit().constData());
		}

		void ScheduledDeliveryMaintainVC::displayInvalidDeliveryDate()
		{
			showWarningMessage(tr("The delivery date is out of the conrtract period.").toLocal8Bit().constData());
		}

		void ScheduledDeliveryMaintainVC::displayQuantityExceedance(int maxQuantity)
		{
			std::stringstream ss;
			ss << maxQuantity;

			showWarningMessage(tr("Quantity for this article exceeded. \nAvailable quantity is ").toLocal8Bit().constData() + ss.str());
		}

		void ScheduledDeliveryMaintainVC::displayCustomerNotSelected()
		{
			showWarningMessage(tr("Please select a customer before saving the scheduled delivery.").toLocal8Bit().constData());
		}

		void ScheduledDeliveryMaintainVC::displayNoArticlesSelected()
		{
			showWarningMessage(tr("At least one article needs to be selected before saving the delivery.").toLocal8Bit().constData());
		}

		void ScheduledDeliveryMaintainVC::displayQuantitiesNotSet()
		{
			showWarningMessage(tr("Quantities are not set.").toLocal8Bit().constData());
		}

		void ScheduledDeliveryMaintainVC::displayPositionHasNoAvailableQuantity()
		{
			showInfoMessage(tr("Article has no available quantity.").toLocal8Bit().constData());
		}

		void ScheduledDeliveryMaintainVC::displayBranchNoNotValid()
		{
			showInfoMessage(tr("Branch no is not valid.\nDefault branch no set.").toLocal8Bit().constData());
		}

		void ScheduledDeliveryMaintainVC::onSelectItemRequested()
		{
			if(isArticleSelected())
			{
				displayArticleAlreadySelected();
				return;
			}

			basar::appl::SystemEventManager::getInstance().fire( "SelectPositionRequested" );
		}

		void ScheduledDeliveryMaintainVC::onDeselectItemRequested()
		{
			basar::appl::SystemEventManager::getInstance().fire( "DeselectPositionRequested" );
		}

		void ScheduledDeliveryMaintainVC::onSelectAllRequested()
		{
			basar::appl::SystemEventManager::getInstance().fire( "SelectPositionCollectionRequested" );
		}

		void ScheduledDeliveryMaintainVC::onDeselectAllRequested()
		{
			basar::appl::SystemEventManager::getInstance().fire( "DeselectPositionCollectionRequested" );
		}

		void ScheduledDeliveryMaintainVC::onSelectedItemsCellChanged( int row, int column )
		{
			//Check if Quantity has exceeded.
			if( 2 == column )
			{
				basar::appl::SystemEventManager::getInstance().fire( "ChangeQuantityRequested" );
			}

			//Set the cell for 'Branch no for POP' editiable.
			if( 3 == column )
			{
				//Block signals while setting flags for the item to prevent an error!!!
				ui.twSelectedArticles->blockSignals( true );
				setBranchNoCellsEditable( row );
				ui.twSelectedArticles->blockSignals( false );
			}

			//Check if branch no is valid.
			if( 4 == column )
			{
				basar::appl::SystemEventManager::getInstance().fire( "ChangeBranchNoPOPRequested" );
			}
		}

		void ScheduledDeliveryMaintainVC::clear()
		{
			ui.leCustomer->clear();
			ui.leBranch->clear();
			ui.twAvailableArticles->setRowCount(0);
			ui.twSelectedArticles->setRowCount(0);
		}

	} //end namespace viewConn
} //end namespace tender
