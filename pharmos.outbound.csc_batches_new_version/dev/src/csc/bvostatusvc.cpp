//-------------------------------------------------------------------------------------------------//
/*! \file 
*   \brief ViewConnector for input of Proposal.
*   \author Hendrik Polzin
*   \date
*/
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "cmnvc.h"
#include "bvostatusvc.h"
#include "definitions_gui.h"

#include "libabbauw/libabbauw_properties_definitions.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include <libbasarlogin_manager.h>
#include "doubletransformer.h"
#include "mainorderdatetransformer.h"

#include <libbasarcmnutil_date.h>

//-------------------------------------------------------------------------------------------------//
// using declaration section
//-------------------------------------------------------------------------------------------------//
namespace CSC_Batches
{
    namespace viewConn
    {
        using basar::db::aspect::AccessorPropertyTable_YIterator;
        using basar::db::aspect::AccessorPropertyTableRef;
    }
}

namespace CSC_Batches
{
namespace viewConn
{

BVOStatusVC::BVOStatusVC(QWidget *parent)
: QDialog( parent )
{
    ui.setupUi(this);
    Qt::WindowFlags flags = windowFlags() | Qt::WindowMinMaxButtonsHint;
    setWindowFlags( flags );
    wireEvents();
}

BVOStatusVC::~BVOStatusVC()
{

}

void BVOStatusVC::wireEvents()
{
    METHODNAME_DEF( BVOStatusVC, wireEvents ); fun;
    BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

	connect( ui.btnClose,		SIGNAL( clicked()					),
             this,				SLOT  ( onBtnCloseClicked() )		);
    connect( ui.btnRefresh,		SIGNAL( clicked()					),
             this,				SLOT  ( onBtnRefreshClicked() )		);
	connect( ui.btnCancelOrder, SIGNAL( clicked()	                ),
             this,              SLOT  ( onBtnCancelOrderClicked() ) );
	connect( ui.tblBVOTransfer, SIGNAL( dataSelectionChanged()      ),
             this,			    SLOT  ( onSelectionChanged() )      );
	connect( ui.btnExportCSV,   SIGNAL( clicked()					),
             this,              SLOT  ( onBtnExportCSVClicked() )	);
}


void BVOStatusVC::init( basar::db::aspect::AccessorPropertyTableRef proposalPropTab, basar::Int32 orderno )
{
	init( proposalPropTab );
	setupTitleLabel( orderno );
}


void BVOStatusVC::init( basar::db::aspect::AccessorPropertyTableRef proposalPropTab )
{
    METHODNAME_DEF( BVOStatusVC, init )
    BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

    initProposalDisplayFrame( proposalPropTab );
    setupTableWidgetSorter();
}

void BVOStatusVC::labelWindowTitle()
{
	QString windowTitle;
	windowTitle.append( DLG_TITLESHORT1 ).append( DLG_NO_BVOSTATUS ).append( DLG_TITLESHORT2 );
	windowTitle.append( tr( "Status Orderproposals" ) );
	windowTitle.append( tr( " area: " ) );
	basar::I18nString areaName = basar::login::Manager::getInstance().getAreaName();
	windowTitle.append( QString::fromLocal8Bit( areaName.c_str() ) );
	setWindowTitle( windowTitle );
}

bool BVOStatusVC::shutdown()
{
    m_ProposalMatcher.reset();
    return close();
}

basar::gui::tie::WidgetReturnEnum BVOStatusVC::show()
{
    METHODNAME_DEF( BVOStatusVC, show )
    BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

    QWidget::setWindowModality( Qt::ApplicationModal );
	QDialog::show();
	QApplication::processEvents();

    return basar::gui::tie::WidgetReturnEnum();
}

void BVOStatusVC::hide()
{
    METHODNAME_DEF( BVOStatusVC, fun )
    BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );
    
