#ifndef GUARD_TENDER_VIEWCONN_BUSINESSTYPETRANSFORMER_H
#define GUARD_TENDER_VIEWCONN_BUSINESSTYPETRANSFORMER_H

#include <libutil/viewconn.h>
#include "libtender/domainmodule/tender/tendertypecollectionptr.h"

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
    class BusinessTypeTransformer : public basar::gui::tie::Transformer
    {
    public:
        BusinessTypeTransformer( basar::VarString guiColumnName, basar::VarString propTabColumnName, libtender::domMod::tender::TenderTypeCollectionPtr typeMapping );
	    ~BusinessTypeTransformer();

	    virtual void RightToLeft( basar::gui::tie::GuiPropertyTable_YIterator guiIt, const basar::db::aspect::AccessorPropertyTable_YIterator accIt );
	    virtual void LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator guiIt, basar::db::aspect::AccessorPropertyTable_YIterator accIt );
	    virtual boost::shared_ptr < basar::gui::tie::Transformer > create();

    private:
        basar::VarString								   m_guiColName;
        basar::VarString								   m_propTabColName;
		libtender::domMod::tender::TenderTypeCollectionPtr m_TypeMapping;
    };
} // namespace tender
} // namespace irm

#endif //GUARD_TENDER_VIEWCONN_BUSINESSTYPETRANSFORMER_H