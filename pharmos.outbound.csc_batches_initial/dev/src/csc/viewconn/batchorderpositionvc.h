#ifndef GUARD_CSC_BATCHES_VIEWCONN_BATCHORDERPOSVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_BATCHORDERPOSVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "ibatchorderpositionvc.h"

#pragma warning (push)
#pragma warning(disable: 4127 4481 4512)
#include "ui_batchorderpositionvc.h"
#pragma warning (pop)

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
    class BatchOrderPositionVC : public libutil::viewConn::BaseVC, public IBatchOrderPositionVC
    {
        Q_OBJECT

    public:
        BatchOrderPositionVC( QWidget *parent = 0 );
        ~BatchOrderPositionVC();

        void                                                 setSession( libutil::infrastructure::session::ISessionPtr );

        // from IBatchOrderPosVC interface
        void                                                 matchFromBatchOrderHead( basar::db::aspect::AccessorPropertyTable_YIterator );
        void                                                 matchFromBatchOrderPositions( basar::db::aspect::AccessorPropertyTableRef );
        void                                                 matchFromCustomer( basar::db::aspect::AccessorPropertyTable_YIterator );
        //void                                                 matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator );
        basar::gui::tie::WidgetReturnEnum                    show();
        void                                                 hide();

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

    private slots:
        void                                                 onCloseClicked();

    private:
        libutil::infrastructure::session::ISessionPtr        getSession();

        void                                                 wireEvents();
        void                                                 initMatcher( basar::db::aspect::AccessorPropertyTableRef );
        void                                                 setupOrderFrame();
        void                                                 setupTableWidget();
        void                                                 initHeaderLabels();

        void                                                 labelWindowTitle();
        void                                                 clearTableWidget();

    private:
        Ui::BatchOrderPositionVCClass                        ui;

        libutil::infrastructure::session::ISessionPtr        m_Session;

        libutil::misc::Flag                                  m_EventsWiring;
        basar::gui::tie::MatcherRef                          m_OrderPosMatcher;

        std::list< basar::VarString >                        m_ListAttributes;
    };

} // namespace viewConn
} // namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_BATCHORDERPOSVC_H
