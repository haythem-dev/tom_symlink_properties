#include "collectiveorderwishconfirmationvc.h"

#include "libabbauw/loggerpool/libabbauw_loggerpool.h"
#include "libabbauw/libabbauw_properties_definitions.h"

#include "doubletransformer.h"
#include "flagtransformer.h"
#include "mainorderdatetransformer.h"

#include "definitions_gui.h"
#include "waitcursor.h"
#include <libutil/viewconn.h>
#include <QtWidgets/QInputDialog>

namespace CSC_Batches
{
namespace viewConn
{
    CollectiveOrderWishConfirmationVC::CollectiveOrderWishConfirmationVC( QWidget* parent, bool isFiltered )
        : BaseVC( parent ), m_isFiltered( isFiltered ),
        m_EventsWiring ( "CollectiveOrderWishConfirmationVC::EventsWiring" )
    {
        METHODNAME_DEF( CollectiveOrderWishConfirmationVC, CollectiveOrderWishConfirmationVC );
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );
 
        ui.setupUi( this );
        wireEvents();
        initHeaderLabels();
        setupTableWidget();
    }

    CollectiveOrderWishConfirmationVC::~CollectiveOrderWishConfirmationVC()
    {
        METHODNAME_DEF( CollectiveOrderWishConfirmationVC, ~CollectiveOrderWishConfirmationVC )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );
    }

    void CollectiveOrderWishConfirmationVC::setSession( libutil::infrastructure::session::ISessionPtr session )
    {
        m_Session = session;
        labelWindowTitle();
    }

    void CollectiveOrderWishConfirmationVC::wireEvents()
    {
        METHODNAME_DEF( CollectiveOrderWishConfirmationVC, wireEvents ); fun;
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        if( m_EventsWiring.isSet() )
        {
            return;
        }

        connect( ui.twOrderWishes,      SIGNAL( dataSelectionChanged()          ),
	             this,                  SLOT  ( onTableDataSelectionChanged() ) );
        connect( ui.btnRemoveOW,        SIGNAL( clicked()                       ),
                 this,                  SLOT  ( onRemoveOrderWishRequested() )  );
        connect( ui.btnCancel,          SIGNAL( clicked()                       ),
                 this,                  SLOT  ( onCancelOrderWishesRequested() ));
        connect( ui.btnSendOrderWishes, SIGNAL( clicked()                       ),
                 this,                  SLOT  ( onCreateOrderWishesRequested() ));

        m_EventsWiring.set();
    }

    void CollectiveOrderWishConfirmationVC::labelWindowTitle()
    {
        QString windowTitle;
	    windowTitle.append( DLG_TITLESHORT1 ).append( DLG_NO_COLLECTIVEORDERWISHCONFIRM ).append( DLG_TITLESHORT2 );
        QString title;
        if( m_isFiltered )
        {
            title = tr( "Send Filtered Order Wishes" );
        }
        else
        {
            title = tr( "Send Selected Order Wishes" );
        }
        windowTitle.append( title );
        ui.lblHeader->setText( title );

	    windowTitle.append( tr( " area: " ) );
        windowTitle.append( QString::fromLocal8Bit( m_Session->getUser()->getAreaName().c_str() ) );
        setWindowTitle( windowTitle );
    }

    void CollectiveOrderWishConfirmationVC::setupTableWidget()
    {
        ui.twOrderWishes->setSelectionBehavior( QAbstractItemView::SelectRows );
        ui.twOrderWishes->setSelectionMode( QAbstractItemView::SingleSelection );
	    // don't allow to edit data in table
	    ui.twOrderWishes->setEditTriggers( QAbstractItemView::NoEditTriggers );
        ui.twOrderWishes->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );

        int col = 0;
        ui.twOrderWishes->setColumnWidth( col++,  50 ); // MNo
        ui.twOrderWishes->setColumnWidth( col++,  70 ); // Article No.
        //ui.twOrderWishes->setColumnWidth( col++, 230 ); // Article Name
        ui.twOrderWishes->setColumnWidth( col++, 170 ); // Article Name
        ui.twOrderWishes->setColumnWidth( col++,  70 ); // Unit
        ui.twOrderWishes->setColumnWidth( col++,  60 ); // Order No.
        ui.twOrderWishes->setColumnWidth( col++,  35 ); // Pos No.
        ui.twOrderWishes->setColumnWidth( col++,  50 ); // Ordered Qty
        ui.twOrderWishes->setColumnWidth( col++,  68 ); // Order Proposal Qty
        ui.twOrderWishes->setColumnWidth( col++,  90 ); // Next Main Order Date
        ui.twOrderWishes->setColumnWidth( col++,  65 ); // successor
        ui.twOrderWishes->setColumnWidth( col++, 120 ); // Next Main Order Time

        ui.twOrderWishes->horizontalHeader()->setSectionResizeMode( 2, QHeaderView::Stretch ); // use max space for description column

        adjustTableTextAlignment();
    }

    void CollectiveOrderWishConfirmationVC::adjustTableTextAlignment()
    {
        int col = 0;
	    ui.twOrderWishes->setTextAlignmentColumn( col++, Qt::AlignRight   | Qt::AlignVCenter );
        ui.twOrderWishes->setTextAlignmentColumn( col++, Qt::AlignRight   | Qt::AlignVCenter );
        ui.twOrderWishes->setTextAlignmentColumn( col++, Qt::AlignLeft    | Qt::AlignVCenter );
	    ui.twOrderWishes->setTextAlignmentColumn( col++, Qt::AlignRight   | Qt::AlignVCenter );
	    ui.twOrderWishes->setTextAlignmentColumn( col++, Qt::AlignRight   | Qt::AlignVCenter );
        ui.twOrderWishes->setTextAlignmentColumn( col++, Qt::AlignRight   | Qt::AlignVCenter );
	    ui.twOrderWishes->setTextAlignmentColumn( col++, Qt::AlignRight   | Qt::AlignVCenter );
	    ui.twOrderWishes->setTextAlignmentColumn( col++, Qt::AlignRight   | Qt::AlignVCenter );
	    ui.twOrderWishes->setTextAlignmentColumn( col++, Qt::AlignCenter  | Qt::AlignVCenter );
	    ui.twOrderWishes->setTextAlignmentColumn( col++, Qt::AlignRight   | Qt::AlignVCenter );
	    ui.twOrderWishes->setTextAlignmentColumn( col++, Qt::AlignRight   | Qt::AlignVCenter );
    }

    void CollectiveOrderWishConfirmationVC::initHeaderLabels()
    {
        QStringList orderWishHeader;
        
        orderWishHeader.push_back ( tr("MNo")                                   );
        m_ListAttributes.push_back( libabbauw::properties::MANUFACTURERNO       );

        orderWishHeader.push_back ( tr("Article No.")                           );
        m_ListAttributes.push_back( libabbauw::properties::PZN                  );

        orderWishHeader.push_back ( tr("Item Name")                             );
        m_ListAttributes.push_back( libabbauw::properties::ARTICLENAME          );

        orderWishHeader.push_back ( tr("Unit")                                  );
        m_ListAttributes.push_back( libabbauw::properties::UNIT                 );

        orderWishHeader.push_back ( tr("Order No.")                             );
        m_ListAttributes.push_back( libabbauw::properties::ORDERNO              );

        orderWishHeader.push_back ( tr("PNo")                                   );
        m_ListAttributes.push_back( libabbauw::properties::POSNO                );

        orderWishHeader.push_back ( tr("OQTY")                                  );
        m_ListAttributes.push_back( libabbauw::properties::ORDEREDQTY           );

        orderWishHeader.push_back ( tr("Order Proposal Qty")                    );
        m_ListAttributes.push_back( libabbauw::properties::ORDERPROPOSALQTY     );

        orderWishHeader.push_back ( tr("next main order")                       );
        m_ListAttributes.push_back( libabbauw::properties::NEXT_MAIN_ORDER_DATE );

        orderWishHeader.push_back(  tr("successor")                             );
	    m_ListAttributes.push_back( libabbauw::properties::SUCCESSOR            );

        orderWishHeader.push_back ( tr("delivery time")                         );
        m_ListAttributes.push_back( libabbauw::properties::DELIVERY_TIME        );

        ui.twOrderWishes->setColumnCount( static_cast<int>(m_ListAttributes.size()) );
        ui.twOrderWishes->setHorizontalHeaderLabels( orderWishHeader );
    }

    void CollectiveOrderWishConfirmationVC::initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
    {
        m_OrderWishMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twOrderWishes, propTab );
        m_OrderWishMatcher.transformProperties( ui.twOrderWishes->horizontalHeaderLabels(), m_ListAttributes );

        ::viewConn::DoubleTransformer dt( tr("delivery time").toLocal8Bit().constData(), libabbauw::properties::DELIVERY_TIME, 2 );
        FlagTransformer flagTransformer( tr( "successor" ).toLocal8Bit().constData(), libabbauw::properties::SUCCESSOR, ui.twOrderWishes, true );
        CSC_Batches::viewConn::MainOrderDateTransformer modt( libabbauw::properties::NEXT_MAIN_ORDER_DATE, tr("next main order").toLocal8Bit().constData(), 
                                                              ui.twOrderWishes );
        m_OrderWishMatcher.push_back( dt              );
        m_OrderWishMatcher.push_back( flagTransformer );
        m_OrderWishMatcher.push_back( modt            );
    }

    void CollectiveOrderWishConfirmationVC::onTableDataSelectionChanged()
    {
        ui.btnRemoveOW->setEnabled( ! getSelectedOrderWish().isEnd() );
    }

    void CollectiveOrderWishConfirmationVC::onRemoveOrderWishRequested()
    {
        METHODNAME_DEF( CollectiveOrderWishConfirmationVC, onRemoveOrderWishRequested )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        CSC_Batches::gui::WaitCursor bc( this );
        const basar::Int32 selectedRow = ui.twOrderWishes->currentRow();
        basar::appl::SystemEventManager::getInstance().fire( "RemoveOrderWishRequested" );

        // select next item
        if( ui.twOrderWishes->rowCount() > selectedRow )
        {
            ui.twOrderWishes->selectRow( selectedRow );
        }
        else
        {
            ui.twOrderWishes->selectRow( ui.twOrderWishes->rowCount() - 1 );
        }
    }

    void CollectiveOrderWishConfirmationVC::onCreateOrderWishesRequested()
    {
        METHODNAME_DEF( CollectiveOrderWishConfirmationVC, onCreateOrderWishesRequested )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        CSC_Batches::gui::WaitCursor bc( this );
        basar::appl::SystemEventManager::getInstance().fire( "CreateOrderWishesRequested" );
    }

    void CollectiveOrderWishConfirmationVC::onCancelOrderWishesRequested()
    {
        METHODNAME_DEF( CollectiveOrderWishConfirmationVC, onCancelOrderWishesRequested() )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        shutdown();
    }

    basar::gui::tie::WidgetReturnEnum CollectiveOrderWishConfirmationVC::show()
    {
        METHODNAME_DEF( ArticleSearchUC, show )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        return basar::gui::tie::getWidgetReturnType( exec() );
    }

    void CollectiveOrderWishConfirmationVC::hide()
    {
        METHODNAME_DEF( CollectiveOrderWishConfirmationVC, hide )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        QWidget::hide();
    }

    void CollectiveOrderWishConfirmationVC::shutdown()
    {
        METHODNAME_DEF( CollectiveOrderWishConfirmationVC, shutdown )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        QWidget::close();
    }

    void CollectiveOrderWishConfirmationVC::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator )
    {
    }

    void CollectiveOrderWishConfirmationVC::matchFromOrderWishes( basar::db::aspect::AccessorPropertyTableRef propTab )
    {
        METHODNAME_DEF( CollectiveOrderWishConfirmationVC, matchFromOrderWishes )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        initMatcher( propTab );
        libutil::gui::SignalBlocker signalblocker( ui.twOrderWishes );

        ui.twOrderWishes->clearContents();
	    ui.twOrderWishes->setRowCount( static_cast<int>(propTab.size()) );
        basar::Int32 matchedHits = m_OrderWishMatcher.RightToLeft( 0, ui.twOrderWishes->rowCount() );
	    ui.twOrderWishes->setRowCount( matchedHits );
        
        ui.btnSendOrderWishes->setEnabled( matchedHits > 0 );
        ui.lblOrderWishCount->setText( QString::number( propTab.size() ) );
    }

    basar::db::aspect::AccessorPropertyTable_YIterator CollectiveOrderWishConfirmationVC::getSelectedOrderWish()
    {
        return m_OrderWishMatcher.getSelectedRight();
    }

} //namespace viewconn
} //namespace CSC_Batches
