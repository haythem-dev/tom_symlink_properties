#ifndef GUARD_CSC_BATCHES_VIEWCONN_COLLECTIVEORDERWISHCONFIRMATIONVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_COLLECTIVEORDERWISHCONFIRMATIONVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "icollectiveorderwishconfirmationvc.h"

#pragma warning (push)
#pragma warning(disable: 4481 )
#include "ui_collectiveorderwishconfirmationvc.h"
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
    class CollectiveOrderWishConfirmationVC : public libutil::viewConn::BaseVC, public ICollectiveOrderWishConfirmationVC
    {
        Q_OBJECT

    public:
        CollectiveOrderWishConfirmationVC( QWidget *parent = 0, bool isFiltered = true );
        ~CollectiveOrderWishConfirmationVC();

        void                                               setSession( libutil::infrastructure::session::ISessionPtr );

        void                                               matchFromOrderWishes( basar::db::aspect::AccessorPropertyTableRef );
        void                                               matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator );

        basar::db::aspect::AccessorPropertyTable_YIterator getSelectedOrderWish();

        basar::gui::tie::WidgetReturnEnum                  show();
        void                                               hide();
        void                                               shutdown();

    private slots:
        void                                               onTableDataSelectionChanged();
        void                                               onRemoveOrderWishRequested();
        void                                               onCreateOrderWishesRequested();
        void                                               onCancelOrderWishesRequested();

    private:
        void                                               wireEvents();
        void                                               labelWindowTitle();
        void                                               setupTableWidget();
        void                                               adjustTableTextAlignment();
        void                                               initHeaderLabels();
        void                                               initMatcher( basar::db::aspect::AccessorPropertyTableRef );

    private:
        Ui::CollectiveOrderWishConfirmationVCClass         ui;
        bool                                               m_isFiltered;
        libutil::misc::Flag                                m_EventsWiring;
        std::list< basar::VarString >                      m_ListAttributes;
        basar::gui::tie::MatcherRef                        m_OrderWishMatcher;

        libutil::infrastructure::session::ISessionPtr m_Session;
    };

} // namespace viewConn
} // namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_COLLECTIVEORDERWISHCONFIRMATIONVC_H