    QDialog::hide();
}

void BVOStatusVC::waitForEvents()
{
	METHODNAME_DEF( BVOStatusVC, waitForEvents )
    BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

	setAttribute( Qt::WA_Moved, false );
	exec();
}

void BVOStatusVC::adjustTableTextAlignment()
{
	ui.tblBVOTransfer->setTextAlignmentColumn(  0, Qt::AlignRight   | Qt::AlignVCenter );
    ui.tblBVOTransfer->setTextAlignmentColumn(  1, Qt::AlignRight   | Qt::AlignVCenter );
	ui.tblBVOTransfer->setTextAlignmentColumn(  3, Qt::AlignRight   | Qt::AlignVCenter );
	ui.tblBVOTransfer->setTextAlignmentColumn(  4, Qt::AlignRight   | Qt::AlignVCenter );
	ui.tblBVOTransfer->setTextAlignmentColumn(  5, Qt::AlignRight   | Qt::AlignVCenter );
    ui.tblBVOTransfer->setTextAlignmentColumn(  6, Qt::AlignRight   | Qt::AlignVCenter );
	ui.tblBVOTransfer->setTextAlignmentColumn(  9, Qt::AlignRight   | Qt::AlignVCenter );
	ui.tblBVOTransfer->setTextAlignmentColumn( 10, Qt::AlignRight   | Qt::AlignVCenter );
	ui.tblBVOTransfer->setTextAlignmentColumn( 11, Qt::AlignRight   | Qt::AlignVCenter );
    ui.tblBVOTransfer->setTextAlignmentColumn( 12, Qt::AlignCenter  | Qt::AlignVCenter );
    ui.tblBVOTransfer->setTextAlignmentColumn( 13, Qt::AlignCenter  | Qt::AlignVCenter );
}

void BVOStatusVC::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
{
    if( ! m_TableWidgetSorter.getSortOrder().empty() )
    {
        yitSearch.setString( libabbauw::properties::SORT, m_TableWidgetSorter.getSortOrder().toString() );
    }
}

void BVOStatusVC::matchFromProposal()
{
	libutil::gui::SignalBlocker signalblocker( ui.tblBVOTransfer );

    const basar::Int32 bvoCount = static_cast<basar::Int32>(m_ProposalMatcher.getRight().size());
    ui.tblBVOTransfer->setRowCount( bvoCount );
    const basar::Int32 matchedHits = m_ProposalMatcher.RightToLeft(  );
    adjustSizeProposalsDisplayFrame();

    signalblocker.unblock();

    disableProposalsDisplayFrame( 0 >= matchedHits );
    ui.lblBVOCount->setText( QString::number( bvoCount ) );
	adjustTableTextAlignment();
}

basar::db::aspect::AccessorPropertyTable_YIterator BVOStatusVC::getSelectedOrderProposal()
{
	return m_ProposalMatcher.getSelectedRight();
}

void BVOStatusVC::setupTitleLabel()
{
	QString str = tr("Status processing orderproposals (all orders)");
	ui.lblBVOTitle->setText(str);
}

void BVOStatusVC::setupTitleLabel(basar::Int32 orderno)
{
	QString str = tr("Status processing orderproposals (order %1)").arg(orderno);
	ui.lblBVOTitle->setText(str);	
}

void BVOStatusVC::setupTableWidgetSorter()
{
    m_TableWidgetSorter.setTableWidget( ui.tblBVOTransfer );
    m_TableWidgetSorter.setListAttributes( m_ListAttributes );

    m_TableWidgetSorter.addSortableColumn( libabbauw::properties::PURCHASEORDERSUPPLIERNO  );
    m_TableWidgetSorter.addSortableColumn( libabbauw::properties::PZN                      );
    m_TableWidgetSorter.addSortableColumn( libabbauw::properties::ARTICLENAME              );
    m_TableWidgetSorter.addSortableColumn( libabbauw::properties::ORDERNO                  );
    m_TableWidgetSorter.addSortableColumn( libabbauw::properties::ORDERPROPOSALTIME        );
    m_TableWidgetSorter.addSortableColumn( libabbauw::properties::DATE_OF_PURCHASE         );
    m_TableWidgetSorter.addSortableColumn( libabbauw::properties::PURCHASEORDERNO          );
    m_TableWidgetSorter.addSortableColumn( libabbauw::properties::ORDER_PROP_DISCLAIM_NOTE );

    util::SortOrder sortOrder = m_TableWidgetSorter.getSortOrder();
    sortOrder.setOrder( libabbauw::properties::PURCHASEORDERSUPPLIERNO, util::SortOrder::ASCENDING );
    sortOrder.setOrder( libabbauw::properties::PZN,     util::SortOrder::ASCENDING );
    sortOrder.setOrder( libabbauw::properties::ORDERNO, util::SortOrder::ASCENDING );
    m_TableWidgetSorter.setSortOrder( sortOrder );
}

void BVOStatusVC::configureProposalFrame()
{
    METHODNAME_DEF( BVOStatusVC, configureProposalFrame )
    BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );
}

void BVOStatusVC::disableProposalFrame( const bool disabled )
{
    METHODNAME_DEF( BVOStatusVC, disableArticleSearchFrame )
    BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

    ui.btnClose->setDisabled( disabled );
}

void BVOStatusVC::initProposalDisplayFrame( basar::db::aspect::AccessorPropertyTableRef articlePropTab )
{
    METHODNAME_DEF( BVOStatusVC, initProposalDisplayFrame )
    BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

    QStringList lstLables; 
    //std::list<basar::VarString> m_ListAttributes;
    initHeaderLablesProposalsDisplayFrame( lstLables );
    
	labelWindowTitle();
	setupTitleLabel();
    configureProposalsDisplayFrame( lstLables );
    clearProposalsDisplayFrame();
    adjustSizeProposalsDisplayFrame();
    disableProposalsDisplayFrame( true );
    initMatcherProposalsDisplayFrame( articlePropTab );
	ui.btnCancelOrder->setDisabled( true );
}

