#include "mainorderdatetransformer.h"

#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasarguitie_propertytable.h>
#include <libutil/misc/datetime/datetime.h>

#pragma warning (push)
#pragma warning(disable: 4512 4800)
#include <libbasarqtwidget_tablewidget.h>
#pragma warning (pop)

namespace CSC_Batches
{
namespace viewConn
{

MainOrderDateTransformer::MainOrderDateTransformer( const basar::VarString& accPropTblCol, const basar::VarString& guiPropTblCol, 
											        BasarTableWidget* table )
: m_AccPropTblCol( accPropTblCol ), m_GUIPropertyName( guiPropTblCol ), m_TableWidget( table )
{
}

MainOrderDateTransformer::~MainOrderDateTransformer()
{
}

void MainOrderDateTransformer::RightToLeft( basar::gui::tie::GuiPropertyTable_YIterator              guiIt, 
                                            const basar::db::aspect::AccessorPropertyTable_YIterator accIt )
{
	const basar::Date fdate    = accIt.getDate( m_AccPropTblCol );
    const basar::Int32 column  = static_cast<basar::Int32>(guiIt.getIndex( m_GUIPropertyName ));
    const basar::Int32 row     = guiIt.getPosition();

	QTableWidgetItem *item     = m_TableWidget->item( row, column );
    if( item != NULL )
    {
        libutil::misc::DateTime date( fdate ), today( basar::cmnutil::Date::getCurrent() );   
        libutil::misc::DateTime deadline( basar::cmnutil::Date::getCurrent() ); deadline.addDays( 15 );
        if( date < today || date >= deadline )
        {
            QFont font = item->font();
            font.setBold( true );
            item->setFont( font );
            //item->setTextColor( QColor(227, 25, 25) ); // dark red
            item->setForeground( QColor(227, 25, 25) ); // dark red
        }
    }
}

void MainOrderDateTransformer::LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator  guiIt, 
										           basar::db::aspect::AccessorPropertyTable_YIterator accIt)
{
	// Nothing to be done here
    guiIt;
    accIt;
}

basar::gui::tie::Transformer::TransformSharedPtr MainOrderDateTransformer::create()
{
	return basar::gui::tie::Transformer::TransformSharedPtr( new MainOrderDateTransformer( *this ) );
}

} // namespace viewConn
} // namespace CSC_Batches
