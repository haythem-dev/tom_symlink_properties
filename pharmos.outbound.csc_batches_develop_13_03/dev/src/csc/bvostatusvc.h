//-------------------------------------------------------------------------------------------------//
/*! \file 
*   \brief ViewConnector for input of articles.
*   \author 
*   \date 
*/
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_CSC_Batches_VIEWCONN_BVOSTATUSVC_H
#define GUARD_CSC_Batches_VIEWCONN_BVOSTATUSVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "ibvostatusvc.h"
#include "tablewidgetsorter.h"
#include <libbasarguitie_iviewconnector.h>

#pragma warning (push)
#pragma warning(disable: 4231 4481 4800)
#include <QtWidgets/QDialog>
#include "ui_bvostatusvc.h"
#pragma warning (pop)

#include <libbasarguitie_matcher.h>
#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace CSC_Batches
{
namespace viewConn
{

class BVOStatusVC : public QDialog, public IBVOStatusVC, public basar::gui::tie::IViewConnector
{
    Q_OBJECT

    public:
        BVOStatusVC(QWidget *parent = 0);
        ~BVOStatusVC();

        ///////////////////////////////////////////////////////////////////////////////////////
        //
        ///////////////////////////////////////////////////////////////////////////////////////
        void                                                        init( basar::db::aspect::AccessorPropertyTableRef  );
		void										                init( basar::db::aspect::AccessorPropertyTableRef, basar::Int32 );
        virtual basar::gui::tie::WidgetReturnEnum	                show();
        virtual void								                hide();
		virtual void                                                waitForEvents();
        virtual bool                                                shutdown();

        void                                                        matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator );
        void                                                        matchFromProposal();
		basar::db::aspect::AccessorPropertyTable_YIterator          getSelectedOrderProposal();

		void                                                        info( const basar::I18nString& text );
        void                                                        error( const basar::I18nString& text );
        const basar::gui::tie::MessageBoxButtonEnum                 question( const basar::I18nString& text );

		QWidget*													getWidget();

    private:
        void                                                        wireEvents();

        void                                                        configureProposalFrame();
        void                                                        clearProposalSearchFrame();
        void                                                        adjustSizeProposalSearchFrame();
        void                                                        disableProposalFrame( const bool );
		void														adjustTableTextAlignment();
		void								                        labelWindowTitle();

        /////////////////////////////////////////////////////////////////////////////////////////
        ////
        /////////////////////////////////////////////////////////////////////////////////////////
        void                                                        initProposalDisplayFrame( basar::db::aspect::AccessorPropertyTableRef );
        void                                                        initHeaderLablesProposalsDisplayFrame( QStringList& );
        void                                                        configureProposalsDisplayFrame( QStringList& );
        void                                                        clearProposalsDisplayFrame();
        void                                                        adjustSizeProposalsDisplayFrame();
        void                                                        disableProposalsDisplayFrame( const bool );
        void                                                        initMatcherProposalsDisplayFrame( basar::db::aspect::AccessorPropertyTableRef );

		void                                                        resetProposalsDisplayFrame();
		void								                        setupTitleLabel();
		void								                        setupTitleLabel(basar::Int32);
        void                                                        setupTableWidgetSorter();

    private slots:
        //////////////////////////////////////////////////////////////
        // private slots
        //////////////////////////////////////////////////////////////
        void							                            onBtnCloseClicked();
        void                                                        onBtnRefreshClicked();
		void                                                        onBtnCancelOrderClicked();
		void														onBtnExportCSVClicked();
		void                                                        onSelectionChanged();

    private:
        Ui::frBVOTransfer					                        ui;
        basar::gui::tie::MatcherRef                                 m_ProposalMatcher;
        std::list<basar::VarString>                                 m_ListAttributes;
        util::TableWidgetSorter                                     m_TableWidgetSorter;
};

} // namespace viewConn
} // namespace CSC_Batches

#endif // GUARD_CSC_Batches_VIEWCONN_BVOSTATUSVC_H
