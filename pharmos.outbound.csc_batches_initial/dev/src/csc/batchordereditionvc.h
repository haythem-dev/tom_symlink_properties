#ifndef CSC_BATCHES_VIEWCONN_BATCHORDEREDITIONVC_H
#define CSC_BATCHES_VIEWCONN_BATCHORDEREDITIONVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "ibatchordereditionvc.h"
#include <libbasarguitie_iviewconnector.h>

#pragma warning (push)
#pragma warning(disable: 4127 4481 4512)
#include <QtWidgets/QDialog>
#include "ui_batchordereditionvc.h"
#pragma warning (pop)

#include <libbasarguitie_matcher.h>
#include <libutil/flag.h>

#include "libabbauw/iuserfiltersettingsptr.h"
#include "libabbauw/idaystocolorparameterptr.h"
#include "tablewidgetsorter.h"
#include "iuserrightsptr.h"

namespace CSC_Batches
{
namespace viewConn
{

    class BatchOrderEditionVC : public QDialog, public IBatchOrderEditionVC, public basar::gui::tie::IViewConnector
    {
        Q_OBJECT

    public:
        BatchOrderEditionVC( QWidget *parent = 0 );
        ~BatchOrderEditionVC();

        void                                                        injectFilterParameter( libabbauw::domMod::parameter::IUserFilterSettingsPtr );
        void                                                        injectCalcParameter( libabbauw::domMod::parameter::IDaysToColorParameterPtr );
        void                                                        injectUserRights( infrastructure::rights::IUserRightsPtr );

        // from batch order edition interface
		void														init        ( basar::db::aspect::AccessorPropertyTableRef, bool isReservedQtyDifferent );
		virtual basar::gui::tie::WidgetReturnEnum	                show		();
	    virtual void								                hide		(){}
	    virtual bool								                shutdown	();

        virtual void                                                waitForEvents();
        virtual void                                                info        ( const basar::I18nString& );
        virtual void                                                error       ( const basar::I18nString& );
        virtual const basar::gui::tie::MessageBoxButtonEnum         question    ( const basar::I18nString& );
        virtual void                                                showOrderChangedNotification();

		virtual void                                                setNumberOfGreenOrders( basar::Int32 greenOrders );
        virtual void                                                setTotalNumberOfOrders( basar::Int32 );
        
        virtual void                                                matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator );
		virtual void												matchFromTransferOrders();
		
        virtual basar::db::aspect::AccessorPropertyTable_YIterator  getSelectedOrder();
		virtual YIterators                                          getSelectedOrders();

        virtual bool                                                isOrderViewActive();
        
    private slots:
        void                                                        onBtnSearchClicked();
		void														onBtnCloseClicked();
        void                                                        onBtnChangeViewClicked();
		void                                                        onBtnResetFilterClicked();
		void                                                        onBtnResetSortClicked();
		void                                                        onBtnChangeOrderClicked();
        void                                                        onBtnChangeOrderFilteredClicked();
        void                                                        onBtnSendORFilteredClicked();
        void                                                        onBtnSendORSelectedClicked();
		void														onBtnShowPositionsClicked();
		void														onBtnStatusBVOClicked();
		void                                                        onBtnReleaseOrdersClicked();
		void                                                        onBtnReleaseGreenOrdersClicked();
		void                                                        onClearSelectionShortcut();
		void                                                        onSelectGreenOrdersShortcut();
		void                                                        onTableDoubleClicked( QModelIndex );
		void                                                        onTableDataSelectionChanged();
		void                                                        onKeyHome();
		void                                                        onKeyEnd();
        
    private:
		void                                                        setupPermissions();
		void                                                        resizeEvent( QResizeEvent* );
		void                                                        closeEvent( QCloseEvent* );
        void                                                        wireEvents();
        void                                                        initHeaderLabels();
		void														initMatcher( basar::db::aspect::AccessorPropertyTableRef );
		void														labelWindowTitle();
		void                                                        prepareDateWidgets();
		void                                                        preinitDateWidgets();
		void														setupDateWidgets();
        void                                                        setupTableWidgetSorter();
		void                                                        disableButtons();
        void                                                        resetDaysToColor();
		void                                                        setupShortcuts();
		void														setupLineEdits();
		void                                                        setupComboBoxes();
		void                                                        setupTableWidget();
		void                                                        clearTableWidget();
		void                                                        clearLineEditFilters();
		void                                                        clearComboBoxes();
		void														adjustTableSize();
		void														adjustTableTextAlignment();
		void                                                        setTableDefaultSorting();
        void                                                        setSendBWEnabled();
        void                                                        enableBWButtons();
        bool                                                        checkIfCMIOrderSelected();
        bool                                                        checkIfCMIOrderFiltered();

    private:
        Ui::BatchOrderEditionVCClass                                ui;
		log4cplus::Logger                                           m_Logger;
        libutil::misc::Flag                                         m_EventsWiring;
        std::list< basar::VarString >                               m_ListAttributes;
		basar::gui::tie::MatcherRef                                 m_OrdersMatcher;
        util::TableWidgetSorter                                     m_TableWidgetSorter;
		libabbauw::domMod::parameter::IUserFilterSettingsPtr        m_UserFilterSettings;
        libabbauw::domMod::parameter::IDaysToColorParameterPtr      m_DaysToColorParameter;
        infrastructure::rights::IUserRightsPtr                      m_UserRights;
		basar::Int32                                                m_NumberOfGreenOrders;
        basar::Int32                                                m_TotalNumberOfOrders;

        bool                                                        m_IsReserveQtyDifferent;
    };

}
}

#endif // CSC_BATCHES_VIEWCONN_BATCHORDEREDITIONVC_H