void BVOStatusVC::initHeaderLablesProposalsDisplayFrame( QStringList& lstLables )
{
    METHODNAME_DEF( BVOStatusVC, initHeaderLablesProposalDisplayFrame )
    BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

	using namespace libabbauw::properties;

    //- purchordersupplno - BESLA-NR
	//- articleno 		- Artikelnummer
	//- orderproposalqty 	- AMGE 	- Angeforderte Menge 
	//- ordernobatch 		– ANR 		– Batch-Auftragsnummer
	//- posnobatch 		– POS 		– Batch-Positionsnummer
	//- reasonid 		– Nachbearbeitungsgrund als Text
	//- orderproposaltime 	– BVO-Datum und Uhrzeit - Auslösezeitpunkt des Bestellwunsches in ABBA
	//- transfertime 		–Transfer EK 	– Zeitpunkt der Übertragung in das EK-System
	//- orderproposalprocessedtime – Verarbeitung EK – Verarbeitungsbestätigung aus EK
	//- orderproposaldisclaimnote	- ggf. Text mit Ablehnungsgrund
	//- orerproposaldisclaimnote 	- Ablehnungsgrund des EK falls nicht bestellt wurde
	//- purchaseorderno 	– EK-Bestellnr 	– Bestellnummer für Hersteller
	//- dateofpurchase 	– Ob und wann die Bestellung an den Hersteller herausging

    lstLables.push_back(        tr("purchordersupplno")                    );
    m_ListAttributes.push_back( PURCHASEORDERSUPPLIERNO.getName()          );

    lstLables.push_back(        tr("articleno")                            );
    m_ListAttributes.push_back( PZN.getName()                              );

	lstLables.push_back(        tr("article_name")                         );
    m_ListAttributes.push_back( ARTICLENAME.getName()                      );

	lstLables.push_back(        tr("unit")                                 );
    m_ListAttributes.push_back( UNIT.getName()                             );

	lstLables.push_back(        tr("ordernobatch")                         );
    m_ListAttributes.push_back( ORDERNO.getName()                          );

	lstLables.push_back(        tr("posnobatch")                           );
    m_ListAttributes.push_back( POSNO.getName()                            );

    lstLables.push_back(        tr("orderproposalqty")                     );
    m_ListAttributes.push_back( ORDERPROPOSALQTY.getName()                 );

	lstLables.push_back(        tr("orderproposaltime")                    );
    m_ListAttributes.push_back( ORDERPROPOSALTIME.getName()                );
   
	lstLables.push_back(        tr("transfertime")                         );
    m_ListAttributes.push_back( TRANSFER_TIME.getName()                    );

	lstLables.push_back(        tr("orderproposalprocessedtime")           );
    m_ListAttributes.push_back( ORDER_PROP_PROCESSED_TIME.getName()        );

	lstLables.push_back(        tr("dateofpurchase")                       );
    m_ListAttributes.push_back( DATE_OF_PURCHASE.getName()                 );

	lstLables.push_back(        tr("purchaseorderno")                      );
    m_ListAttributes.push_back( PURCHASEORDERNO.getName()                  );

    lstLables.push_back(        tr("next main order")                      );
    m_ListAttributes.push_back( NEXT_MAIN_ORDER_DATE.getName()             );

    lstLables.push_back(        tr("delivery time")                        );
    m_ListAttributes.push_back( DELIVERY_TIME.getName()                    );

    lstLables.push_back(        tr("expectedgidate")                       );
    m_ListAttributes.push_back( EXPECTED_GI_DATE.getName()                 );

	lstLables.push_back(        tr("orderproposaldisclaimnote")            );
    m_ListAttributes.push_back( ORDER_PROP_DISCLAIM_NOTE.getName()         );

	lstLables.push_back(        tr("reasonid")                             );
	m_ListAttributes.push_back( REASON_ID.getName()                        );
}

void BVOStatusVC::configureProposalsDisplayFrame( QStringList& lstLables )
{
    ui.tblBVOTransfer->setColumnCount              ( lstLables.size()                     );
    ui.tblBVOTransfer->setHorizontalHeaderLabels   ( lstLables                            );
    ui.tblBVOTransfer->setResizeColumnsToContents  ( true                                 );
    ui.tblBVOTransfer->setVerticalScrollBarPolicy  ( Qt::ScrollBarAlwaysOn                );
    ui.tblBVOTransfer->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded                );
    ui.tblBVOTransfer->setSelectionBehavior        ( QAbstractItemView::SelectRows        );
    ui.tblBVOTransfer->setSelectionMode            ( QAbstractItemView::SingleSelection   );
    ui.tblBVOTransfer->hideVerticalHeader          ( true                                 );
    
	// don't allow to edit data in table
	ui.tblBVOTransfer->setEditTriggers( QAbstractItemView::NoEditTriggers );
}

