#ifndef GUARD_CSC_BATCHES_VIEWCONN_BATCHORDERMAINTENANCEVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_BATCHORDERMAINTENANCEVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "ibatchordermaintenancevc.h"

#pragma warning (push)
#pragma warning(disable: 4127 4481 4512)
#include "ui_batchordermaintenancevc.h"
#pragma warning (pop)

#include "tablewidgetsorter.h"

#include <libutil/flag.h>
#include <libutil/viewconn.h>
#include <libutil/session.h>

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace CSC_Batches
{
namespace viewConn
{
    class BatchOrderMaintenanceVC : public libutil::viewConn::BaseVC, public IBatchOrderMaintenanceVC
    {
        Q_OBJECT

    public:
        BatchOrderMaintenanceVC( QWidget *parent = 0 );
        ~BatchOrderMaintenanceVC();

        void                                                 setSession( libutil::infrastructure::session::ISessionPtr );

        // from IBatchOrderMaintenanceVC interface
        void                                                 setTotalNumberOfLines( basar::Int32 );
        void                                                 matchFromBatchOrders( basar::db::aspect::AccessorPropertyTableRef );
        void                                                 matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator );

        basar::gui::tie::WidgetReturnEnum                    show();
        void                                                 waitForEvents();
        void                                                 hide();

        basar::db::aspect::AccessorPropertyTable_YIterator   getSelectedOrder();
        YIterators                                           getSelectedOrders();

        void                                                 showInfoMessage( const basar::I18nString & message, const libutil::exceptions::BaseException & e, 
                                                                              const basar::I18nString & title = basar::I18nString() ) const;
        void                                                 showInfoMessage( const libutil::exceptions::BaseException & e, 
                                                                              const basar::I18nString & title = basar::I18nString() ) const;
        void                                                 showInfoMessage( const basar::I18nString & message, 
                                                                              const basar::I18nString & title = basar::I18nString() ) const;

        void                                                 showWarningMessage( const basar::I18nString & message, const libutil::exceptions::BaseException & e,
                                                                                 const basar::I18nString & title = basar::I18nString() ) const;
        void                                                 showWarningMessage( const libutil::exceptions::BaseException & e, 
                                                                                 const basar::I18nString & title = basar::I18nString() ) const;
        void                                                 showWarningMessage( const basar::I18nString & message, 
                                                                                 const basar::I18nString & title = basar::I18nString() ) const;

        void                                                 showErrorMessage( const basar::I18nString & message, const libutil::exceptions::BaseException & e, 
                                                                               const basar::I18nString & title = basar::I18nString() ) const;
        void                                                 showErrorMessage( const libutil::exceptions::BaseException & e, 
                                                                               const basar::I18nString & title = basar::I18nString() ) const;
        void                                                 showErrorMessage( const basar::I18nString & message, 
                                                                               const basar::I18nString & title = basar::I18nString() ) const;

        const basar::gui::tie::MessageBoxButtonEnum          showQuestionMessage( const basar::I18nString & message, const libutil::exceptions::BaseException & e,
                                                                                  const basar::gui::tie::MessageBoxButtons buttons = basar::gui::tie::ButtonYes | basar::gui::tie::ButtonNo, 
                                                                                  const basar::I18nString & title = basar::I18nString() ) const;
        const basar::gui::tie::MessageBoxButtonEnum          showQuestionMessage( const libutil::exceptions::BaseException & e,
                                                                                  const basar::gui::tie::MessageBoxButtons buttons = basar::gui::tie::ButtonYes | basar::gui::tie::ButtonNo, 
                                                                                  const basar::I18nString & title = basar::I18nString() ) const;
        const basar::gui::tie::MessageBoxButtonEnum          showQuestionMessage( const basar::I18nString & message,
                                                                                  const basar::gui::tie::MessageBoxButtons buttons = basar::gui::tie::ButtonYes | basar::gui::tie::ButtonNo, 
                                                                                  const basar::I18nString & title = basar::I18nString() ) const;

        basar::Int32                                         showNumberOfOrdersToBeReleasedInputDialog();

    private slots:
        void                                                 onTableDoubleClicked( QModelIndex );
        void                                                 onTableDataSelectionChanged();
        void                                                 onSearchOrdersClicked();
        void                                                 onResetFilterClicked();
        void                                                 onResetSortClicked();
        void                                                 onShowPositionsClicked();
        void                                                 onCloseOrderMaintenanceClicked();
        void                                                 onReleaseSelectedOrdersClicked();
        void                                                 onReleaseFilteredOrdersClicked();

    private:
        libutil::infrastructure::session::ISessionPtr        getSession();

        void                                                 wireEvents();
        void                                                 initMatcher( basar::db::aspect::AccessorPropertyTableRef );
        void                                                 setupTableWidget();
        void                                                 setupTableWidgetSorter();
        void                                                 initHeaderLabels();
        void                                                 adjustTableSize();

        void                                                 labelWindowTitle();
        void                                                 preinitDateWidgets();
        void                                                 prepareDateWidgets();
        void                                                 setupFilterFrame();
        void                                                 clearLineEditFilters();
        void                                                 clearComboBoxes();
        void                                                 clearTableWidget();
        void                                                 restoreTableDefaultSorting();

    private:
        Ui::BatchOrderMaintenanceVCClass                     ui;

        libutil::infrastructure::session::ISessionPtr        m_Session;

        libutil::misc::Flag                                  m_EventsWiring;
        basar::gui::tie::MatcherRef                          m_OrdersMatcher;

        std::list< basar::VarString >                        m_ListAttributes;
        util::TableWidgetSorter                              m_TableWidgetSorter;
    };

} // namespace viewConn
} // namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_BATCHORDERMAINTENANCEVC_H
