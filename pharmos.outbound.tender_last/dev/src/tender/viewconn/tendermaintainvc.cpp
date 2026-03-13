#include "tendermaintainvc.h"
#include "itendercontractinfo.h"
#include "itenderarticleinfo.h"
#include <libutil/util.h>
#include <libutil/viewconn.h>
#include <loggerpool/tenderloggerpool.h>
#include <libtender/domainmodule/tender/itender.h>
#include <libtender/domainmodule/tender/itenderpositioncollection.h>
#include <libtender/domainmodule/libtender_properties_definitions.h>
#include <libtender/domainmodule/tender/tendertypecollection.h>
#include <libtender/domainmodule/tender/tenderstatecollection.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddelivery.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliverycollectiondm.h>
#include "util.h"
#include "doublevalidator.h"
#include <limits>
#include <libtender/domainmodule/tender/tenderstateenum.h>
#include <libtender/domainmodule/constants.h>
#include "flagtransformer.hpp"
#include "discountflags_transformer.h"

namespace tender
{
namespace viewConn
{

TenderMaintainVC::TenderMaintainVC( QWidget * parent, const basar::Int16 area )
: BaseVC( parent ),
  m_Area( area ),
  m_Logger( libtender::LoggerPool::getLoggerViewConn() ),
  m_EventsWiring( "TenderMaintainVC::EventsWiring" ),
  m_StartSearchAction( NULL ),
  m_AreDeliveriesLoaded( false ),
  m_CloneDeliveriesFlag( false )
{
	METHODNAME_DEF( TenderMaintainVC, TenderMaintainVC )
	BLOG_TRACE_METHOD( m_Logger, fun );

	ui.setupUi( this );

	m_StartSearchAction = new QAction( tr( "&Search" ), this );
	m_StartSearchAction->setShortcut( Qt::Key_F2 );  
	this->addAction( m_StartSearchAction );

	wireEvents();
	initTenderPositionLabels();
    setupTenderPositionTable();

	initDeliveryLabels();
	setupDeliveryTable();

	initDeliveryPositionLabels();
	setupDeliveryPositionTable();

	ui.tabWidget->setCurrentIndex( 0 );
	ui.tabWidget->setTabEnabled( 1, false );
    ui.pbCreateOrders->setDisabled(true);

	ui.pbModifyDelivery->setDisabled( true );
	ui.pbDeleteDelivery->setDisabled( true );
	ui.pbClone->setDisabled( true );
}

TenderMaintainVC::~TenderMaintainVC()
{
	METHODNAME_DEF( TenderMaintainVC, ~TenderMaintainVC )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

void TenderMaintainVC::injectContractInfo( tender::viewConn::ITenderContractInfoPtr contractInfo )
{
	m_uiContractInfo = contractInfo.get();
	ui.contractInfoLayout->insertWidget( 0, m_uiContractInfo );
}

void TenderMaintainVC::setTenderState( libtender::domMod::tender::TenderStateEnum tenderState)
{
    m_uiContractInfo->setTenderState(tenderState);
}

void TenderMaintainVC::setupScheduleTab( )
{
    if(m_Area == constants::REGION_CH)
    {
        ui.tabWidget->removeTab(ui.tabWidget->indexOf(ui.tbScheduling));
    }
    else
    {
        ui.pbCreateOrders->setDisabled(true);
        ui.pbCreateOrders->hide();
    }
}

void TenderMaintainVC::injectArticleInfo( tender::viewConn::ITenderArticleInfoPtr articleInfo )
{
    m_uiArticleInfo = articleInfo.get();
    ui.articleInfoLayout->insertWidget(0, m_uiArticleInfo );

    // This code neeeds to be moved to appropriate place
    connect( m_uiArticleInfo, SIGNAL( inputCodeQtyValid() ),
    this,					  SLOT  ( OnInputCheckCodeQtyValid() ) );
    
    connect( m_uiArticleInfo, SIGNAL( inputCodeQtyInvalid() ),
    this,					  SLOT  ( OnInputCheckCodeQtyInvalid() ) );

    m_uiArticleInfo->addArtValidator();
}

void TenderMaintainVC::setPOPFunctionality( basar::I18nString popFunctionality )
{
	if( "TRUE" == popFunctionality )
	{
		m_POPFunctionality = true;
	}
	else
	{
		m_POPFunctionality = false;
	}
}

void TenderMaintainVC::setCloneDeliveriesFlag(bool flag)
{
	m_CloneDeliveriesFlag = flag;
}

basar::gui::tie::WidgetReturnEnum TenderMaintainVC::show()
{
	METHODNAME_DEF( TenderMaintainVC, show )
    BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

	//Hide columns if POP functionality is set to FALSE in UADM
	if( false == m_POPFunctionality )
	{
		ui.tblDeliveryItems->setColumnHidden( 4, true );
		ui.tblDeliveryItems->setColumnHidden( 5, true );
		ui.tblDeliveryItems->setColumnHidden( 6, true );
	}

    return basar::gui::tie::getWidgetReturnType( exec() );
}

void TenderMaintainVC::wireEvents()
{
	if( m_EventsWiring.isSet() )
	{
		return;
	}

	connect( ui.pbSave,			       SIGNAL( clicked() ),
	this,						       SLOT  ( OnSaveClicked() ) );

	connect( ui.pbSaveAndContinue,     SIGNAL( clicked() ),
	this,						       SLOT  ( OnSaveClicked() ) );

	connect( ui.pbClose,		       SIGNAL( clicked() ),
	this,						       SLOT  ( OnCloseClicked() ) );

	connect( ui.pbAddItem,		       SIGNAL( clicked() ),
	this,						       SLOT  ( OnSaveItemClicked() ) );

	connect( ui.pbAddAndNewItem,       SIGNAL( clicked() ),
	this,						       SLOT  ( OnSaveAndNewItemClicked() ) );

	connect( ui.pbCancelModifyItem,    SIGNAL( clicked() ),
	this,							   SLOT  ( OnCancelModifyItemClicked() ) );

	connect( ui.pbModifyItem,		   SIGNAL( clicked() ),
	this,							   SLOT  ( OnModifyItemClicked() ) );

	connect( ui.pbDeleteItem,		   SIGNAL( clicked() ),
	this,							   SLOT  ( OnDeleteItemClicked() ) );

	connect( ui.pbNewItem,			   SIGNAL( clicked() ),
	this,							   SLOT  ( OnNewItemClicked() ) );

	connect( ui.tblItems,			   SIGNAL( dataSelectionChanged() ),
	this,							   SLOT  ( OnTblItemsDataSelectionChanged() ) );

	connect( ui.tblItems,			   SIGNAL( itemClicked(QTableWidgetItem*) ),
	this,							   SLOT  ( OnTblItemsDataSelectionChanged() ) );

	connect( ui.tblItems,			   SIGNAL( cellDoubleClicked(int, int) ),
	this,							   SLOT  ( OnModifyItemClicked() )            );

	connect( m_StartSearchAction,	   SIGNAL( triggered() ), 
	this,							   SLOT  ( OnStartSearch() ) );
									   
	connect( ui.pbAddDelivery,         SIGNAL( clicked () ),
	this,							   SLOT  ( OnAddDeliveryClicked() ) );
									   
    connect( ui.pbCreateOrders,       SIGNAL( clicked () ),
    this,                              SLOT  ( OnCreateOrdersClicked() ) );
									   
	connect( ui.pbModifyDelivery,	   SIGNAL( clicked () ),
	this,							   SLOT  ( OnModifyDeliveryClicked() ) );
									   
	connect( ui.pbDeleteDelivery,	   SIGNAL( clicked () ),
	this,							   SLOT  ( OnDeleteDeliveryClicked() ) );
									   
	connect( ui.pbClone,			   SIGNAL( clicked() ),
	this,							   SLOT  ( OnCloneDeliveryClicked()  ) );
									   
	connect( ui.tblDeliveries,		   SIGNAL( dataSelectionChanged() ),
	this,							   SLOT  ( OnTblDeliveriesDataSelectionChanged() ) );
									   
	connect( ui.tblDeliveries,		   SIGNAL( cellDoubleClicked(int, int) ),
	this,							   SLOT  ( OnModifyDeliveryClicked() ) );
									   
	connect( ui.tabWidget,			   SIGNAL( currentChanged(int) ),
	this,						       SLOT  ( OnTabChanged(int) ) );

	m_EventsWiring.set();
}

void TenderMaintainVC::initTenderPositionLabels()
{
    METHODNAME_DEF( TenderMaintainVC, initTenderPositionLabels )
	BLOG_TRACE_METHOD( m_Logger, fun );

	if (constants::REGION_CH == m_Area)
	{
		m_Labels.push_back(tr("Sts"));
		m_OriginalAttributes.push_back(properties::POSITION_STATUS);
	}
	m_Labels.push_back( tr( "Article Code" ) );
	m_OriginalAttributes.push_back( properties::ARTICLE_CODE );

	m_Labels.push_back( tr( "Article Name" ) );
	m_OriginalAttributes.push_back( properties::ARTICLE_NAME );

	m_Labels.push_back( tr( "Quantity" ) );
	m_OriginalAttributes.push_back( properties::CONTRACT_QTY );

	if( constants::REGION_CH == m_Area )
	{
		m_Labels.push_back( tr( "Remaining Quantity" ) );
		m_OriginalAttributes.push_back( properties::REMAINING_QUANTITY );
	}

	if( constants::REGION_CH != m_Area )
	{
		m_Labels.push_back( tr( "Price" ) );
		m_OriginalAttributes.push_back( properties::CONTRACT_PRICE );
	}

    m_Labels.push_back( tr( "Discount" ) );
    m_OriginalAttributes.push_back( properties::ADD_DISCOUNT_PCT );

	if (constants::REGION_CH == m_Area)
	{
		m_Labels.push_back( tr( "Discount Calc From" ) );
		m_OriginalAttributes.push_back( properties::DISCOUNT_CALC_FROM );

		m_Labels.push_back( tr( "Discount Apply To" ) );
		m_OriginalAttributes.push_back( properties::DISCOUNT_APPLY_TO );

		m_Labels.push_back( tr( "Own Quota Qty" ) );
		m_OriginalAttributes.push_back( properties::OWNQUOTA_QTY );

		m_Labels.push_back( tr( "Own Quota Allowed" ) );
		m_OriginalAttributes.push_back( properties::OWNQUOTA_FLAG );

		m_Labels.push_back( tr( "Recall" ) );
		m_OriginalAttributes.push_back( properties::RECALL_FLAG );
	}

	if (constants::REGION_RS == m_Area)
	{
		m_Labels.push_back(tr("Blocked"));
		m_OriginalAttributes.push_back(properties::TENDER_STATE);
	}
}

void TenderMaintainVC::setupTenderPositionTable()
{
	METHODNAME_DEF( TenderMaintainVC, setupTenderPositionTable )
	BLOG_TRACE_METHOD( m_Logger, fun );

    // Keyboard selection of article name works for RS,
    //if we want for other countries also remove this if else and just keep
    //ui.tblItems->setEditTriggers(QAbstractItemView::NoEditTriggers ); and in tenderpositionacc modify SQl to ORDER BY articlename ASC but make thorough test
	if( constants::REGION_RS == m_Area )
	{
		ui.tblItems->setEditTriggers(QAbstractItemView::NoEditTriggers );
	}
	else
	{
		ui.tblItems->setABCVerticalHeaderPolicy();
	}

	ui.tblItems->setColumnCount( m_Labels.size() );
	ui.tblItems->setHorizontalHeaderLabels( m_Labels );
	ui.tblItems->setResizeRowsToContents( true );
	ui.tblItems->setResizeColumnsToContents( true );
	ui.tblItems->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
	ui.tblItems->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	ui.tblItems->setSelectionBehavior( QAbstractItemView::SelectRows );
	ui.tblItems->setSelectionMode( QAbstractItemView::SingleSelection );

	if( constants::REGION_CH == m_Area )
	{
		basar::Int32 index = 0;
		ui.tblItems->setTextAlignmentColumn( index++, Qt::AlignHCenter | Qt::AlignVCenter ); // Status
		ui.tblItems->setTextAlignmentColumn( index++, Qt::AlignLeft    | Qt::AlignVCenter ); // Article Code
		ui.tblItems->setTextAlignmentColumn( index++, Qt::AlignLeft    | Qt::AlignVCenter ); // Article Name
		ui.tblItems->setTextAlignmentColumn( index++, Qt::AlignRight   | Qt::AlignVCenter ); // Quantity
		ui.tblItems->setTextAlignmentColumn( index++, Qt::AlignRight   | Qt::AlignVCenter ); // Remaining Quantity
		ui.tblItems->setTextAlignmentColumn( index++, Qt::AlignRight   | Qt::AlignVCenter ); // Discount
		ui.tblItems->setTextAlignmentColumn( index++, Qt::AlignRight   | Qt::AlignVCenter ); // Discount Calc From
		ui.tblItems->setTextAlignmentColumn( index++, Qt::AlignRight   | Qt::AlignVCenter ); // Discount Apply To 
		ui.tblItems->setTextAlignmentColumn( index++, Qt::AlignRight   | Qt::AlignVCenter ); // Own Quota Qty
		ui.tblItems->setTextAlignmentColumn( index++, Qt::AlignHCenter | Qt::AlignVCenter ); // Own Quota Allowed
		ui.tblItems->setTextAlignmentColumn( index++, Qt::AlignHCenter | Qt::AlignVCenter ); // Recall
	}

	ui.tblItems->setSortingEnabled( false );
	ui.tblItems->setDisabled( true );
}

void TenderMaintainVC::initDeliveryLabels()
{
	METHODNAME_DEF( TenderMaintainVC, initDeliveryLabels )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_DeliveryLabels.push_back( tr( "Delivery Date" ) );
	m_DeliveryOriginalAttributes.push_back( properties::DELIVERY_DATE );
	m_DeliveryLabels.push_back( tr( "CSC order no" ) );
	m_DeliveryOriginalAttributes.push_back( properties::CSC_ORDER_NO );
}

void TenderMaintainVC::setupDeliveryTable()
{
	METHODNAME_DEF( TenderMaintainVC, setupDeliveryPositionTable )
	BLOG_TRACE_METHOD( m_Logger, fun );

	ui.tblDeliveries->setABCVerticalHeaderPolicy();
	ui.tblDeliveries->setColumnCount( m_DeliveryLabels.size() );
	ui.tblDeliveries->setHorizontalHeaderLabels( m_DeliveryLabels );
	ui.tblDeliveries->setResizeRowsToContents( true );
	ui.tblDeliveries->setResizeColumnsToContents( true );
	ui.tblDeliveries->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
	ui.tblDeliveries->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	ui.tblDeliveries->setSelectionBehavior( QAbstractItemView::SelectRows );
	ui.tblDeliveries->setSelectionMode( QAbstractItemView::SingleSelection );

	ui.tblDeliveries->setSortingEnabled( false );
	ui.tblDeliveries->setDisabled( true );
}

void TenderMaintainVC::initDeliveryPositionLabels()
{
	METHODNAME_DEF( TenderMaintainVC, initDeliveryLabels )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_DeliveryPositionLabels.push_back( tr( "Article Code" ) );
	m_DeliveryPositionOriginalAttributes.push_back( properties::ARTICLE_CODE );

	m_DeliveryPositionLabels.push_back( tr( "Article Name" ) );
	m_DeliveryPositionOriginalAttributes.push_back( properties::ARTICLE_NAME );

	m_DeliveryPositionLabels.push_back( tr( "Quantity" ) );
	m_DeliveryPositionOriginalAttributes.push_back( properties::QUANTITY );

	m_DeliveryPositionLabels.push_back( tr( "Quantity Delivered" ) );
	m_DeliveryPositionOriginalAttributes.push_back( properties::QUANTITY_DELIVERED );

	m_DeliveryPositionLabels.push_back( tr( "Pop no" ) );
	m_DeliveryPositionOriginalAttributes.push_back( properties::ORDER_PROPOSAL_NO );

	m_DeliveryPositionLabels.push_back( tr( "Order no" ) );
	m_DeliveryPositionOriginalAttributes.push_back( properties::PURCHASE_ORDER_NO );

	m_DeliveryPositionLabels.push_back( tr( "Branch no for POP" ) );
	m_DeliveryPositionOriginalAttributes.push_back( properties::BRANCH_NO_POP );
}

void TenderMaintainVC::setupDeliveryPositionTable()
{
	METHODNAME_DEF( TenderMaintainVC, setupDeliveryPositionTable )
	BLOG_TRACE_METHOD( m_Logger, fun );

	ui.tblDeliveryItems->setABCVerticalHeaderPolicy();
	ui.tblDeliveryItems->setColumnCount( m_DeliveryPositionLabels.size() );
	ui.tblDeliveryItems->setHorizontalHeaderLabels( m_DeliveryPositionLabels );
	ui.tblDeliveryItems->setResizeRowsToContents( true );
	ui.tblDeliveryItems->setResizeColumnsToContents( true );
	ui.tblDeliveryItems->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
	ui.tblDeliveryItems->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	//ui.tblDeliveryItems->setSelectionBehavior( QAbstractItemView::SelectRows );
	ui.tblDeliveryItems->setSelectionMode( QAbstractItemView::NoSelection );

	ui.tblDeliveryItems->setSortingEnabled( false );
}

basar::Int32 TenderMaintainVC::initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
{
	METHODNAME_DEF( TenderMaintainVC, initMatcher )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_TenderPositionMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.tblItems, propTab );
	m_TenderPositionMatcher.setMatchingPolicy(
					basar::gui::tie::INSERT | 
					basar::gui::tie::SKIP |
					basar::gui::tie::CONVERTTYPE |
					basar::gui::tie::DISREGARD_DELETED );
	m_TenderPositionMatcher.transformProperties( ui.tblItems->horizontalHeaderLabels(), m_OriginalAttributes );

    if( constants::REGION_RS == m_Area )
    {
        FlagTransformer<basar::Int16> blockedTransformer( tr( "Blocked" ).toLocal8Bit().constData(), 
            properties::TENDER_STATE.getName(), static_cast<basar::Int16>( 8 ), static_cast<basar::Int16>( 1 ), "X", "" );
        m_TenderPositionMatcher.push_back( blockedTransformer );
    }

	if( constants::REGION_CH == m_Area )
	{
		DiscountFlagsTransformer dcfTransformer(properties::DISCOUNT_CALC_FROM.getName(), tr("Discount Calc From").toLocal8Bit().constData());
		m_TenderPositionMatcher.push_back(dcfTransformer);

		DiscountFlagsTransformer datTransformer(properties::DISCOUNT_APPLY_TO.getName(), tr("Discount Apply To").toLocal8Bit().constData());
		m_TenderPositionMatcher.push_back(datTransformer);

		FlagTransformer<basar::Int16> quotaTransformer( tr( "Own Quota Allowed" ).toLocal8Bit().constData(), 
			properties::OWNQUOTA_FLAG.getName(), static_cast<basar::Int16>( 1 ), static_cast<basar::Int16>( 0 ), "X", "" );
		m_TenderPositionMatcher.push_back( quotaTransformer );

		FlagTransformer<basar::Int16> recallTransformer( tr( "Recall" ).toLocal8Bit().constData(), 
			properties::RECALL_FLAG.getName(), static_cast<basar::Int16>( 1 ), static_cast<basar::Int16>( 0 ), "X", "" );
		m_TenderPositionMatcher.push_back( recallTransformer );
	}

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

basar::Int32 TenderMaintainVC::initDeliveriesMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
{
	METHODNAME_DEF( TenderMaintainVC, initDeliveriesMatcher )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_ScheduledDeliveryCollectionMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.tblDeliveries, propTab );
	m_ScheduledDeliveryCollectionMatcher.transformProperties( ui.tblDeliveries->horizontalHeaderLabels(), m_DeliveryOriginalAttributes );

	basar::Int32 count = 0;
	basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();
	
	while (!yit.isEnd())
	{
		++count;
		++yit;
	}
	return count;
}

void TenderMaintainVC::matchFromTender(libtender::domMod::tender::ITenderPtr tender)
{
	METHODNAME_DEF(TenderMaintainVC, matchFromTender)
	BLOG_TRACE_METHOD(m_Logger, fun);

	basar::db::aspect::AccessorPropertyTableRef tenderPositionsPropTab = tender->getPositionCollection()->get();

	basar::Int32 rowCount = initMatcher(tenderPositionsPropTab);

	libutil::gui::SignalBlocker signalblocker(ui.tblItems);
	ui.tblItems->setRowCount(rowCount);
	m_TenderPositionMatcher.RightToLeft(0, ui.tblItems->rowCount());
	ui.tblItems->setResizeColumnsToContents(true);
	signalblocker.unblock();
	ui.tblItems->setEnabled(true);
	resetTenderPosition();
	OnTblItemsDataSelectionChanged();
	ui.pbAddAndNewItem->setDefault(true);
	// validate and set the positions status after shifting data into table
	ValidateAndSetPositionsStatus();
	m_uiArticleInfo->setFocusArticleCode();

	if(0 < rowCount)
	{
		m_uiContractInfo->matchFromTender(tender->getHeader(), true);
        if( m_Area == constants::REGION_CH)
        {
            ui.pbCreateOrders->setEnabled( m_uiContractInfo->getStateData() == libtender::domMod::tender::TS_OK );
        }
	}
	else
	{
		ui.pbAddDelivery->setEnabled(true);
		m_uiContractInfo->matchFromTender(tender->getHeader(), false);
	}
}

void TenderMaintainVC::matchFromTenderPositions( basar::db::aspect::AccessorPropertyTableRef positions) 
{
    METHODNAME_DEF(TenderMaintainVC, matchFromTenderPositions)
	BLOG_TRACE_METHOD(m_Logger, fun);

	basar::Int32 rowCount = initMatcher(positions);

    libutil::gui::SignalBlocker signalblocker(ui.tblItems);
	ui.tblItems->setRowCount(rowCount);
	m_TenderPositionMatcher.RightToLeft(0, ui.tblItems->rowCount());
	ui.tblItems->setResizeColumnsToContents(true);
	signalblocker.unblock();
	ui.tblItems->setEnabled(true);
	resetTenderPosition();
	OnTblItemsDataSelectionChanged();
	ui.pbAddAndNewItem->setDefault(true);
	// validate and set the positions status after shifting data into table
	ValidateAndSetPositionsStatus();
	//ui.leArticleCode->setFocus();
}

void TenderMaintainVC::matchToTender(libtender::domMod::tender::ITenderPtr tender) const
{
	METHODNAME_DEF(TenderMaintainVC, matchToTender)
	BLOG_TRACE_METHOD(m_Logger, fun);

	m_uiContractInfo->matchToTender(tender->getHeader());
}

void TenderMaintainVC::matchToTenderPosition( basar::db::aspect::AccessorPropertyTable_YIterator tenderPosition ) const
{
	METHODNAME_DEF( TenderMaintainVC, matchToTenderPosition )
	BLOG_TRACE_METHOD( m_Logger, fun );
    m_uiArticleInfo->matchToTenderPosition(tenderPosition);
}

void TenderMaintainVC::matchFromTenderPosition( basar::db::aspect::AccessorPropertyTable_YIterator tenderPosition )
{
	METHODNAME_DEF( TenderMaintainVC, matchFromTenderPosition )
	BLOG_TRACE_METHOD( m_Logger, fun );

    m_uiArticleInfo->matchFromTenderPosition(tenderPosition);
	ui.pbAddAndNewItem->setDefault( true );
}

void TenderMaintainVC::matchFromScheduledDeliveryCollection( libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr scheduledDeliveryCollection )
{
	METHODNAME_DEF( TenderMaintainVC, matchFromScheduledDeliveryCollection )
	BLOG_TRACE_METHOD( m_Logger, fun );

	basar::db::aspect::AccessorPropertyTableRef scheduledDeliveriesPropTab = scheduledDeliveryCollection->get();

	basar::Int32 rowCount = initDeliveriesMatcher( scheduledDeliveriesPropTab );

	libutil::gui::SignalBlocker signalblocker( ui.tblDeliveries );
	ui.tblDeliveries->setRowCount( rowCount );
	m_ScheduledDeliveryCollectionMatcher.RightToLeft( 0, ui.tblDeliveries->rowCount() );
	ui.tblDeliveries->setResizeColumnsToContents( true );
	signalblocker.unblock();
	
	ui.tblDeliveries->setEnabled( true );
	ui.tblDeliveries->selectRow(0);

	OnTblDeliveriesDataSelectionChanged();
}

void TenderMaintainVC::matchFromScheduledDeliveryPositionCollection( basar::db::aspect::AccessorPropertyTableRef scheduledDeliveryPositionCollection )
{
	METHODNAME_DEF( TenderMaintainVC, matchFromScheduledDeliveryPositionCollection )
	BLOG_TRACE_METHOD( m_Logger, fun );
	
	basar::gui::tie::MatcherRef matcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.tblDeliveryItems, scheduledDeliveryPositionCollection );
	matcher.transformProperties( ui.tblDeliveryItems->horizontalHeaderLabels(), m_DeliveryPositionOriginalAttributes );

