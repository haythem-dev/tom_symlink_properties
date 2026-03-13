//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for colorful cells in table
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------

#include "articleattributetransformer.h"

#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasarguitie_propertytable.h>

#pragma warning (push)
#pragma warning(disable: 4512 4800)
#include <libbasarqtwidget_tablewidget.h>
#include <QtGui/QPainter>
#pragma warning (pop)

namespace CSC_Batches
{
namespace viewConn
{

ArticleAttributeTransformer::ArticleAttributeTransformer( const basar::VarString& accPropTblCol, const basar::VarString& guiPropTblCol, 
											  BasarTableWidget* table )
: m_AccPropTblCol( accPropTblCol ), m_GuiPropTblCol( guiPropTblCol ), m_Table( table )
{
}

ArticleAttributeTransformer::~ArticleAttributeTransformer()
{
}

void ArticleAttributeTransformer::RightToLeft( basar::gui::tie::GuiPropertyTable_YIterator              guiIt, 
										       const basar::db::aspect::AccessorPropertyTable_YIterator accIt )
{
	const basar::VarString tag = accIt.getString( m_AccPropTblCol );
    const basar::Int32 column  = static_cast<basar::Int32>(guiIt.getIndex( m_GuiPropTblCol ));
    const basar::Int32 row     = guiIt.getPosition();
	QTableWidgetItem *item     = m_Table->item( row, column );

	if( item == NULL )
	{
		item = new QTableWidgetItem( "" );
		m_Table->setItem( row, column, item );
	}

    QFont font = item->font();
    font.setBold( true );
    font.setHintingPreference( QFont::PreferDefaultHinting );
    font.setPointSize( 10 ); // default is 8
    item->setFont( font );

    // TODO remove string literals N and K
    if( tag == "N" )
    {
        item->setForeground( QBrush( QColor(227, 25, 25) ) );
        item->setText( "BTM" );
    }
    else if( tag == "K" )
    {
        item->setForeground( QBrush( QColor(47, 176, 25) ) );
        item->setText( "K" );
    }
    else
    {
        item->setText( "" );
    }
}

void ArticleAttributeTransformer::LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator  guiIt, 
										           basar::db::aspect::AccessorPropertyTable_YIterator accIt)
{
	// Nothing to be done here
    guiIt;
    accIt;
}

basar::gui::tie::Transformer::TransformSharedPtr ArticleAttributeTransformer::create()
{
	return basar::gui::tie::Transformer::TransformSharedPtr( new ArticleAttributeTransformer( *this ) );
}

} // namespace viewConn
} // namespace CSC_Batches
