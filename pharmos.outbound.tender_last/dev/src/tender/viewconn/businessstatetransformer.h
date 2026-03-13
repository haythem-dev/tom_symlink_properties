#ifndef GUARD_TENDER_VIEWCONN_BUSINESSSTATETRANSFORMER_H
#define GUARD_TENDER_VIEWCONN_BUSINESSSTATETRANSFORMER_H

#include <libutil/viewconn.h>
#include "libtender/domainmodule/tender/tenderstatecollectionptr.h"

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

namespace tender
{
namespace viewConn
{
    class BusinessStateTransformer : public basar::gui::tie::Transformer
    {
    public:
        BusinessStateTransformer( basar::VarString guiColumnName, basar::VarString propTabColumnName, libtender::domMod::tender::TenderStateCollectionPtr typeMapping );
        ~BusinessStateTransformer();

        virtual void RightToLeft( basar::gui::tie::GuiPropertyTable_YIterator guiIt, const basar::db::aspect::AccessorPropertyTable_YIterator accIt );
        virtual void LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator guiIt, basar::db::aspect::AccessorPropertyTable_YIterator accIt );
        virtual boost::shared_ptr < basar::gui::tie::Transformer > create();

    private:
        basar::VarString                                        m_guiColName;
        basar::VarString                                        m_propTabColName;
        libtender::domMod::tender::TenderStateCollectionPtr     m_StateMapping;
    };
} // namespace tender
} // namespace irm

#endif //GUARD_TENDER_VIEWCONN_BUSINESSSTATETRANSFORMER_H
