#ifndef GUARD_CSC_BATCHES_VIEWCONN_IBATCHORDERPOSITIONVC_H
#define GUARD_CSC_BATCHES_VIEWCONN_IBATCHORDERPOSITIONVC_H

#include <libutil/viewconn.h>

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
    class IBatchOrderPositionVC : public libutil::viewConn::IMessageDisplayer
    {
    public:
        virtual ~IBatchOrderPositionVC() {}

        virtual void                                                 matchFromBatchOrderHead( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
        virtual void                                                 matchFromBatchOrderPositions( basar::db::aspect::AccessorPropertyTableRef   ) = 0;
        virtual void                                                 matchFromCustomer( basar::db::aspect::AccessorPropertyTable_YIterator )       = 0;
        //virtual void                                                 matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
        virtual basar::gui::tie::WidgetReturnEnum                    show()                                                                        = 0;
        virtual void                                                 hide()                                                                        = 0;
    };

} // namespace viewConn
} // namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_VIEWCONN_IBATCHORDERPOSITIONVC_H
