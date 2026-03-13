#include "batchorderpositionvc.h"
#include "definitions_gui.h"

#include <libabbauw/loggerpool/libabbauw_loggerpool.h>
#include <libabbauw/libabbauw_properties_definitions.h>

#include <libutil/viewconn.h>

#pragma warning(disable: 4505 ) // caused by new Qt connect syntax

namespace CSC_Batches
{
namespace viewConn
{
    BatchOrderPositionVC::BatchOrderPositionVC( QWidget* parent )
    : BaseVC( parent ),
        m_EventsWiring ( "BatchOrderPositionVC::EventsWiring" )
    {
        METHODNAME_DEF( BatchOrderPositionVC, BatchOrderPositionVC );
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );
 
        ui.setupUi( this );
        wireEvents();
        initHeaderLabels();
        setupOrderFrame();
        setupTableWidget();
    }

    BatchOrderPositionVC::~BatchOrderPositionVC()
    {
        METHODNAME_DEF( BatchOrderPositionVC, ~BatchOrderPositionVC )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );
    }

    void BatchOrderPositionVC::setSession( libutil::infrastructure::session::ISessionPtr session )
    {
        m_Session = session;
    }

    libutil::infrastructure::session::ISessionPtr BatchOrderPositionVC::getSession()
    {
        CHECK_INSTANCE_EXCEPTION( m_Session.get() );
        return m_Session;
    }

    void BatchOrderPositionVC::labelWindowTitle()
    {
	    QString windowTitle;
        windowTitle.append( DLG_TITLESHORT1 ).append( DLG_NO_BATCHORDERPOS ).append( DLG_TITLESHORT2 );
	    windowTitle.append( tr( "Batch Order Positions" ) );
	    windowTitle.append( tr( " area: " ) );
        windowTitle.append( QString::fromLocal8Bit( getSession()->getUser()->getAreaName().c_str() ) );
        setWindowTitle( windowTitle );
    }

    void BatchOrderPositionVC::wireEvents()
    {
        METHODNAME_DEF( BatchOrderPositionVC, wireEvents ); fun;
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        if( m_EventsWiring.isSet() )
        {
            return;
        }

        connect( ui.btnClose, &QPushButton::clicked, this, &BatchOrderPositionVC::onCloseClicked );

        m_EventsWiring.set();
    }

    void BatchOrderPositionVC::initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
    {
        m_OrderPosMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twBatchOrderPositions, propTab );
        m_OrderPosMatcher.transformProperties( ui.twBatchOrderPositions->horizontalHeaderLabels(), m_ListAttributes );
        
        basar::gui::tie::DecimalTransformer decimalTransformer( libabbauw::properties::ARTICLEPRICE, tr("Article Price").toLocal8Bit().constData(), 2 );
        m_OrderPosMatcher.push_back( decimalTransformer );
    }

    void BatchOrderPositionVC::setupOrderFrame()
    {
        ui.lblEdiDeliveryDate->setAccessibleName( libabbauw::properties::EDIDELIVERYDATE.getName().c_str() );
        ui.lblHeaderText->setAccessibleName     ( libabbauw::properties::HEADERTEXT.getName().c_str()      );
        ui.lblOrderNo->setAccessibleName        ( libabbauw::properties::ORDERNO.getName().c_str()         );
        ui.lblOrderType->setAccessibleName      ( libabbauw::properties::ORDERTYPE.getName().c_str()       );
        ui.lblOrderValue->setAccessibleName     ( libabbauw::properties::ORDERVALUE.getName().c_str()      );
        ui.lblPharmacyName->setAccessibleName   ( libabbauw::properties::PHARMACYNAME.getName().c_str()    );
        ui.lblPharmacyNo->setAccessibleName     ( libabbauw::properties::PHARMACYNO.getName().c_str()      );
        ui.lblQuickDialNo->setAccessibleName    ( libabbauw::properties::SPEED_DIAL_NO.getName().c_str()   );
        ui.lblTel->setAccessibleName            ( libabbauw::properties::PHONENO.getName().c_str()         );
    }

    void BatchOrderPositionVC::setupTableWidget()
    {
        ui.twBatchOrderPositions->setABCVerticalHeaderPolicy();
	    ui.twBatchOrderPositions->setSelectionBehavior( QAbstractItemView::SelectRows );
	    // don't allow to edit data in table
	    ui.twBatchOrderPositions->setEditTriggers( QAbstractItemView::NoEditTriggers );
	
        ui.lblPositionsFound->setVisible( false );
	    ui.lblPositionCount->setVisible( false );

        ui.twBatchOrderPositions->setTextAlignmentColumn(  0, Qt::AlignRight   | Qt::AlignVCenter ); // POSNO
        ui.twBatchOrderPositions->setTextAlignmentColumn(  1, Qt::AlignRight   | Qt::AlignVCenter ); // PZN
        ui.twBatchOrderPositions->setTextAlignmentColumn(  2, Qt::AlignRight   | Qt::AlignVCenter ); // ORDEREDQTY
        ui.twBatchOrderPositions->setTextAlignmentColumn(  3, Qt::AlignRight   | Qt::AlignVCenter ); // UNIT
        ui.twBatchOrderPositions->setTextAlignmentColumn(  5, Qt::AlignRight   | Qt::AlignVCenter ); // ARTICLEPRICE
    }

    void BatchOrderPositionVC::initHeaderLabels()
    {
        QStringList orderPosHeader;
        
        orderPosHeader.push_back  ( tr("PNo")                                 );
	    m_ListAttributes.push_back( libabbauw::properties::POSNO              );

        orderPosHeader.push_back  ( tr("PZN")                                 );
	    m_ListAttributes.push_back( libabbauw::properties::PZN                );

        orderPosHeader.push_back  ( tr("OQTY")                                );
	    m_ListAttributes.push_back( libabbauw::properties::ORDEREDQTY         );

        orderPosHeader.push_back  ( tr("Unit")                                );
	    m_ListAttributes.push_back( libabbauw::properties::UNIT               );

        orderPosHeader.push_back  ( tr("Item Name")                           );
	    m_ListAttributes.push_back( libabbauw::properties::ARTICLENAME        );

        orderPosHeader.push_back  ( tr("Article Price")                       );
	    m_ListAttributes.push_back( libabbauw::properties::ARTICLEPRICE       );

        orderPosHeader.push_back  ( tr("PF")                                  );
	    m_ListAttributes.push_back( libabbauw::properties::PHARMACEUTICALFORM );

        orderPosHeader.push_back  ( tr("Exp")                                 );
	    m_ListAttributes.push_back( libabbauw::properties::EXPIRYDATE         );

        ui.twBatchOrderPositions->setColumnCount( static_cast<int>(m_ListAttributes.size()) );
        ui.twBatchOrderPositions->setHorizontalHeaderLabels( orderPosHeader );
    }

    void BatchOrderPositionVC::clearTableWidget()
    {
	    ui.twBatchOrderPositions->clearContents();
	    ui.twBatchOrderPositions->setRowCount( 0 );
    }

    void BatchOrderPositionVC::onCloseClicked()
    {
        METHODNAME_DEF( BatchOrderPositionVC, onCloseClicked )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        close();
    }

    basar::gui::tie::WidgetReturnEnum BatchOrderPositionVC::show()
    {
        METHODNAME_DEF( BatchOrderPositionVC, show )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        labelWindowTitle();
        setAttribute( Qt::WA_Moved, false );

        QWidget::setWindowModality( Qt::ApplicationModal );
        return basar::gui::tie::getWidgetReturnType( exec() );
    }

    void BatchOrderPositionVC::hide()
    {
        METHODNAME_DEF( BatchOrderPositionVC, fun )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        QWidget::hide();
    }

    /*
    void BatchOrderPositionVC::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
    {
        basar::gui::tie::MatcherRef orderSearchMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.grpFilter, 
                                                                                                                yitSearch.getPropertyTable() );
	    orderSearchMatcher.LeftToRight( );
    }
    */

    void BatchOrderPositionVC::matchFromBatchOrderHead( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
        METHODNAME_DEF( BatchOrderPositionVC, matchFromBatchOrderHead )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        basar::gui::tie::MatcherRef orderMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.grpOrder, 
                                                                                                          yit.getPropertyTable() );

        basar::gui::tie::DecimalTransformer decimalTransformer( libabbauw::properties::ORDERVALUE, libabbauw::properties::ORDERVALUE.getName(), 2 );
        orderMatcher.push_back( decimalTransformer );
        orderMatcher.RightToLeft();
    }

    void BatchOrderPositionVC::matchFromBatchOrderPositions( basar::db::aspect::AccessorPropertyTableRef propTab )
    {
        METHODNAME_DEF( BatchOrderPositionVC, matchFromBatchOrderPositions )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        initMatcher( propTab );

        libutil::gui::SignalBlocker signalblocker( ui.twBatchOrderPositions );

        ui.twBatchOrderPositions->clearContents();
	    ui.twBatchOrderPositions->setRowCount( 20 );
        basar::Int32 matchedHits = m_OrderPosMatcher.RightToLeft( 0, ui.twBatchOrderPositions->rowCount() );
	    ui.twBatchOrderPositions->setRowCount( matchedHits );

        if( matchedHits > 0 )
        {
            ui.twBatchOrderPositions->selectRow( 0 );
            ui.twBatchOrderPositions->setFocus();

            size_t count = m_OrderPosMatcher.getCurrentRight().getPropertyTable().size();
            ui.lblPositionCount->setText( QString("%1").arg( static_cast<basar::Int32>(count) ) );
        }
        ui.lblPositionCount->setVisible( matchedHits > 0 );
	    ui.lblPositionsFound->setVisible( matchedHits > 0 );

        ui.twBatchOrderPositions->resizeColumnsToContents();
        ui.twBatchOrderPositions->resizeRowsToContents();
        ui.twBatchOrderPositions->horizontalHeader()->setSectionResizeMode( 4, QHeaderView::Stretch );
    }

    void BatchOrderPositionVC::matchFromCustomer( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
        basar::gui::tie::MatcherRef customerMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.grpOrder, 
                                                                                                             yit.getPropertyTable() );

        customerMatcher.RightToLeft();
    }

    void BatchOrderPositionVC::showInfoMessage( const basar::I18nString & message, const libutil::exceptions::BaseException & e, 
                                                   const basar::I18nString & title ) const
    {
        BaseVC::showInfoMessage( message, e, title );
    }

    void BatchOrderPositionVC::showInfoMessage( const libutil::exceptions::BaseException & e, const basar::I18nString & title ) const
    {
        BaseVC::showInfoMessage( e, title );
    }

    void BatchOrderPositionVC::showInfoMessage( const basar::I18nString & message, const basar::I18nString & title ) const
    {
        BaseVC::showInfoMessage( message, title );
    }


    void BatchOrderPositionVC::showWarningMessage( const basar::I18nString & message, const libutil::exceptions::BaseException & e, 
                                                      const basar::I18nString & title ) const
    {
        BaseVC::showWarningMessage( message, e, title );
    }

    void BatchOrderPositionVC::showWarningMessage( const libutil::exceptions::BaseException & e, const basar::I18nString & title ) const
    {
        BaseVC::showWarningMessage( e, title );
    }

    void BatchOrderPositionVC::showWarningMessage( const basar::I18nString & message, const basar::I18nString & title ) const
    {
        BaseVC::showWarningMessage( message, title );
    }

    void BatchOrderPositionVC::showErrorMessage( const basar::I18nString & message, const libutil::exceptions::BaseException & e, 
                                                    const basar::I18nString & title ) const
    {
        BaseVC::showErrorMessage( message, e, title );
    }

    void BatchOrderPositionVC::showErrorMessage( const libutil::exceptions::BaseException & e, const basar::I18nString & title ) const
    {
        BaseVC::showErrorMessage( e, title );
    }

    void BatchOrderPositionVC::showErrorMessage( const basar::I18nString & message, const basar::I18nString & title ) const
    {
        BaseVC::showErrorMessage( message, title );
    }

    const basar::gui::tie::MessageBoxButtonEnum BatchOrderPositionVC::showQuestionMessage( const basar::I18nString & message, 
                                                                                              const libutil::exceptions::BaseException & e,
                                                                                              const basar::gui::tie::MessageBoxButtons buttons,
                                                                                              const basar::I18nString & title ) const
    {
        return BaseVC::showQuestionMessage( message, e, buttons, title );

    }

    const basar::gui::tie::MessageBoxButtonEnum BatchOrderPositionVC::showQuestionMessage( const libutil::exceptions::BaseException & e,
                                                                                              const basar::gui::tie::MessageBoxButtons buttons,
                                                                                              const basar::I18nString & title ) const
    {
        return BaseVC::showQuestionMessage( e, buttons, title );

    }

    const basar::gui::tie::MessageBoxButtonEnum BatchOrderPositionVC::showQuestionMessage( const basar::I18nString & message,
                                                                                              const basar::gui::tie::MessageBoxButtons buttons,
                                                                                              const basar::I18nString & title ) const
    {
        return BaseVC::showQuestionMessage( message, buttons, title );
    }

} //namespace viewconn
} //namespace CSC_Batches
