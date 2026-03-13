#include "rowcolourtransformer.h"
#include "libbasar_definitions.h"
#include <libutil/viewconn.h>

#pragma warning (push)
#pragma warning(disable: 4481)
#include <QtWidgets/QTableWidget>
#pragma warning (pop)

namespace tender
{
namespace viewConn
{
    RowColourTransformer::RowColourTransformer( const basar::VarString propertyName, basar::VarString columnName, QColor colour, QTableWidget* tableWidget )
    {
		m_PropertyName =	propertyName;
		m_ColumnName =		columnName;
		m_Colour =			colour;
        m_TableWidget =		tableWidget;

		m_WhiteColour =		QColor::fromRgb( 255, 255, 255 );
    }

    RowColourTransformer::~RowColourTransformer() 
    {
	}

    void RowColourTransformer::RightToLeft( basar::gui::tie::GuiPropertyTable_YIterator guiIt, const basar::db::aspect::AccessorPropertyTable_YIterator accIt )
    {
		if( 0 != accIt.getInt32(m_PropertyName) )
		{
			setBackgroundRow( guiIt.getPosition(), m_Colour );
		}
		else
		{
			setBackgroundRow( guiIt.getPosition(), m_WhiteColour );
		}
    }

    void RowColourTransformer::LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator guiIt, basar::db::aspect::AccessorPropertyTable_YIterator accIt )
    {
		if( "0" != guiIt.getString(m_ColumnName) )
		{
			setBackgroundRow( guiIt.getPosition(), m_Colour );
		}
		else
		{
			setBackgroundRow( guiIt.getPosition(), m_WhiteColour );
		}
    }

	boost::shared_ptr < basar::gui::tie::Transformer > RowColourTransformer::create()
    {
	    return boost::shared_ptr < basar::gui::tie::Transformer > ( new RowColourTransformer(*this) );
    }

	void RowColourTransformer::setBackgroundRow( int rowIndex, QColor colour )
	{
		QTableWidgetItem* item;
		libutil::gui::SignalBlocker signalblocker( m_TableWidget );
		//m_TableWidget->blockSignals( true );
		for (int colIndex = 0; colIndex < m_TableWidget->columnCount(); ++ colIndex)
		{
			item = m_TableWidget->item(rowIndex, colIndex);
			if (item)
			item->setBackground( colour );
		}
		//m_TableWidget->blockSignals( false );
		signalblocker.unblock();
	}

} //end namespace viewConn
} //end namespace tender