	basar::Int32 rowCount = 0;
	basar::db::aspect::AccessorPropertyTable_YIterator yit = scheduledDeliveryPositionCollection.begin();
	while (!yit.isEnd())
	{
		++rowCount;
		++yit;
	}

	libutil::gui::SignalBlocker signalblocker( ui.tblDeliveryItems );
	ui.tblDeliveryItems->setRowCount( rowCount );
	matcher.RightToLeft( 0, ui.tblDeliveryItems->rowCount() );
	ui.tblDeliveryItems->setResizeColumnsToContents( true );
	signalblocker.unblock();
}

void TenderMaintainVC::matchToArticleSearch( basar::db::aspect::AccessorPropertyTable_YIterator articleSearchYit ) const
{
	METHODNAME_DEF( TenderMaintainVC, matchToArticleSearch )
	BLOG_TRACE_METHOD( m_Logger, fun );
    basar::VarString articleCode = m_uiArticleInfo->getArticleCode();
	if( !articleCode.empty() )
	{
		articleSearchYit.setString( properties::ARTICLE_CODE, articleCode );
	}
	articleSearchYit.setInt32( properties::TENDER_TYPE, m_uiContractInfo->getType() );
}

void TenderMaintainVC::matchFromArticle( basar::db::aspect::AccessorPropertyTable_YIterator article )
{
	METHODNAME_DEF( TenderMaintainVC, matchFromArticle )
	BLOG_TRACE_METHOD( m_Logger, fun );

    m_uiArticleInfo->matchFromArticle(article);
}

void TenderMaintainVC::matchToCustomerSearch( basar::db::aspect::AccessorPropertyTable_YIterator customerSearchYit ) const
{
	METHODNAME_DEF( TenderMaintainVC, matchToCustomerSearch )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_uiContractInfo->matchToCustomerSearch( customerSearchYit );
}

void TenderMaintainVC::matchFromCustomer( basar::db::aspect::AccessorPropertyTable_YIterator customer )
{
	METHODNAME_DEF( TenderMaintainVC, matchFromCustomer )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_uiContractInfo->matchFromCustomer( customer );
}

void TenderMaintainVC::matchToPharmacyGroupSearch( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyGroupSearchYit ) const
{
    METHODNAME_DEF( TenderMaintainVC, matchToPharmacyGroupSearch )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_uiContractInfo->matchToPharmacyGroupSearch( pharmacyGroupSearchYit );
}

void TenderMaintainVC::matchFromPharmacyGroup( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyGroup )
{
    METHODNAME_DEF( TenderMaintainVC, matchFromPharmacyGroup )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    m_uiContractInfo->matchFromPharmacyGroup( pharmacyGroup );
}

void TenderMaintainVC::matchToPartnerSearch( basar::db::aspect::AccessorPropertyTable_YIterator partnerSearchYit ) const
{
	METHODNAME_DEF( TenderMaintainVC, matchToPartnerSearch )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_uiContractInfo->matchToPartnerSearch( partnerSearchYit );
}

void TenderMaintainVC::matchFromPartner( basar::db::aspect::AccessorPropertyTable_YIterator partner )
{
	METHODNAME_DEF( TenderMaintainVC, matchFromPartner )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_uiContractInfo->matchFromPartner( partner );
}

void TenderMaintainVC::matchToPharmacyChainSearch( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyChainPickerYit ) const
{
	METHODNAME_DEF( TenderMaintainVC, matchToPharmacyChainSearch )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_uiContractInfo->matchToPharmacyChainSearch( pharmacyChainPickerYit );
}

void TenderMaintainVC::matchFromPharmacyChain( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyChain )
{
	METHODNAME_DEF( TenderMaintainVC, matchFromPharmacyChain )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_uiContractInfo->matchFromPharmacyChainNo( pharmacyChain );
}

basar::db::aspect::AccessorPropertyTable_YIterator TenderMaintainVC::getCurrentTenderPosition()
{
	METHODNAME_DEF( TenderMaintainVC, getCurrentTenderPosition )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return m_TenderPositionMatcher.getCurrentRight();
}

void TenderMaintainVC::ValidateAndSetPositionsStatus()
{
	// only for switzerland AGP and GEP and discount flags exists to be validated
	if ((constants::REGION_CH == m_Area) && (m_TenderPositionMatcher.isNull() == false))
	{
		basar::db::aspect::AccessorPropertyTableRef positions = m_TenderPositionMatcher.getRight();
		if (positions.isNull() == false)
		{
			int rowindex = 0;
			basar::db::aspect::AccessorPropertyTable_YIterator yit;
			for (yit = positions.begin(); yit != positions.end(); yit++)
			{
				bool valid = true;
				int dcf = 0; // discount calculated from flag
				int dat = 0; // discount applied to flag
				double agp = 0.0; // amedis group price = agp
				double gep = 0.0; // grosso einkauf price = gep
				double pct = 0.0; // manual discount for a tender position
				if ((yit.isContainedAndSet(properties::DISCOUNT_CALC_FROM) == false) ||
					(yit.isContainedAndSet(properties::DISCOUNT_APPLY_TO) == false) ||
					(yit.isContainedAndSet(properties::AGP) == false) ||
					(yit.isContainedAndSet(properties::GEP) == false) ||
					(yit.isContainedAndSet(properties::ADD_DISCOUNT_PCT) == false))
				{
					valid = false;
				}
				if (valid == true)
				{
					// get the values
					dcf = yit.getInt16(properties::DISCOUNT_CALC_FROM);
					dat = yit.getInt16(properties::DISCOUNT_APPLY_TO);
					agp = yit.getDecimal(properties::AGP).toFloat64();
					gep = yit.getDecimal(properties::GEP).toFloat64();
					pct = yit.getDecimal(properties::ADD_DISCOUNT_PCT).toFloat64();
					// get the table column for the position status
					QTableWidgetItem* pItem = ui.tblItems->item(rowindex, 0);
					if (pItem != NULL)
					{
						if (pct > 0.00)
						{
							if (agp > 0.00)
							{
								if ((agp < gep) && (dcf == 0) && (dat == 1))
								{
									// this combination is not valid
									QIcon icon(":/widget/resources/status_exclamation.png");
									if (pItem != NULL)
									{
										pItem->setIcon(icon);
										pItem->setToolTip(tr("Attention: Discount based on GEP will be applied on AGP, but current GEP is higher than AGP!"));
									}
								}
								else if ((gep < agp) && (dat == 0) && (dcf == 1))
								{
									// this combination is not valid
									QIcon icon(":/widget/resources/status_exclamation.png");
									if (pItem != NULL)
									{
										pItem->setIcon(icon);
										pItem->setToolTip(tr("Attention: Discount based on AGP will be applied on GEP, but current AGP is higher than GEP!"));
									}
								}
								else
								{
									// this combination is valid
									QIcon icon(":/widget/resources/status_okay.png");
									if (pItem != NULL)
									{
										pItem->setIcon(icon);
										pItem->setToolTip("");
									}
								}
							}
							else
							{
								if ((dcf == 1) || (dat == 1))
								{
									// use of AGP in discount flags, but AGP is zero
									QIcon icon(":/widget/resources/status_exclamation.png");
									if (pItem != NULL)
									{
										pItem->setIcon(icon);
										pItem->setToolTip(tr("Attention: Use of AGP in discount flags, but current AGP is zero!"));
									}
								}
							}
						}
					}
				}
				rowindex++;
			}
		}
	}
}

std::string TenderMaintainVC::getArticleToBeAdded()
{
    return  m_uiArticleInfo->getArticleCode();
}

void TenderMaintainVC::reset()
{
	m_uiContractInfo->resetAllFields();
	
	resetDeliveries();
	m_AreDeliveriesLoaded = false;

	ui.tabWidget->setCurrentIndex( 0 );
	ui.tabWidget->setTabEnabled( 1, false );
    
    if(m_Area == constants::REGION_CH)
    {
        ui.pbCreateOrders->setDisabled(true);
    }
}

void TenderMaintainVC::resetCustomer()
{
	m_uiContractInfo->resetCustomer();
}

void TenderMaintainVC::resetPharmacyGroup()
{
    m_uiContractInfo->resetPharmacyGroup();
}

void TenderMaintainVC::resetArticle()
{
    m_uiArticleInfo->resetArticleName();
}

void TenderMaintainVC::prepareForCloning()
{
	m_uiContractInfo->prepareForCloning();
}

void TenderMaintainVC::resetTenderPosition()
{
    switch( m_Area ) 
    {
    case constants::REGION_CH:
        resetTenderPositionForCH();
        break;
    default:
        resetTenderPositionForOthers();
        break;
    }
}

void TenderMaintainVC::resetTenderPositionForCH()
{
    m_uiArticleInfo->resetTenderPosition();
    int state = m_uiContractInfo->getStateData();

    basar::Int32 rowCount = ui.tblItems->rowCount();
    if( 0 < rowCount )
    {
        m_uiContractInfo->disableAllExceptState( false, true );
        ui.tabWidget->setTabEnabled( 1, true );
    }
    else
    {
        m_uiContractInfo->disableAllExceptState( false, false );
        ui.tabWidget->setTabEnabled( 1, false );
    }

    ui.tblItems->setEnabled( true );
    ui.pbCreateOrders->setDisabled(true);
    ui.pbNewItem->setDisabled( true );

    if( state == libtender::domMod::tender::TS_OK && 0 < rowCount )
    {
        ui.pbCreateOrders->setEnabled(true);
    }
	
    if( state != libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED )
    {
        ui.pbSave->setEnabled( true );
        ui.pbSaveAndContinue->setEnabled( true );
    }

    if( m_uiContractInfo->isFilled() && 
        state != libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED &&
        state != libtender::domMod::tender::TS_MANUALLY_CLOSED &&
        rowCount == 0 )
    {
        ui.pbNewItem->setEnabled( true );
    }

	ui.pbAddItem->setText( tr("Add item") );
	ui.pbAddAndNewItem->setText( tr("Add && New") );
    m_uiArticleInfo->setDisabled( true );
    ui.pbCancelModifyItem->setDisabled( true );
    m_uiArticleInfo->enableInputValidator( false );
}

void TenderMaintainVC::resetTenderPositionForOthers()
{
    m_uiArticleInfo->resetTenderPosition();

	if(0 < ui.tblItems->rowCount())
	{
		m_uiContractInfo->disableAllExceptState(false, true);
	}
	else
	{
		m_uiContractInfo->disableAllExceptState(false, false);
	}

	ui.tblItems->setEnabled(true);
	ui.pbNewItem->setDisabled(true);

	if(m_uiContractInfo->getStateData() != libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED)
	{
		ui.pbSave->setEnabled(true);
		ui.pbSaveAndContinue->setEnabled(true);
	}

	if(m_uiContractInfo->isFilled() && !isStateClosed())
	{
		ui.pbNewItem->setEnabled(true);
	}
	else
	{
		ui.pbNewItem->setDisabled(true);
	}
	
	if(0 < ui.tblItems->rowCount())
	{
		ui.tabWidget->setTabEnabled(1, true);
	}
	else
	{
		ui.tabWidget->setTabEnabled(1, false);
	}

	ui.pbAddItem->setText( tr("Add item") );
	ui.pbAddAndNewItem->setText( tr("Add && New") );
    m_uiArticleInfo->setDisabled(true);
	ui.pbCancelModifyItem->setDisabled(true);
    m_uiArticleInfo->enableInputValidator(false);
}

void TenderMaintainVC::enableTenderPositionEditing( bool isModifying )
{
	m_uiContractInfo->disableAll();
	
	ui.tblItems->setDisabled( true );
	ui.pbDeleteItem->setDisabled( true );
	ui.pbModifyItem->setDisabled( true );
	ui.pbNewItem->setDisabled( true );
	ui.pbSave->setDisabled( true );
	ui.pbSaveAndContinue->setDisabled( true );
	ui.tabWidget->setTabEnabled( 1, false );

    m_uiArticleInfo->setEnabled( true );
	if( isModifying )
	{
        m_uiArticleInfo->disableArticleCode();
		ui.pbAddItem->setText( tr("Modify item") );
		ui.pbAddAndNewItem->setText( tr("Modify && New") );
	}
	else
	{
        m_uiArticleInfo->enableArticleCode();
	}

	ui.pbCancelModifyItem->setEnabled( true );
    m_uiArticleInfo->enableInputValidator( true );
    m_uiArticleInfo->setFocusArticleCode();
    ui.pbCreateOrders->setDisabled(true);
}

void TenderMaintainVC::resetDeliveries()
{
	ui.tblDeliveries->setRowCount(0);
	ui.tblDeliveryItems->setRowCount(0);
}

basar::db::aspect::AccessorPropertyTable_YIterator TenderMaintainVC::getCurrentScheduledDelivery()
{
	METHODNAME_DEF( TenderMaintainVC, getCurrentScheduledDelivery )
	BLOG_TRACE_METHOD( m_Logger, fun );

	if( m_ScheduledDeliveryCollectionMatcher.isNull() )
	{
		basar::db::aspect::AccessorPropertyTable_YIterator yitSelectedDelivery;

		return yitSelectedDelivery;
	}

	return m_ScheduledDeliveryCollectionMatcher.getCurrentRight();
}

bool TenderMaintainVC::isDeliveryModifiable( basar::db::aspect::AccessorPropertyTable_YIterator deliveryYit )
{
	METHODNAME_DEF( TenderMaintainVC, isDeliveryModifiable )
	BLOG_TRACE_METHOD( m_Logger, fun );

	if( basar::Date::getCurrent().getDate() > deliveryYit.getDate( properties::DELIVERY_DATE ).getDate() ||
		0 != deliveryYit.getInt32( properties::CSC_ORDER_NO ) )
	{
		return false;
	}

	return true;
}

bool TenderMaintainVC::isContractDateValid()
{
	METHODNAME_DEF( TenderMaintainVC, isContractDateValid )
	BLOG_TRACE_METHOD( m_Logger, fun );
 
	return m_uiContractInfo->isContractPeriodValid();
}

bool TenderMaintainVC::isStateClosed()
{
	METHODNAME_DEF(TenderMaintainVC, isStateClosed)
	BLOG_TRACE_METHOD(m_Logger, fun);

	if(m_uiContractInfo->getStateData() == libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED || 
	   m_uiContractInfo->getStateData() == libtender::domMod::tender::TS_MANUALLY_CLOSED)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool TenderMaintainVC::isItemSelected()
{
	METHODNAME_DEF(TenderMaintainVC, isItemSelected)
	BLOG_TRACE_METHOD(m_Logger, fun);

	basar::db::aspect::AccessorPropertyTable_YIterator yit = m_TenderPositionMatcher.getCurrentRight();

	if(!yit.isNull() && !yit.isEnd())
	{
		return true;
	}

	return false;
}

bool TenderMaintainVC::isTenderHeadFilled()
{
	METHODNAME_DEF( TenderMaintainVC, isTenderHeadFilled )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return m_uiContractInfo->isFilled();
}

bool TenderMaintainVC::isCustomerSelected()
{
	METHODNAME_DEF( TenderMaintainVC, isTenderHeadFilled )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return m_uiContractInfo->isCustomerSelected();
}

void TenderMaintainVC::checkContractInfo()
{
	if(m_uiContractInfo->isFilled())
	{
        if(m_Area == constants::REGION_CH)
        {
            ui.pbNewItem->setEnabled(ui.tblItems->rowCount() == 0);
        }
		else if(!isStateClosed())
        {
			ui.pbNewItem->setEnabled(true);

			if(isItemSelected())
			{
				ui.pbModifyItem->setEnabled(true);
				ui.pbDeleteItem->setEnabled(true);
			}
		}
	}
	else
	{
		ui.pbNewItem->setDisabled(true);
		ui.pbModifyItem->setDisabled(true);
		ui.pbDeleteItem->setDisabled(true);
	}
}

void TenderMaintainVC::selectDelivery( basar::db::aspect::AccessorPropertyTable_YIterator scheduledDelivery )
{
	if( !scheduledDelivery.isContainedAndSet( properties::DELIVERY_DATE ) )
	{
		return;
	}

	std::string date = scheduledDelivery.getDate( properties::DELIVERY_DATE ).toStrDate().c_str();
	QList< QTableWidgetItem* > items = ui.tblDeliveries->findItems( QString::fromStdString( date ), Qt::MatchExactly );

	foreach( QTableWidgetItem* item, items )
	{
		ui.tblDeliveries->selectRow( item->row() );
	}
}

void TenderMaintainVC::displayArticleAlreadySelected()
{
	METHODNAME_DEF( TenderMaintainVC, displayArticleAlreadySelected )
	BLOG_TRACE_METHOD( m_Logger, fun );

	showInfoMessage( tr("Article already selected.").toLocal8Bit().constData() );
    m_uiArticleInfo->setFocusArticleCode();
    m_uiArticleInfo->selectArticlecode();
}

void TenderMaintainVC::displayArticleExistsInDelivery()
{
	METHODNAME_DEF( TenderMaintainVC, displayArticleExistsInDelivery )
	BLOG_TRACE_METHOD( m_Logger, fun );

	showErrorMessage( tr("The article exists in at least one delivery.\n Please remove this article first of all from all deliveries before you delete the article from the Tender.\n If a delivery is completed you are not allowed to delete this article from the tender.").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayConractQuantityUndershot( int quantity )
{
	METHODNAME_DEF( TenderMaintainVC, displayConractQuantityUndershot )
	BLOG_TRACE_METHOD( m_Logger, fun );
	
	std::stringstream ss;
	ss << quantity;

	std::string msg = "The quantity of scheduled deliveries is higher than the Tender quantity of ";
	msg.append(ss.str());
	msg.append(" for this article.\nPlease decrease the scheduled quantity before you change the Tender quantity of this article" );

	showErrorMessage( msg, "" );

    m_uiArticleInfo->selectQuantity();
    m_uiArticleInfo->setFocusQuantity();
}

void TenderMaintainVC::displayTenderHeaderMustBeFilled()
{
	METHODNAME_DEF( TenderMaintainVC, displayTenderHeaderMustBeFilled )
	BLOG_TRACE_METHOD( m_Logger, fun );

	showWarningMessage( tr("Contract Info must be filled out before saving.").toLocal8Bit().constData() );
	m_uiContractInfo->setFocusOnFirstEmptyField();
}

void TenderMaintainVC::displayOpenDeliveriesWarning()
{
	showWarningMessage( tr("Attention! There is at least one open delivery for this tender.").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayTenderStateWarning( basar::VarString warning)
{
    showWarningMessage( tr( warning.c_str() ).toLocal8Bit().constData() );
}

void TenderMaintainVC::displayStateInfo()
{
	showWarningMessage( tr("Attention! State of the tender is Capturing Data.").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayPartnerNotValid()
{
	showErrorMessage( tr("Entered partner is not valid!").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayPharmacyChainNotValid()
{
	showErrorMessage( tr("Entered pharmacy chain is not valid!").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayPharmacyGroupNotValid()
{
    showErrorMessage( tr("Entered pharmacy group is not valid!").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayCustomerNotValid()
{
	showErrorMessage( tr("Entered customer is not valid!").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayArticleNotExisting()
{
	showErrorMessage( tr("Entered article is not existing or invalid!").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayPurchaseOrderProposalExisting()
{
	showWarningMessage( tr("Attention! Purchase order proposal already existing.\nPlease clarify with purchase department!").toLocal8Bit().constData() );
}

void TenderMaintainVC::displaySaveTender()
{
	showWarningMessage( tr("Please save the tender before adding/modifying deliveries.").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayCustomerListNotSaved()
{
	showWarningMessage( tr("Customer list has changed, please save the tender before adding/modifying deliveries.").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayDeliveryOutOfContractPeriod()
{
	showWarningMessage( tr("One or more deliveries have a delivery date which is out of the contract period.").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayContractInfoIsNotFilled()
{
	showWarningMessage( tr("Lack of information in contract info. Please fill out the missing informations.").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayCustomerListInvalid()
{
	showWarningMessage( tr("Customer list must at least contain one customer.").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayCustomerNotSelected()
{
	showWarningMessage( tr("Please select a customer before adding/modifying a scheduled delivery!").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayContractPeriodOutOfDate( std::string additionalText )
{
	std::string message;
	message = "Contract period is out of date.\n" + additionalText;

	showWarningMessage( tr( message.c_str() ).toLocal8Bit().constData() );
}

void TenderMaintainVC::displayTenderMustExist()
{
	showWarningMessage( tr("Please save the tender before checking the protocols.").toLocal8Bit().constData() );
}

void TenderMaintainVC::displayCustomerGroupIDNotValid()
{
	showErrorMessage( tr("Customer group ID is not valid!").toLocal8Bit().constData() );
}

void TenderMaintainVC::disableAllExceptState(bool disable)
{
	if(disable)
	{
		if(0 == ui.tblItems->rowCount())
		{
			m_uiContractInfo->disableAllExceptState(disable, false);
		}
		else
		{
			m_uiContractInfo->disableAllExceptState(disable, true);
		}

		ui.pbDeleteItem->setDisabled( true );
		ui.pbModifyItem->setDisabled( true );
		ui.pbNewItem->setDisabled( true );
        m_uiArticleInfo->setDisabled( true );
		ui.tblDeliveryItems->setDisabled( true );
		ui.pbAddDelivery->setDisabled( true );
		ui.pbModifyDelivery->setDisabled( true );
		ui.pbDeleteDelivery->setDisabled( true );
		ui.pbClone->setDisabled( true );

		if( m_uiContractInfo->getStateData() == libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED  )
		{
			ui.pbSave->setDisabled( true );
			ui.pbSaveAndContinue->setDisabled( true );
		}
	}
	else
	{
		if( 0 == ui.tblItems->rowCount() )
		m_uiContractInfo->disableAllExceptState( disable, false );
		else
		m_uiContractInfo->disableAllExceptState( disable, true );

		int istate = m_uiContractInfo->getStateData();
		if( isTenderHeadFilled() && 
		  ( istate != libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED &&
		    istate != libtender::domMod::tender::TS_MANUALLY_CLOSED) )
		{
			ui.pbNewItem->setEnabled( true );
		}

		libtender::domMod::tender::TenderStateEnum state = m_uiContractInfo->getStateIndex();
		if( m_uiContractInfo->isFilled() &&
			state != libtender::domMod::tender::TS_AUTOMATICALLY_CLOSED &&
		    state != libtender::domMod::tender::TS_MANUALLY_CLOSED )
		{
			ui.pbNewItem->setEnabled( true );
		}
		
		ui.tblDeliveryItems->setEnabled( true );
		ui.pbAddDelivery->setEnabled( true );

		basar::db::aspect::AccessorPropertyTable_YIterator yitSelectedDelivery = getCurrentScheduledDelivery();
		bool isSelected = (    false == yitSelectedDelivery.isNull() 
							&& false == yitSelectedDelivery.isEnd() );
		
		if( isSelected &&
			isDeliveryModifiable( yitSelectedDelivery ) &&
			libtender::domMod::tender::TS_OK > m_uiContractInfo->getStateIndex()  ) 
		{
			ui.pbModifyDelivery->setEnabled( true );
			ui.pbDeleteDelivery->setEnabled( true );

			if( 1 == ui.tabWidget->currentIndex() )
			{
				ui.pbClone->setEnabled( true );
			}
		}

		ui.pbSave->setEnabled( true );
		ui.pbSaveAndContinue->setEnabled( true );
	}
}

void TenderMaintainVC::tblDeliverySelectionChanged()
{
	OnTblDeliveriesDataSelectionChanged();
}

void TenderMaintainVC::reject()
{
	m_CloneDeliveriesFlag = false;
	QDialog::reject();
}

void TenderMaintainVC::OnSaveClicked()
{
	METHODNAME_DEF( TenderMaintainVC, OnSaveClicked )
	BLOG_TRACE_METHOD( m_Logger, fun );

	if( !m_uiContractInfo->isContractPeriodValid() )
	{
		showWarningMessage( tr("Contract begin date is greater than or equal to contract end date.").toLocal8Bit().constData() );
		return;
	}

	if( 0 == ui.tblItems->rowCount() )
	{
		showWarningMessage( tr("Please add at least one article to your tender before saving.").toLocal8Bit().constData() );
		return;
	}

	libutil::gui::WaitCursor bc( this );
	if( !basar::appl::SystemEventManager::getInstance().fire( "SaveTenderRequested" ).isHandlerOK() )
	{
		return;
	}

	m_AreDeliveriesLoaded = false;
	if( QObject::sender() == ui.pbSave )
	{
		this->close();
	}
}

void TenderMaintainVC::OnCloseClicked()
{
	m_CloneDeliveriesFlag = false;
	reject();
}

void TenderMaintainVC::OnSaveItemClicked()
{
	METHODNAME_DEF( TenderMaintainVC, OnSaveItemClicked )
	BLOG_TRACE_METHOD( m_Logger, fun );

	libutil::gui::WaitCursor bc( this );
	basar::appl::SystemEventManager::getInstance().fire( "SaveTenderPositionRequested" );
}

void TenderMaintainVC::OnSaveAndNewItemClicked()
{
	METHODNAME_DEF( TenderMaintainVC, OnSaveAndNewItemClicked )
	BLOG_TRACE_METHOD( m_Logger, fun );

	basar::appl::SystemEventManager::getInstance().fire( "SaveAndNewTenderPositionRequested" );
}

void TenderMaintainVC::OnCancelModifyItemClicked()
{
	METHODNAME_DEF( TenderMaintainVC, OnCancelModifyItemClicked )
	BLOG_TRACE_METHOD( m_Logger, fun );

	basar::appl::SystemEventManager::getInstance().fire( "CancelModifyTenderPositionRequested" );
	OnTblItemsDataSelectionChanged();
}

void TenderMaintainVC::OnModifyItemClicked()
{
	METHODNAME_DEF(TenderMaintainVC, OnModifyItemClicked)
	BLOG_TRACE_METHOD(m_Logger, fun);

	if(isStateClosed() || !m_uiContractInfo->isFilled())
	{
		return;
	}

	libutil::gui::WaitCursor bc(this);
	basar::appl::SystemEventManager::getInstance().fire("ModifyTenderPositionRequested");
}

void TenderMaintainVC::OnDeleteItemClicked()
{
	METHODNAME_DEF( TenderMaintainVC, OnDeleteItemClicked )
	BLOG_TRACE_METHOD( m_Logger, fun );

	libutil::gui::WaitCursor bc( this );
	basar::appl::SystemEventManager::getInstance().fire( "DeleteTenderPositionRequested" );
}

void TenderMaintainVC::OnNewItemClicked()
{
	resetTenderPosition();
	enableTenderPositionEditing(false);
}

void TenderMaintainVC::OnTblItemsDataSelectionChanged()
{
	if(isStateClosed())
	{
		return;
	}

	bool isSelected = isItemSelected();

    if(m_Area == constants::REGION_CH)
    {
        if(ui.tblItems->rowCount() > 0)
        {
            ui.pbDeleteItem->setEnabled(isSelected);
            ui.pbModifyItem->setEnabled(false);
        }
        else
        {
            ui.pbDeleteItem->setEnabled(false);
            ui.pbModifyItem->setEnabled(false);
        }
    }
    else
    {
		ui.pbDeleteItem->setEnabled(isSelected && m_uiContractInfo->isFilled());
		ui.pbModifyItem->setEnabled(isSelected && m_uiContractInfo->isFilled());
    }

	if(isSelected)
	{
		matchFromTenderPosition(m_TenderPositionMatcher.getCurrentRight());
	}
}

void TenderMaintainVC::OnTblDeliveriesDataSelectionChanged()
{
	libutil::gui::WaitCursor bc(this);
	basar::appl::SystemEventManager::getInstance().fire("DeliverySelectionChangeRequested");

	if(true == m_CloneDeliveriesFlag)
	{
		ui.pbAddDelivery->setDisabled(true);
		ui.pbClone->setDisabled(true);
		ui.pbModifyDelivery->setDisabled(true);
		ui.pbDeleteDelivery->setDisabled(true);
		return;
	}
	else
	{
		if(m_uiContractInfo->isFilled() && !isStateClosed())
		{
			ui.pbAddDelivery->setEnabled(true);
		}
		else
		{
			ui.pbAddDelivery->setDisabled(!ui.pbAddDelivery->isEnabled());
		}
	}

	basar::db::aspect::AccessorPropertyTable_YIterator yitSelectedDelivery = getCurrentScheduledDelivery();

	bool isSelected = (false == yitSelectedDelivery.isNull() && false == yitSelectedDelivery.isEnd());

	if(isSelected && libtender::domMod::tender::TS_OK >= m_uiContractInfo->getStateIndex()) //TODO m_uiContractInfo->getState()
	{
		ui.pbClone->setEnabled(isSelected);
	}
	else
	{
		ui.pbClone->setEnabled(false);
	}

	if(isSelected && 
	   isDeliveryModifiable(yitSelectedDelivery) &&
	   libtender::domMod::tender::TS_OK >= m_uiContractInfo->getStateIndex()) //TODO m_uiContractInfo->getState()
	{
		ui.pbClone->setEnabled(isSelected);
		ui.pbModifyDelivery->setEnabled(isSelected);
		ui.pbDeleteDelivery->setEnabled(isSelected);
	}
	else
	{
		ui.pbClone->setEnabled(false);
		ui.pbModifyDelivery->setEnabled(false);
		ui.pbDeleteDelivery->setEnabled(false);
	}
}
	
void TenderMaintainVC::OnInputCheckCodeQtyValid()
{
    ui.pbAddItem->setEnabled(true);

    if(m_Area == constants::REGION_CH)
    {
        ui.pbAddAndNewItem->setDisabled(true);
    }
    else
    {
		ui.pbAddAndNewItem->setEnabled(true);
	}
}

void TenderMaintainVC::OnInputCheckCodeQtyInvalid()
{
	ui.pbAddItem->setDisabled(true);
	ui.pbAddAndNewItem->setDisabled(true);
}

void TenderMaintainVC::OnStartSearch()
{
	METHODNAME_DEF( TenderMaintainVC, OnStartSearch )
	BLOG_TRACE_METHOD( m_Logger, fun );

    if( m_uiArticleInfo->hasFocus() )
	{
		libutil::gui::WaitCursor bc( this );
		basar::appl::SystemEventManager::getInstance().fire( "SearchArticleRequested" );
	}

	if( m_uiContractInfo->isPartnerInFocus() )
	{
		libutil::gui::WaitCursor bc( this );
		basar::appl::SystemEventManager::getInstance().fire( "SearchPartnerRequested" );
	}
	
	if( m_uiContractInfo->isCustomerInFocus() )
	{
		libutil::gui::WaitCursor bc( this );
		basar::appl::SystemEventManager::getInstance().fire( "SearchCustomerRequested" );
	}

	if( m_uiContractInfo->isPharmacyChainInFocus() )
	{
		libutil::gui::WaitCursor bc( this );
		basar::appl::SystemEventManager::getInstance().fire( "SearchPharmacyChainRequested" );
	}
}

void TenderMaintainVC::OnCheckCustomer()
{
	METHODNAME_DEF( TenderMaintainVC, OnCheckCustomer )
	BLOG_TRACE_METHOD( m_Logger, fun );

	basar::appl::SystemEventManager::getInstance().fire( "CheckCustomerRequested" );
}

void TenderMaintainVC::OnCheckPartner()
{
	METHODNAME_DEF( TenderMaintainVC, OnCheckPartner )
	BLOG_TRACE_METHOD( m_Logger, fun );

	basar::appl::SystemEventManager::getInstance().fire( "CheckPartnerRequested" );
}

void TenderMaintainVC::OnCreateOrdersClicked()
{
    METHODNAME_DEF( TenderMaintainVC, OnCreateOrdersClicked )
    BLOG_TRACE_METHOD( m_Logger, fun );
   
    if( basar::cmnutil::Date::getCurrent().getDate() > m_uiContractInfo->getDateTo().getDate()  )
    {
        displayContractPeriodOutOfDate( "To add a delivery, please adjust the contract period." );
        return;
    }

    bool isHandlerOK = false;
    if( m_Area == constants::REGION_CH )
    {
        isHandlerOK = basar::appl::SystemEventManager::getInstance().fire( "CheckIfTenderWasModifiedRequested" ).isHandlerOK();
    }
    else
    {
        isHandlerOK = basar::appl::SystemEventManager::getInstance().fire( "CheckIfPositionsWereModifiedRequested" ).isHandlerOK();
    }
    if( !isHandlerOK )
    {
        displaySaveTender();
        return;
    }
    
    if( 0 == m_uiContractInfo->getStateIndex() )
    {
        showWarningMessage( tr("Attention! Actual tender state is Capturing data.").toLocal8Bit().constData() );
    }
    libutil::gui::WaitCursor bc( this );
    basar::appl::SystemEventManager::getInstance().fire( "CreateCSCOrdersRequested" );
}

void TenderMaintainVC::OnAddDeliveryClicked()
{
	METHODNAME_DEF( TenderMaintainVC, OnAddDeliveryClicked )
	BLOG_TRACE_METHOD( m_Logger, fun );

	if( basar::cmnutil::Date::getCurrent().getDate() > m_uiContractInfo->getDateTo().getDate()  )
	{
		displayContractPeriodOutOfDate( "To add a delivery, please adjust the contract period." );
		return;
	}

	if( 0 == m_uiContractInfo->getStateIndex() )
	{
		showWarningMessage( tr("Attention! Actual tender state is Capturing data.").toLocal8Bit().constData() );
	}

	libutil::gui::WaitCursor bc( this );
	basar::appl::SystemEventManager::getInstance().fire( "AddDeliveryRequested" );
}

void TenderMaintainVC::OnModifyDeliveryClicked()
{
	METHODNAME_DEF( TenderMaintainVC, OnModifyDeliveryClicked )
	BLOG_TRACE_METHOD( m_Logger, fun );

	if( true == m_CloneDeliveriesFlag )
	{
		return;
	}

	if( !isDeliveryModifiable( getCurrentScheduledDelivery() )  ||
		libtender::domMod::tender::TS_OK < m_uiContractInfo->getStateIndex() )
	{
		return;
	}

	libutil::gui::WaitCursor bc( this );
	basar::appl::SystemEventManager::getInstance().fire( "ModifyDeliveryRequested" );
}

void TenderMaintainVC::OnDeleteDeliveryClicked()
{
	METHODNAME_DEF( TenderMaintainVC, OnDeleteDeliveryClicked )
	BLOG_TRACE_METHOD( m_Logger, fun );

	if( basar::gui::tie::ButtonNo == showQuestionMessage( tr("Do you want to delete this delivery?").toLocal8Bit().constData() ) )
	{
		return;
	}

	libutil::gui::WaitCursor bc( this );
	basar::appl::SystemEventManager::getInstance().fire( "DeleteDeliveryRequested" );
}

void TenderMaintainVC::OnCloneDeliveryClicked()
{
	METHODNAME_DEF( TenderMaintainVC, OnCloneDeliveryClicked )
	BLOG_TRACE_METHOD( m_Logger, fun );

	if( getCurrentScheduledDelivery().isEnd() )
	{
		return;
	}

	libutil::gui::WaitCursor bc( this );
	basar::appl::SystemEventManager::getInstance().fire( "CloneDeliveryRequested" );
}

void TenderMaintainVC::OnTabChanged(int index)
{
	METHODNAME_DEF(TenderMaintainVC, OnTabChanged)
	BLOG_TRACE_METHOD(m_Logger, fun);

	if(ui.tabWidget->indexOf(ui.tbScheduling) == index)
	{
		if(true == m_CloneDeliveriesFlag)
		{
			libutil::gui::WaitCursor bc(this);
			basar::appl::SystemEventManager::getInstance().fire("ScheduleRequested");
			m_AreDeliveriesLoaded = true;
		}
		else if(!basar::appl::SystemEventManager::getInstance().fire("CheckIfPositionsWereModifiedRequested" ).isHandlerOK())
		{
			ui.tabWidget->setCurrentIndex(0);
			displaySaveTender();
			return;
		}
		
		if(false == m_AreDeliveriesLoaded)
		{
			libutil::gui::WaitCursor bc(this);
			basar::appl::SystemEventManager::getInstance().fire("ScheduleRequested");
			m_AreDeliveriesLoaded = true ;
		}
	}
}

} // end namespace viewConn
} // end namespace tender
