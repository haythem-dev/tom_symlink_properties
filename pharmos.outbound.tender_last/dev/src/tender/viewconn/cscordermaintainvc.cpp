#include "cscordermaintainvc.h"
#include <libutil/util.h>
#include <libutil/viewconn.h>

#include "loggerpool/tenderloggerpool.h"

#include <libtender/domainmodule/scheduleddelivery/ischeduleddelivery.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryheader.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliverypositioncollection.h>
#include <libtender/domainmodule/tender/itender.h>
#include <libtender/domainmodule/tender/itenderpositioncollection.h>
#include "libtender/domainmodule/libtender_properties_definitions.h"

#include "qvalidatoritemdelegate.h"
#include "checkboxtabletransformer.h"

namespace tender
{
namespace viewConn
{
CSCOrderMaintainVC::CSCOrderMaintainVC( QWidget* parent )
: ICSCOrderMaintain( parent ),
  m_EventsWiring( "CSCOrderMaintainVC::EventsWiring" )
{
    ui.setupUi( this );
    wireEvents();
    m_ColumnColour = QColor::fromRgb( 135, 206, 250 );
    initTableHeaderLabels();
    setupTableWidgets();
}

CSCOrderMaintainVC::~CSCOrderMaintainVC()
{
}

void CSCOrderMaintainVC::wireEvents()
{
	if( m_EventsWiring.isSet() )
	{
		return;
	}

	connect( ui.btnCreateCSCOrder,  SIGNAL( clicked() ),
			 this,					SLOT( onCreateCSCOrderRequested() ) );

	connect( ui.btnCancel,  SIGNAL( clicked() ),
			 this,			SLOT  ( onDialogCloseRequested() ) );

	connect( ui.twTenderPositions,  SIGNAL( activated( const QModelIndex & ) ),
			 this,					SLOT( onSelectItemRequested() ) );

	connect( ui.twOrderPositions,   SIGNAL( cellChanged( int, int ) ),
			 this,					SLOT( onSelectedItemsCellChanged( int, int ) ) );

	m_EventsWiring.set();
}

void CSCOrderMaintainVC::setupTableWidgets()
{
	METHODNAME_DEF( CSCOrderMaintainVC, setupTableWidgets )
	BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

	ui.twTenderPositions->setABCVerticalHeaderPolicy();
	ui.twTenderPositions->setColumnCount              ( m_LstLabelsTenderPositions.size()   );
	ui.twTenderPositions->setHorizontalHeaderLabels   ( m_LstLabelsTenderPositions          );
	ui.twTenderPositions->setResizeRowsToContents     ( true                               );
	ui.twTenderPositions->setResizeColumnsToContents  ( true                               );
	ui.twTenderPositions->setVerticalScrollBarPolicy  ( Qt::ScrollBarAsNeeded              );
	ui.twTenderPositions->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded              );
	ui.twTenderPositions->setSelectionBehavior        ( QAbstractItemView::SelectRows      );
	ui.twTenderPositions->setSelectionMode            ( QAbstractItemView::SingleSelection );
	ui.twTenderPositions->setSortingEnabled(false);
	ui.twTenderPositions->setEnabled(true);

	ui.twOrderPositions->setABCVerticalHeaderPolicy();
	ui.twOrderPositions->setColumnCount              ( m_LstLabelsOrderPositions.size()    );
	ui.twOrderPositions->setHorizontalHeaderLabels   ( m_LstLabelsOrderPositions           );
	ui.twOrderPositions->setResizeRowsToContents     ( true                               );
	ui.twOrderPositions->setResizeColumnsToContents  ( true                               );
	ui.twOrderPositions->setVerticalScrollBarPolicy  ( Qt::ScrollBarAsNeeded              );
	ui.twOrderPositions->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded              );
	ui.twOrderPositions->setSelectionBehavior        ( QAbstractItemView::SelectRows      );
	ui.twOrderPositions->setSelectionMode            ( QAbstractItemView::SingleSelection );
	ui.twOrderPositions->setEditableColumn           ( 0, true                            );
	ui.twOrderPositions->setEditableColumn           ( 1, false                           );
	ui.twOrderPositions->setEditableColumn           ( 2, false                           );
	ui.twOrderPositions->setEditableColumn           ( 3, false                           );
	ui.twOrderPositions->setEditableColumn           ( 4, false                           );
	ui.twOrderPositions->setBackgroundColorColumn    ( 1, m_ColumnColour                  );
	ui.twOrderPositions->setBackgroundColorColumn    ( 2, m_ColumnColour                  );
	ui.twOrderPositions->setBackgroundColorColumn    ( 3, m_ColumnColour                  );
	ui.twOrderPositions->setBackgroundColorColumn    ( 4, m_ColumnColour                  );
	ui.twOrderPositions->setSortingEnabled(false);
	ui.twOrderPositions->setEnabled(true);

	const QRegExp rx("[0-9]+[0-9]*");
	QValidator *validator = new QRegExpValidator(rx, this);

