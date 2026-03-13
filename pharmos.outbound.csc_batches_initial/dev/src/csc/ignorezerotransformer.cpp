//----------------------------------------------------------------------------
/*! \file
 *  \brief   transformer which ignores 0 in integer colums
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------

#include "ignorezerotransformer.h"
#include "libabbauw/libabbauw_properties_definitions.h"

#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasarguitie_propertytable.h>

namespace CSC_Batches
{
namespace viewConn
{

IgnoreZeroTransformer::IgnoreZeroTransformer( const basar::VarString& accPropTblCol, const basar::VarString& basarTblCol )
: m_AccPropTblCol( accPropTblCol ), m_GUIPropertyName( basarTblCol )
{
}

IgnoreZeroTransformer::~IgnoreZeroTransformer()
{
}

void IgnoreZeroTransformer::RightToLeft( basar::gui::tie::GuiPropertyTable_YIterator guiIt, 
										 const basar::db::aspect::AccessorPropertyTable_YIterator accIt )
{
    const bool isIntValue = ( accIt.getType( m_AccPropTblCol ).getType() == basar::INT32 &&  accIt.getInt32( m_AccPropTblCol ) == 0 ) 
                         || ( accIt.getType( m_AccPropTblCol ).getType() == basar::INT16 &&  accIt.getInt16( m_AccPropTblCol ) == 0 );

    if( isIntValue )
    {
        guiIt.setString( m_GUIPropertyName, "" );
    }
}

void IgnoreZeroTransformer::LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator guiIt, 
										 basar::db::aspect::AccessorPropertyTable_YIterator   accIt)
{
	// Nothing to be done here
    guiIt;
    accIt;
}

basar::gui::tie::Transformer::TransformSharedPtr IgnoreZeroTransformer::create()
{
	return basar::gui::tie::Transformer::TransformSharedPtr( new IgnoreZeroTransformer( *this ) );
}

} // namespace viewConn
} // namespace CSC_Batches
