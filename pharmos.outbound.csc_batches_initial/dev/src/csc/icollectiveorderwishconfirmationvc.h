#ifndef GUARD_CSC_BATCHES_VIEWCONN_ICOLLECTIVEORDERWISHCONFIRMATIONVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_ICOLLECTIVEORDERWISHCONFIRMATIONVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libutil/viewconn.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace gui
    {
        namespace tie
        {
            enum WidgetReturnEnum;
        }
    }
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
        }
    }
}

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace CSC_Batches
{
namespace viewConn
{
    class ICollectiveOrderWishConfirmationVC
    {
        public:
            virtual ~ICollectiveOrderWishConfirmationVC() {}

            virtual void                                               matchFromOrderWishes ( basar::db::aspect::AccessorPropertyTableRef )        = 0;
            virtual void                                               matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

            virtual basar::db::aspect::AccessorPropertyTable_YIterator getSelectedOrderWish() = 0;

            virtual basar::gui::tie::WidgetReturnEnum                  show()     = 0;
            virtual void                                               shutdown() = 0;
    };

} // namespace viewConn
} // namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_ICOLLECTIVEORDERWISHCONFIRMATIONVC_H