	QValidatorItemDelegate* delegate = new QValidatorItemDelegate(this); 
	delegate->setColumnValidator( 0, validator ); 
	ui.twOrderPositions->setItemDelegate(delegate);
}

basar::Int32 CSCOrderMaintainVC::initTenderPositionsMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
{
	METHODNAME_DEF( TenderMaintainVC, initTenderPositionsMatcher )
	BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

	m_TenderPositionsMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twTenderPositions, propTab );
	m_TenderPositionsMatcher.transformProperties( ui.twTenderPositions->horizontalHeaderLabels(), m_LstOriginalAttributesTenderPositions );

	basar::Int32 count = 0;
	basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();
	while( !yit.isEnd() )
	{
		++count;
		++yit;
	}
	return count;
}

basar::Int32 CSCOrderMaintainVC::initOrderPositionsMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
{
	METHODNAME_DEF( TenderMaintainVC, initOrderPositionsMatcher )
	BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

	m_OrderPositionsMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twOrderPositions, propTab );
	m_OrderPositionsMatcher.setMatchingPolicy(
		basar::gui::tie::INSERT | 
		basar::gui::tie::SKIP |
		basar::gui::tie::CONVERTTYPE |
		basar::gui::tie::DISREGARD_DELETED 
			);
	m_OrderPositionsMatcher.transformProperties( ui.twOrderPositions->horizontalHeaderLabels(), m_LstOriginalAttributesOrderPositions );

	basar::Int32 count = 0;
	basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin();
	while( !yit.isEnd() )
	{
		if( yit.getState( properties::ARTICLE_CODE ) != basar::SS_DELETE )
		{
			++count;
		}
		++yit;
	}
	return count;
}

void CSCOrderMaintainVC::initTableHeaderLabels()
{
	METHODNAME_DEF( CSCOrderMaintainVC, initTableHeaderLabels )
	BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

	m_LstLabelsTenderPositions.push_back( tr( "Article Code" ) );
	m_LstOriginalAttributesTenderPositions.push_back( properties::ARTICLE_CODE );

	m_LstLabelsTenderPositions.push_back( tr( "Article Name" ) );
	m_LstOriginalAttributesTenderPositions.push_back( properties::ARTICLE_NAME );

	m_LstLabelsTenderPositions.push_back( tr( "Available quantity" ) );
	m_LstOriginalAttributesTenderPositions.push_back( properties::REMAINING_QUANTITY );

	m_LstLabelsTenderPositions.push_back( tr( "Allocated quantity" ) );
	m_LstOriginalAttributesTenderPositions.push_back( properties::QUANTITY_ALLOCATED );

	//----------------------------------------------------------------------------

	m_LstLabelsOrderPositions.push_back( tr( "Quantity" ) );
	m_LstOriginalAttributesOrderPositions.push_back( properties::QUANTITY );

	m_LstLabelsOrderPositions.push_back( tr( "Customer No" ) );
	m_LstOriginalAttributesOrderPositions.push_back( properties::CUSTOMER_NO );

	m_LstLabelsOrderPositions.push_back( tr( "Customer Name" ) );
	m_LstOriginalAttributesOrderPositions.push_back( properties::PHARMACY_NAME );

	m_LstLabelsOrderPositions.push_back( tr( "City" ) );
	m_LstOriginalAttributesOrderPositions.push_back( properties::PHARMACY_LOCATION );

	m_LstLabelsOrderPositions.push_back( tr( "Branch No" ) );
	m_LstOriginalAttributesOrderPositions.push_back( properties::BRANCH_NO );
}

basar::db::aspect::AccessorPropertyTable_YIterator CSCOrderMaintainVC::getCurrentTenderPosition( basar::db::aspect::AccessorPropertyTableRef positionPropTab )
{
	m_TenderPositionsMatcher.LeftToRight();
	return m_TenderPositionsMatcher.getSelectedRight();
}

basar::db::aspect::AccessorPropertyTable_YIterator CSCOrderMaintainVC::getCurrentOrderPosition( basar::db::aspect::AccessorPropertyTableRef positionPropTab )
{
	m_OrderPositionsMatcher.LeftToRight();
	return m_OrderPositionsMatcher.getSelectedRight();
}

basar::gui::tie::WidgetReturnEnum CSCOrderMaintainVC::show()
{
	METHODNAME_DEF( CSCOrderMaintainVC, show )
	BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

	if( ui.twTenderPositions->rowCount() > 0 )
	{
		ui.twTenderPositions->selectRow( 0 );
		onSelectItemRequested();
	}
	ui.twTenderPositions->setFocus();
            
	return basar::gui::tie::getWidgetReturnType( exec() );
}

void CSCOrderMaintainVC::matchFromTender( libtender::domMod::tender::ITenderPtr tender )
{
	METHODNAME_DEF( CSCOrderMaintainVC, matchFromTender )
	BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

	basar::Int32 rowCount = initTenderPositionsMatcher( tender->getPositionCollection()->get() );
	libutil::gui::SignalBlocker signalblocker( ui.twTenderPositions );
	ui.twTenderPositions->setRowCount( rowCount );
	m_TenderPositionsMatcher.RightToLeft( 0, ui.twTenderPositions->rowCount() );
	ui.twTenderPositions->setResizeColumnsToContents( true );
	signalblocker.unblock();
}

