// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include <infrastructure/accessor/cmnaccessor.h>

#include "businesstypetransformer.h"
#include "libbasar_definitions.h"
#include "libtender/domainmodule/tender/tendertypecollection.h"

namespace tender
{
namespace viewConn
{
    BusinessTypeTransformer::BusinessTypeTransformer( basar::VarString guiColumnName, basar::VarString propTabColumnName, libtender::domMod::tender::TenderTypeCollectionPtr typeMapping )
    {
        m_guiColName =     guiColumnName;
        m_propTabColName = propTabColumnName;
		m_TypeMapping = typeMapping;
    }

    BusinessTypeTransformer::~BusinessTypeTransformer() 
    {}

    void BusinessTypeTransformer::RightToLeft( basar::gui::tie::GuiPropertyTable_YIterator guiIt, const basar::db::aspect::AccessorPropertyTable_YIterator accIt )
    {
        basar::I18nString translatedBusinessTypeName = 
		 m_TypeMapping->getBusinessTypeName( static_cast< libtender::domMod::tender::TenderTypeEnum >( accIt.getInt32( m_propTabColName ) ));
		guiIt.setString( m_guiColName, translatedBusinessTypeName );
    }

    void BusinessTypeTransformer::LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator guiIt, basar::db::aspect::AccessorPropertyTable_YIterator accIt )
    {
    }

	boost::shared_ptr < basar::gui::tie::Transformer > BusinessTypeTransformer::create()
    {
	    return boost::shared_ptr < basar::gui::tie::Transformer > ( new BusinessTypeTransformer(*this) );
    }

} //end namespace viewConn
} //end namespace tender
