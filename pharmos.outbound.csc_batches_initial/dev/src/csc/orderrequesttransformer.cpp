#include "orderrequesttransformer.h"
#include "libabbauw/libabbauw_properties_definitions.h"

#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasarguitie_propertytable.h>

#include <QtCore/QCoreApplication>

namespace CSC_Batches
{
namespace viewConn
{

OrderRequestTransformer::OrderRequestTransformer( const basar::VarString& GUIPropertyName, const basar::VarString& DBPropertyName )
: m_GUIPropertyName( GUIPropertyName ), m_DBPropertyName( DBPropertyName )
{
}

OrderRequestTransformer::~OrderRequestTransformer()
{
}

void OrderRequestTransformer::RightToLeft( basar::gui::tie::GuiPropertyTable_YIterator guiIt, 
										 const basar::db::aspect::AccessorPropertyTable_YIterator accIt )
{
    if( accIt.getInt32( m_DBPropertyName ) == 0 )
    {
        guiIt.setString( m_GUIPropertyName, "" );
    }
    else
    {
        guiIt.setString( m_GUIPropertyName, QCoreApplication::translate( "OrderRequestTransformer", "OR" ).toLocal8Bit().constData() );
    }
}

void OrderRequestTransformer::LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator guiIt, 
										 basar::db::aspect::AccessorPropertyTable_YIterator   accIt)
{
	// Nothing to be done here
    guiIt;
    accIt;
}

basar::gui::tie::Transformer::TransformSharedPtr OrderRequestTransformer::create()
{
	return basar::gui::tie::Transformer::TransformSharedPtr( new OrderRequestTransformer( *this ) );
}

} // namespace viewConn
} // namespace CSC_Batches
