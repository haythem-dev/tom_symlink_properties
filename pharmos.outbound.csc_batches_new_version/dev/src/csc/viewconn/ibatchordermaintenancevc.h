#ifndef GUARD_CSC_BATCHES_VIEWCONN_IBATCHORDERMAINTENANCEVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_IBATCHORDERMAINTENANCEVC_H

#include <libutil/viewconn.h>
#include <vector>

//-------------------------------------------------------------------------------------------------//
// forward declarations
//-------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
            class AccessorPropertyTableRef;
        }
    }
}

//-------------------------------------------------------------------------------------------------//
// interface declaration section
//-------------------------------------------------------------------------------------------------//
namespace CSC_Batches
{
namespace viewConn
{
    class IBatchOrderMaintenanceVC : public libutil::viewConn::IMessageDisplayer
    {
    public:
        virtual ~IBatchOrderMaintenanceVC() {}
        typedef std::vector< basar::db::aspect::AccessorPropertyTable_YIterator > YIterators;

        virtual void                                                 matchFromBatchOrders( basar::db::aspect::AccessorPropertyTableRef )         = 0;
        virtual void                                                 matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;

        virtual basar::gui::tie::WidgetReturnEnum                    show()                                                                      = 0;
        virtual void                                                 waitForEvents()                                                             = 0;
        virtual void                                                 hide()                                                                      = 0;

        virtual basar::db::aspect::AccessorPropertyTable_YIterator   getSelectedOrder()                                                          = 0;
        virtual YIterators                                           getSelectedOrders()                                                         = 0;

        virtual basar::Int32                                         showNumberOfOrdersToBeReleasedInputDialog()                                 = 0;
    };

} // namespace viewConn
} // namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_IBATCHORDERMAINTENANCEVC_H
