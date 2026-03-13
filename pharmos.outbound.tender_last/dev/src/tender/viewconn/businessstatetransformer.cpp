// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>

#include "businessstatetransformer.h"
#include "libbasar_definitions.h"
#include "libtender/domainmodule/tender/tenderstatecollection.h"

namespace tender
{
namespace viewConn
{
    BusinessStateTransformer::BusinessStateTransformer( basar::VarString guiColumnName, basar::VarString propTabColumnName, libtender::domMod::tender::TenderStateCollectionPtr stateMapping )
    {
        m_guiColName =     guiColumnName;
        m_propTabColName = propTabColumnName;
        m_StateMapping = stateMapping;
    }

    BusinessStateTransformer::~BusinessStateTransformer() 
    {}

    void BusinessStateTransformer::RightToLeft( basar::gui::tie::GuiPropertyTable_YIterator guiIt, const basar::db::aspect::AccessorPropertyTable_YIterator accIt )
    {
        basar::I18nString translatedBusinessTypeName = 
            m_StateMapping->getBusinessStateName( static_cast< libtender::domMod::tender::TenderStateEnum >( accIt.getInt16( m_propTabColName ) ));
        guiIt.setString( m_guiColName, translatedBusinessTypeName );
    }

    void BusinessStateTransformer::LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator guiIt, basar::db::aspect::AccessorPropertyTable_YIterator accIt )
    {
    }

    boost::shared_ptr < basar::gui::tie::Transformer > BusinessStateTransformer::create()
    {
        return boost::shared_ptr < basar::gui::tie::Transformer > ( new BusinessStateTransformer(*this) );
    }

} //end namespace viewConn
} //end namespace tender