void BVOStatusVC::clearProposalsDisplayFrame()
{
	libutil::gui::SignalBlocker signalblocker( ui.tblBVOTransfer );
    ui.tblBVOTransfer->clearContext();
    disableProposalsDisplayFrame( true );
}

void BVOStatusVC::adjustSizeProposalsDisplayFrame()
{
    libutil::gui::SignalBlocker signalblocker( ui.tblBVOTransfer );
    
    ui.tblBVOTransfer->resizeColumnsToContents();
    ui.tblBVOTransfer->resizeRowsToContents();
}

void BVOStatusVC::disableProposalsDisplayFrame( const bool disable )
{
    libutil::gui::SignalBlocker signalblocker( ui.tblBVOTransfer );
    ui.tblBVOTransfer->setDisabled( disable );
}
void BVOStatusVC::initMatcherProposalsDisplayFrame( basar::db::aspect::AccessorPropertyTableRef articlePropTab )
{   
	using namespace libabbauw::properties;
    m_ProposalMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.tblBVOTransfer, articlePropTab );
	
	basar::gui::tie::DateTimeTransformer dtt( ORDERPROPOSALTIME, tr("orderproposaltime").toLocal8Bit().constData(), "dd.MM.yyyy hh:mm" );
	basar::gui::tie::DateTimeTransformer dtt2( ORDER_PROP_PROCESSED_TIME, tr("orderproposalprocessedtime").toLocal8Bit().constData(), 
                                               "dd.MM.yyyy hh:mm" );
	basar::gui::tie::DateTimeTransformer dtt3( TRANSFER_TIME, tr("transfertime").toLocal8Bit().constData(), "dd.MM.yyyy hh:mm" );
    ::viewConn::DoubleTransformer dt( tr("delivery time").toLocal8Bit().constData(), DELIVERY_TIME, 2 );
    CSC_Batches::viewConn::MainOrderDateTransformer modt( NEXT_MAIN_ORDER_DATE, tr("next main order").toLocal8Bit().constData(), 
                                                          ui.tblBVOTransfer );
    
	m_ProposalMatcher.transformProperties( ui.tblBVOTransfer->horizontalHeaderLabels(), m_ListAttributes );
	m_ProposalMatcher.push_back( dtt );
	m_ProposalMatcher.push_back( dtt2 );
	m_ProposalMatcher.push_back( dtt3 );
    m_ProposalMatcher.push_back( dt   );
    m_ProposalMatcher.push_back( modt );
}

void BVOStatusVC::onBtnCloseClicked()
{
    METHODNAME_DEF( BVOStatusVC, onBtnCloseClicked );
    BLOG_TRACE_METHOD(  libabbauw::LoggerPool::loggerViewConn, fun );

    shutdown();
}

void BVOStatusVC::onBtnRefreshClicked()
{
    CSC_Batches::gui::WaitCursor waitCursor( this );
    basar::appl::SystemEventManager::getInstance().fire( "FindOrderProposalsRequested" );
}

void BVOStatusVC::onBtnCancelOrderClicked()
{
    gui::WaitCursor waitCursor( this );
    basar::appl::SystemEventManager::getInstance().fire( "CancelOrderProposal" );
	onSelectionChanged();
}

void BVOStatusVC::onBtnExportCSVClicked()
{
	gui::WaitCursor waitCursor(this);
    basar::appl::SystemEventManager::getInstance().fire("ExportCSVRequested");
}

void BVOStatusVC::onSelectionChanged()
{
	ui.btnCancelOrder->setEnabled( ! getSelectedOrderProposal().isEnd() );
}

void BVOStatusVC::info( const basar::I18nString& text )
{
	basar::gui::tie::infoMsgBox( this, text.c_str(), tr( "Info" ).toLocal8Bit().constData() );
}

void BVOStatusVC::error( const basar::I18nString& text)
{
	basar::gui::tie::criticalMsgBox( this, text.c_str(), tr("Error").toLocal8Bit().constData() );
}

const basar::gui::tie::MessageBoxButtonEnum BVOStatusVC::question( const basar::I18nString& text)
{
	return basar::gui::tie::questionMsgBox( this, text.c_str(), tr("Question").toLocal8Bit().constData() );
}

QWidget* BVOStatusVC::getWidget()
{
	return this;
}

} // namespace viewConn
} // namespace CSC_Batches
