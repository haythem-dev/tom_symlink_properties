//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for colorful cells in table
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------

#include "tablecolortransformer.h"

#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasarguitie_propertytable.h>

#pragma warning (push)
#pragma warning(disable: 4512)
#include <libbasarqtwidget_tablewidget.h>
#pragma warning (pop)

#include "libabbauw/libabbauw_properties_definitions.h"
#include "trafficlightcolorsenum.h"

namespace CSC_Batches
{
namespace viewConn
{

TableColorTransformer::TableColorTransformer( const basar::VarString& accPropTblCol, const basar::VarString& basarTblCol, 
											  BasarTableWidget* table )
: m_AccPropTblCol( accPropTblCol ), m_BasarTblCol( 0 ), m_Table( table )
{
	for( int column = 0; column < m_Table->columnCount(); ++column, m_BasarTblCol++  )
    {
		if( basarTblCol == basar::VarString( m_Table->horizontalHeaderItem(column)->text().toLocal8Bit().data() ) )
			break;
    }
}

TableColorTransformer::~TableColorTransformer()
{
}

void TableColorTransformer::RightToLeft( basar::gui::tie::GuiPropertyTable_YIterator guiIt, 
										 const basar::db::aspect::AccessorPropertyTable_YIterator accIt )
{
	const basar::Int32 row   = guiIt.getPosition();
	const basar::Int16 color = accIt.getInt16( m_AccPropTblCol );
	QTableWidgetItem *item   = m_Table->item( row, m_BasarTblCol );

    if( item == NULL )
	{
		item = new QTableWidgetItem( "" );
		m_Table->setItem( row, m_BasarTblCol, item );
	}
	
	// for color codes see e.g. www.colorpicker.com
	if( (color & libabbauw::domMod::transferOrder::PURPLE) == libabbauw::domMod::transferOrder::PURPLE )
		item->setBackground( QColor("purple") );
	else if( (color & libabbauw::domMod::transferOrder::RED) == libabbauw::domMod::transferOrder::RED )
		item->setBackground( QColor(227, 25, 25) );
	else if( (color & libabbauw::domMod::transferOrder::YELLOW) == libabbauw::domMod::transferOrder::YELLOW )
		item->setBackground( QColor(235, 212, 43) );
	else if( (color & libabbauw::domMod::transferOrder::GREEN) == libabbauw::domMod::transferOrder::GREEN )
		item->setBackground( QColor(47, 176, 25) );

    // don't show amount "0" if all items are in stock -> leave amount empty instead
    if(  color == libabbauw::domMod::transferOrder::NO_COLOR || color == libabbauw::domMod::transferOrder::GREEN || 
        (color & libabbauw::domMod::transferOrder::PURPLE) == libabbauw::domMod::transferOrder::PURPLE )
        item->setText( "" );
}

void TableColorTransformer::LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator guiIt, 
										 basar::db::aspect::AccessorPropertyTable_YIterator   accIt)
{
	// Nothing to be done here
	guiIt;
	accIt;
}

basar::gui::tie::Transformer::TransformSharedPtr TableColorTransformer::create()
{
	return basar::gui::tie::Transformer::TransformSharedPtr( new TableColorTransformer( *this ) );
}

} // namespace viewConn
} // namespace CSC_Batches
