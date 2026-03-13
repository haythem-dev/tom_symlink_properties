#include "batchordermaintenancevc.h"
#include "definitions_gui.h"

#include <libabbauw/loggerpool/libabbauw_loggerpool.h>
#include <libabbauw/libabbauw_properties_definitions.h>

#include "waitcursor.h"
#include <libutil/viewconn.h>
#include <QtWidgets/QInputDialog>
#include <set>

namespace CSC_Batches
{
namespace viewConn
{
    BatchOrderMaintenanceVC::BatchOrderMaintenanceVC( QWidget* parent )
    : BaseVC( parent ),
        m_EventsWiring ( "BatchOrderMaintenanceVC::EventsWiring" )
    {
        METHODNAME_DEF( BatchOrderMaintenanceVC, BatchOrderMaintenanceVC );
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );
 
        ui.setupUi( this );
        wireEvents();
        initHeaderLabels();
        setupFilterFrame();
        setupTableWidget();
        setupTableWidgetSorter();
    }

    BatchOrderMaintenanceVC::~BatchOrderMaintenanceVC()
    {
        METHODNAME_DEF( BatchOrderMaintenanceVC, ~BatchOrderMaintenanceVC )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );
    }

    void BatchOrderMaintenanceVC::setSession( libutil::infrastructure::session::ISessionPtr session )
    {
        m_Session = session;
    }

    libutil::infrastructure::session::ISessionPtr BatchOrderMaintenanceVC::getSession()
    {
        CHECK_INSTANCE_EXCEPTION( m_Session.get() );
        return m_Session;
    }

    void BatchOrderMaintenanceVC::labelWindowTitle()
    {
	    QString windowTitle;
        windowTitle.append( DLG_TITLESHORT1 ).append( DLG_NO_BATCHORDERS ).append( DLG_TITLESHORT2 );
	    windowTitle.append( tr( "Release Batches" ) );
	    windowTitle.append( tr( " area: " ) );
        windowTitle.append( QString::fromLocal8Bit( getSession()->getUser()->getAreaName().c_str() ) );
        setWindowTitle( windowTitle );
    }

    void BatchOrderMaintenanceVC::wireEvents()
    {
        METHODNAME_DEF( BatchOrderMaintenanceVC, wireEvents ); fun;
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        if( m_EventsWiring.isSet() )
        {
            return;
        }

        connect( ui.twBatchOrders,            SIGNAL( doubleClicked( QModelIndex )          ),
	             this,                        SLOT  ( onTableDoubleClicked( QModelIndex ) ) );
	    connect( ui.twBatchOrders,            SIGNAL( dataSelectionChanged()                ),
	             this,                        SLOT  ( onTableDataSelectionChanged() )       );
        connect( ui.btnClose,                 SIGNAL( clicked()                             ),
                 this,                        SLOT  ( onCloseOrderMaintenanceClicked() )    );
        connect( ui.btnReleaseFilteredOrders, SIGNAL( clicked()                             ),
                 this,                        SLOT  ( onReleaseFilteredOrdersClicked() )    );
        connect( ui.btnReleaseSelectedOrders, SIGNAL( clicked()                             ),
                 this,                        SLOT  ( onReleaseSelectedOrdersClicked() )    );
        connect( ui.btnResetFilter,           SIGNAL( clicked()                             ),
                 this,                        SLOT  ( onResetFilterClicked() )              );
        connect( ui.btnResetSort,             SIGNAL( clicked()                             ),
                 this,                        SLOT  ( onResetSortClicked() )                );
        connect( ui.btnSearch,                SIGNAL( clicked()                             ),
                 this,                        SLOT  ( onSearchOrdersClicked() )             );
        connect( ui.btnShowPositions,         SIGNAL( clicked()                             ),
                 this,                        SLOT  ( onShowPositionsClicked() )            );

        m_EventsWiring.set();
    }

    void BatchOrderMaintenanceVC::initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab )
    {
        m_OrdersMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.twBatchOrders, propTab );
        m_OrdersMatcher.transformProperties( ui.twBatchOrders->horizontalHeaderLabels(), m_ListAttributes );
    }

    void BatchOrderMaintenanceVC::setupTableWidget()
    {
        ui.twBatchOrders->setABCVerticalHeaderPolicy();
	    ui.twBatchOrders->setSelectionBehavior( QAbstractItemView::SelectRows );
	    // don't allow to edit data in table
	    ui.twBatchOrders->setEditTriggers( QAbstractItemView::NoEditTriggers );

        ui.twBatchOrders->setTextAlignmentColumn(  0, Qt::AlignRight   | Qt::AlignVCenter ); // PHARMACYNO
        ui.twBatchOrders->setTextAlignmentColumn(  2, Qt::AlignRight   | Qt::AlignVCenter ); // ORDERNO
        ui.twBatchOrders->setTextAlignmentColumn(  3, Qt::AlignCenter  | Qt::AlignVCenter ); // STATUS
        ui.twBatchOrders->setTextAlignmentColumn(  4, Qt::AlignRight   | Qt::AlignVCenter ); // POSQTY
        ui.twBatchOrders->setTextAlignmentColumn(  5, Qt::AlignCenter  | Qt::AlignVCenter ); // ORDERACQUISITIONDATE
    }

    void BatchOrderMaintenanceVC::setupTableWidgetSorter()
    {
        m_TableWidgetSorter.setTableWidget( ui.twBatchOrders );
        m_TableWidgetSorter.setListAttributes( m_ListAttributes );

        m_TableWidgetSorter.addSortableColumn( libabbauw::properties::PHARMACYNO           );
        m_TableWidgetSorter.addSortableColumn( libabbauw::properties::PHARMACYNAME         );
        m_TableWidgetSorter.addSortableColumn( libabbauw::properties::ORDERNO              );
        m_TableWidgetSorter.addSortableColumn( libabbauw::properties::STATUS               );
        m_TableWidgetSorter.addSortableColumn( libabbauw::properties::ORDERACQUISITIONDATE );
        m_TableWidgetSorter.addSortableColumn( libabbauw::properties::DATASOURCETYPE       );

        restoreTableDefaultSorting();
    }

    void BatchOrderMaintenanceVC::initHeaderLabels()
    {
        QStringList batchOrdersHeaders;
        
        batchOrdersHeaders.push_back( tr("Pharmacy No")                           );
        m_ListAttributes.push_back  ( libabbauw::properties::PHARMACYNO           );

        batchOrdersHeaders.push_back( tr("Pharmacy Name")                         );
        m_ListAttributes.push_back  ( libabbauw::properties::PHARMACYNAME         );

        batchOrdersHeaders.push_back( tr("Order No")                              );
        m_ListAttributes.push_back  ( libabbauw::properties::ORDERNO              );

        batchOrdersHeaders.push_back( tr("Status")                                );
        m_ListAttributes.push_back  ( libabbauw::properties::STATUS               );

        batchOrdersHeaders.push_back( tr("Pos.")                                  );
        m_ListAttributes.push_back  ( libabbauw::properties::POSQTY               );

        batchOrdersHeaders.push_back( tr("Order Acquisition Date")                );
        m_ListAttributes.push_back  ( libabbauw::properties::ORDERACQUISITIONDATE );

        batchOrdersHeaders.push_back( tr("Data Source")                           );
        m_ListAttributes.push_back  ( libabbauw::properties::DATASOURCETYPE       );

        ui.twBatchOrders->setColumnCount( static_cast<int>(m_ListAttributes.size()) );
        ui.twBatchOrders->setHorizontalHeaderLabels( batchOrdersHeaders );
    }

    void BatchOrderMaintenanceVC::adjustTableSize()
    {
	    // WORKAROUND: store size when method is called the first time to store the vertical header's default size.
	    //             width() is updated only if GUI thread repaints the window and so sometimes still returns 0 
	    //             although the vertical header has been set and is visible
	    static const int vertHeaderWidth = ui.twBatchOrders->verticalHeader()->width();
	    const int anchor = ( ui.twBatchOrders->rowCount() > 0 ) ? vertHeaderWidth - 3 : 0;

        int i = 0;
	    ui.twBatchOrders->setColumnWidth( i++, ui.line1->x()  - anchor          );
	    ui.twBatchOrders->setColumnWidth( i++, ui.line2->x()  - ui.line1->x()   );
	    ui.twBatchOrders->setColumnWidth( i++, ui.line3->x()  - ui.line2->x()   );
	    ui.twBatchOrders->setColumnWidth( i++, ui.line4->x()  - ui.line3->x()   );
	    ui.twBatchOrders->setColumnWidth( i++, ui.line5->x()  - ui.line4->x()   );
	    ui.twBatchOrders->setColumnWidth( i++, ui.line6->x()  - ui.line5->x()   );
	    ui.twBatchOrders->setColumnWidth( i++, ui.line7->x()  - ui.line6->x()   );

	    ui.twBatchOrders->horizontalHeader()->setStretchLastSection( true );	// auto adjust last column

        // if the table is full then adjust all entries to occupy the full space of the table
        const int maxDisplayedRows = 15;
	    if( ui.twBatchOrders->rowCount() >= maxDisplayedRows )
        {
		    ui.twBatchOrders->verticalHeader()->setSectionResizeMode( QHeaderView::Stretch );
        }
	    else
	    {
            // table is not full - don't occupy the whole space because it looks bad
		    ui.twBatchOrders->verticalHeader()->setSectionResizeMode( QHeaderView::Custom );

		    const int rowHeight = (ui.twBatchOrders->geometry().height() - ui.twBatchOrders->horizontalHeader()->geometry().height()) / maxDisplayedRows;
		    for( int irc = 0; irc < ui.twBatchOrders->rowCount(); irc++ )
            {
			    ui.twBatchOrders->setRowHeight( irc, rowHeight );
            }
	    }
    }

    void BatchOrderMaintenanceVC::prepareDateWidgets()
    {
	    QDate minDate( 2000,  1,  1 );
	    QDate maxDate( 2099, 12, 31 );

        ui.dtOrderAcquisitionDateFrom->setMaximumDate( maxDate );
        ui.dtOrderAcquisitionDateFrom->setMinimumDate( minDate );
        ui.dtOrderAcquisitionDateTo->setMaximumDate( maxDate );
        ui.dtOrderAcquisitionDateTo->setMinimumDate( minDate );

	    preinitDateWidgets();
    }

    void BatchOrderMaintenanceVC::preinitDateWidgets()
    {
        basar::Date startDate; startDate.addDays( -1 ); // equals today - 1
	    basar::Date endDate; // equals today

	    QDate fromDate( startDate.getYear(),  startDate.getMonth(), startDate.getDay() );
	    QDate toDate(   endDate.getYear(),    endDate.getMonth(),   endDate.getDay()   );

        ui.dtOrderAcquisitionDateFrom->setDate( fromDate );
        ui.dtOrderAcquisitionDateTo->setDate  ( toDate   );
    }

    void BatchOrderMaintenanceVC::setupFilterFrame()
    {
        // txt
        ui.txtDataSource->setAccessibleName      ( libabbauw::properties::DATASOURCETYPE.getName().c_str()   );
        ui.txtOrderNoFrom->setAccessibleName     ( libabbauw::properties::ORDERNO_FROM.getName().c_str()     );
        ui.txtOrderNoTo->setAccessibleName       ( libabbauw::properties::ORDERNO_TO.getName().c_str()       );
        ui.txtPharmacyName->setAccessibleName    ( libabbauw::properties::PHARMACYNAME.getName().c_str()     );
        ui.txtPharmacyNo->setAccessibleName      ( libabbauw::properties::PHARMACYNO.getName().c_str()       );
        ui.txtPositionsFrom->setAccessibleName   ( libabbauw::properties::POS_FROM.getName().c_str()         );
        ui.txtPositionsTo->setAccessibleName     ( libabbauw::properties::POS_TO.getName().c_str()           );

        // date
        ui.dtOrderAcquisitionDateFrom->setAccessibleName( libabbauw::properties::ORDERACQUISITIONDATE_FROM.getName().c_str() );
	    ui.dtOrderAcquisitionDateTo->setAccessibleName  ( libabbauw::properties::ORDERACQUISITIONDATE_TO.getName().c_str()   );

        //cbo
        ui.cboStatus->addItem( ""   );
	    ui.cboStatus->addItem( "ZU" );
	    ui.cboStatus->addItem( "FE" );
	    ui.cboStatus->addItem( "ER" );
	    ui.cboStatus->addItem( "ST" );
    }

    void BatchOrderMaintenanceVC::restoreTableDefaultSorting()
    {
        util::SortOrder sortOrder = m_TableWidgetSorter.getSortOrder();
        sortOrder.setOrder( libabbauw::properties::ORDERNO, util::SortOrder::ASCENDING );
        m_TableWidgetSorter.setSortOrder( sortOrder );
    }

    void BatchOrderMaintenanceVC::clearLineEditFilters()
    {
	    ui.txtOrderNoFrom->clear();
	    ui.txtOrderNoTo->clear();
	    ui.txtPharmacyName->clear();
	    ui.txtPharmacyNo->clear();
        ui.txtPositionsFrom->clear();
        ui.txtPositionsTo->clear();
        ui.txtDataSource->clear();
    }

    void BatchOrderMaintenanceVC::clearComboBoxes()
    {
	    ui.cboStatus->setCurrentIndex(       0 );
    }

    void BatchOrderMaintenanceVC::clearTableWidget()
    {
	    ui.twBatchOrders->clearContents();
	    ui.twBatchOrders->setRowCount( 0 );
	    adjustTableSize();
    }

    void BatchOrderMaintenanceVC::onTableDoubleClicked( QModelIndex index )
    {
        ui.twBatchOrders->setCurrentCell( index.row(), index.column(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
        onShowPositionsClicked();
    }

    void BatchOrderMaintenanceVC::onTableDataSelectionChanged()
    {
	    bool releaseAllowed = true;
	
	    YIterators yits     = getSelectedOrders();
	    bool enable         = ! yits.empty();          // at least one order is selected

	    for( YIterators::const_iterator it = yits.begin(); it != yits.end() && releaseAllowed; ++it )
	    {
		    const bool status_ZU   = it->getString( libabbauw::properties::STATUS ) == "ZU";
		    if( !status_ZU )
            {
			    releaseAllowed = false;
            }
	    }

        ui.btnReleaseSelectedOrders->setEnabled( enable & releaseAllowed );
	    ui.btnShowPositions->setEnabled( enable );
    }

    void BatchOrderMaintenanceVC::onResetFilterClicked()
    {
	    preinitDateWidgets();
	    clearLineEditFilters();
	    clearComboBoxes();
    }

    void BatchOrderMaintenanceVC::onResetSortClicked()
    {
	    restoreTableDefaultSorting();
    }

    void BatchOrderMaintenanceVC::onShowPositionsClicked()
    {
        CSC_Batches::gui::WaitCursor bc( this );
        basar::appl::SystemEventManager::getInstance().fire( "ShowPositionsRequested" );
    }

    void BatchOrderMaintenanceVC::onSearchOrdersClicked()
    {
        CSC_Batches::gui::WaitCursor bc( this );
        basar::appl::SystemEventManager::getInstance().fire( "SearchBatchOrdersRequested" );
    }

    void BatchOrderMaintenanceVC::onCloseOrderMaintenanceClicked()
    {
        METHODNAME_DEF( BatchOrderMaintenanceVC, onCloseOrderMaintenanceClicked )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        close();
    }

    void BatchOrderMaintenanceVC::onReleaseSelectedOrdersClicked()
    {
        CSC_Batches::gui::WaitCursor bc( this );
        basar::appl::SystemEventManager::getInstance().fire( "ReleaseSelectedOrdersRequested" );
    }

    void BatchOrderMaintenanceVC::onReleaseFilteredOrdersClicked()
    {
        CSC_Batches::gui::WaitCursor bc( this );
        basar::appl::SystemEventManager::getInstance().fire( "ReleaseFilteredOrdersRequested" );
    }

    basar::gui::tie::WidgetReturnEnum BatchOrderMaintenanceVC::show()
    {
        METHODNAME_DEF( BatchOrderMaintenanceVC, show )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        labelWindowTitle();
        setAttribute( Qt::WA_Moved, false );

        QWidget::setWindowModality( Qt::ApplicationModal );
        QDialog::show(); // needed for line1.x(), line2.x() ... to get actual value (otherwise all coords are 0 if mask not shown)

	    prepareDateWidgets();
        adjustTableSize();

        QDialog::show();
        return basar::gui::tie::WidgetReturnEnum();
    }

    void BatchOrderMaintenanceVC::waitForEvents()
    {
        exec();
    }

    void BatchOrderMaintenanceVC::hide()
    {
        METHODNAME_DEF( BatchOrderMaintenanceVC, fun )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        QWidget::hide();
    }

    void BatchOrderMaintenanceVC::matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch )
    {
        basar::gui::tie::MatcherRef orderSearchMatcher = basar::gui::tie::Manager::getInstance().createMatcher( ui.grpFilter, 
                                                                                                                yitSearch.getPropertyTable() );
	    orderSearchMatcher.LeftToRight( );

        // BUG? Why does an empty text field match?
        if( ui.txtDataSource->text().isEmpty() )
        {
            yitSearch.setPropertyState( libabbauw::properties::DATASOURCETYPE, basar::SS_UNSET );
        }

        QString text = ui.cboStatus->currentText();
	    if( ! text.isEmpty() )
        {
		    yitSearch.setString( libabbauw::properties::STATUS, text.toLocal8Bit().constData() );
        }

        if( ! m_TableWidgetSorter.getSortOrder().empty() )
        {
            yitSearch.setString( libabbauw::properties::SORT, m_TableWidgetSorter.getSortOrder().toString() );
        }
    }

    void BatchOrderMaintenanceVC::matchFromBatchOrders( basar::db::aspect::AccessorPropertyTableRef propTab )
    {
        METHODNAME_DEF( BatchOrderMaintenanceVC, matchFromBatchOrders )
        BLOG_TRACE_METHOD( libabbauw::LoggerPool::loggerViewConn, fun );

        initMatcher(propTab);

        ui.twBatchOrders->clearContents();
	    ui.twBatchOrders->setRowCount(15);
        basar::Int32 matchedHits = m_OrdersMatcher.RightToLeft( 0, ui.twBatchOrders->rowCount() );
	    ui.twBatchOrders->setRowCount(matchedHits);

        if(matchedHits > 0)
        {
            ui.twBatchOrders->selectRow(0);
            ui.twBatchOrders->setFocus();
        }

        ui.btnReleaseFilteredOrders->setEnabled(matchedHits > 0);

        // counts
        const size_t count = m_OrdersMatcher.getCurrentRight().getPropertyTable().size();
        ui.txtOrderCount->setText( QString::number( static_cast< basar::Int32 >( count ) ) );

        // use std::set to count pharmacy numbers
        std::set< basar::Int32 > pharmacyNumbers;
        basar::Int32 lineCount = 0;
        for( basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin(); ! yit.isEnd(); ++yit )
        {
            pharmacyNumbers.insert( yit.getInt32( libabbauw::properties::PHARMACYNO ) );
            lineCount += yit.getInt32( libabbauw::properties::POSQTY );
        }
        ui.txtPharmacyCount->setText( QString::number( pharmacyNumbers.size() ) );
        ui.txtLineCount->setText    ( QString::number( lineCount ) );

        adjustTableSize();
    }

    basar::db::aspect::AccessorPropertyTable_YIterator BatchOrderMaintenanceVC::getSelectedOrder()
    {
	    return m_OrdersMatcher.getSelectedRight();
    }

    BatchOrderMaintenanceVC::YIterators BatchOrderMaintenanceVC::getSelectedOrders()
    {
	    YIterators yiterators;
	    for( basar::db::aspect::AccessorPropertyTable_YIterator it = m_OrdersMatcher.getSelectedRight(); !it.isEnd(); 
		    it = m_OrdersMatcher.getNextSelectedRight() )
	    {
		    yiterators.push_back( it );
	    }

	    return yiterators;
    }

    void BatchOrderMaintenanceVC::showInfoMessage( const basar::I18nString & message, const libutil::exceptions::BaseException & e, 
                                                   const basar::I18nString & title ) const
    {
        BaseVC::showInfoMessage( message, e, title );
    }

    void BatchOrderMaintenanceVC::showInfoMessage( const libutil::exceptions::BaseException & e, const basar::I18nString & title ) const
    {
        BaseVC::showInfoMessage( e, title );
    }

    void BatchOrderMaintenanceVC::showInfoMessage( const basar::I18nString & message, const basar::I18nString & title ) const
    {
        BaseVC::showInfoMessage( message, title );
    }


    void BatchOrderMaintenanceVC::showWarningMessage( const basar::I18nString & message, const libutil::exceptions::BaseException & e, 
                                                      const basar::I18nString & title ) const
    {
        BaseVC::showWarningMessage( message, e, title );
    }

    void BatchOrderMaintenanceVC::showWarningMessage( const libutil::exceptions::BaseException & e, const basar::I18nString & title ) const
    {
        BaseVC::showWarningMessage( e, title );
    }

    void BatchOrderMaintenanceVC::showWarningMessage( const basar::I18nString & message, const basar::I18nString & title ) const
    {
        BaseVC::showWarningMessage( message, title );
    }

    void BatchOrderMaintenanceVC::showErrorMessage( const basar::I18nString & message, const libutil::exceptions::BaseException & e, 
                                                    const basar::I18nString & title ) const
    {
        BaseVC::showErrorMessage( message, e, title );
    }

    void BatchOrderMaintenanceVC::showErrorMessage( const libutil::exceptions::BaseException & e, const basar::I18nString & title ) const
    {
        BaseVC::showErrorMessage( e, title );
    }

    void BatchOrderMaintenanceVC::showErrorMessage( const basar::I18nString & message, const basar::I18nString & title ) const
    {
        BaseVC::showErrorMessage( message, title );
    }

    const basar::gui::tie::MessageBoxButtonEnum BatchOrderMaintenanceVC::showQuestionMessage( const basar::I18nString & message, 
                                                                                              const libutil::exceptions::BaseException & e,
                                                                                              const basar::gui::tie::MessageBoxButtons buttons,
                                                                                              const basar::I18nString & title ) const
    {
        return BaseVC::showQuestionMessage( message, e, buttons, title );

    }

    const basar::gui::tie::MessageBoxButtonEnum BatchOrderMaintenanceVC::showQuestionMessage( const libutil::exceptions::BaseException & e,
                                                                                              const basar::gui::tie::MessageBoxButtons buttons,
                                                                                              const basar::I18nString & title ) const
    {
        return BaseVC::showQuestionMessage( e, buttons, title );

    }

    const basar::gui::tie::MessageBoxButtonEnum BatchOrderMaintenanceVC::showQuestionMessage( const basar::I18nString & message,
                                                                                              const basar::gui::tie::MessageBoxButtons buttons,
                                                                                              const basar::I18nString & title ) const
    {
        return BaseVC::showQuestionMessage( message, buttons, title );
    }


    basar::Int32 BatchOrderMaintenanceVC::showNumberOfOrdersToBeReleasedInputDialog()
    {
        bool ok = false;
        QString msg = tr( "Number of orders to be released" );
        basar::Int32 value = QInputDialog::getInt( this, msg, msg, 200, 0, 200, 1, & ok );

        if( ! ok )
        {
            showWarningMessage( tr( "Invalid input!" ).toLocal8Bit().constData(), tr( "Warning" ).toLocal8Bit().constData() );
            value = -1;
        }

        return value;
    }


} //namespace viewconn
} //namespace CSC_Batches