void CSCOrderMaintainVC::matchFromOrderPositions( basar::db::aspect::AccessorPropertyTableRef positionPropTab )
{
	METHODNAME_DEF( CSCOrderMaintainVC, matchFromOrderPositions )
	BLOG_TRACE_METHOD( libtender::LoggerPool::getLoggerViewConn(), fun );

	basar::Int32 rowCount = initOrderPositionsMatcher( positionPropTab );

	libutil::gui::SignalBlocker signalblocker( ui.twOrderPositions );
	ui.twOrderPositions->clearContext();
	ui.twOrderPositions->setRowCount(rowCount);
	m_OrderPositionsMatcher.RightToLeft();
	ui.twOrderPositions->setResizeColumnsToContents(true);
	signalblocker.unblock();
}

void CSCOrderMaintainVC::onCreateCSCOrderRequested()
{
	METHODNAME_DEF( CSCOrderMaintainVC, onCreateCSCOrderRequested )
	BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );
            
	if( !isSavable() ) return;
            
	libutil::gui::WaitCursor bc( this );
	basar::appl::SystemEventManager::getInstance().fire( "CreateCSCOrderRequested" );
            
	this->close();
}

void CSCOrderMaintainVC::onDialogCloseRequested()
{
	METHODNAME_DEF( CSCOrderMaintainVC, onDialogCloseRequested )
	BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

	this->close();
}

bool CSCOrderMaintainVC::isArticleSelected()
{
	METHODNAME_DEF( CSCOrderMaintainVC, isArticleSelected )
	BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

	if( ui.twOrderPositions->findItems( ui.twTenderPositions->item( ui.twTenderPositions->currentItem()->row(), 0 )->text(), Qt::MatchExactly ).isEmpty() )
	{
		return false;
	}

	return true;
}

bool CSCOrderMaintainVC::isArticleSelected( int row )
{
	METHODNAME_DEF( CSCOrderMaintainVC, isArticleSelected )
	BLOG_TRACE_METHOD(libtender::LoggerPool::getLoggerViewConn(), fun );

	if( ui.twOrderPositions->findItems( ui.twTenderPositions->item( row, 0 )->text(), Qt::MatchExactly ).isEmpty() )
	{
		return false;
	}

	return true;
}

bool CSCOrderMaintainVC::isSavable()
{
	if( 0 == ui.twOrderPositions->rowCount() )
	{
		displayNoArticlesSelected();
		return false;
	}
            
	if( !areQuantitiesSet() )
	{
		displayQuantitiesNotSet();
		return false;
	}
	return true;
}

bool CSCOrderMaintainVC::areQuantitiesSet()
{
	int columnQuantity = 0; // Setting Qunatity column index

	for( int row = 0; ui.twOrderPositions->rowCount() > row; ++row )
	{
		if( ui.twOrderPositions->item( row, columnQuantity  ) && ui.twOrderPositions->item( row, columnQuantity )->text().toInt() > 0)
		{
			return true;
		}
	}
            
	return false;
}

void CSCOrderMaintainVC::displayArticleAlreadySelected()
{
	showWarningMessage( tr("Article is already selected.").toLocal8Bit().constData() );
}

void CSCOrderMaintainVC::displayQuantityExceedance( int maxQuantity )
{
	std::stringstream ss;
	ss << tr( "Quantity for this article exceeded. \nAvailable quantity is" ).toLocal8Bit().constData() << " " << maxQuantity << ".";
	ui.twOrderPositions->clearFocus();
	showWarningMessage( ss.str().c_str() );
}

void CSCOrderMaintainVC::displayNoArticlesSelected()
{
	showWarningMessage( tr( "At least one article needs to be selected before saving the delivery." ).toLocal8Bit().constData() );
}

void CSCOrderMaintainVC::displayQuantitiesNotSet()
{
	showWarningMessage( tr( "Quantities are not set." ).toLocal8Bit().constData() );
}

void CSCOrderMaintainVC::displayPositionHasNoAvailableQuantity()
{
	showInfoMessage( tr( "Article has no available quantity." ).toLocal8Bit().constData() );
}

void CSCOrderMaintainVC::displayCscOrderCreationDetails( const std::string orderInfo )
{
	showInfoMessage( orderInfo.c_str() );
}

void CSCOrderMaintainVC::onSelectItemRequested()
{
	if( isArticleSelected() )
	{
		displayArticleAlreadySelected();
		return;
	}

	basar::appl::SystemEventManager::getInstance().fire( "SelectPositionRequested" );
}

void CSCOrderMaintainVC::onSelectedItemsCellChanged( int row, int column )
{
	//Check if Quantity has exceeded.
	if( COLUMN_QUANTITY == column && row >= 0)
	{
		basar::appl::SystemEventManager::getInstance().fire( "ChangeQuantityRequested" );
	}
}

} //end namespace viewConn
} //end namespace tender
