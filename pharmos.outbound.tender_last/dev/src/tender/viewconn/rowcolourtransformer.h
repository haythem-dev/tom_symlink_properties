#ifndef GUARD_TENDER_VIEWCONN_ROWCOLOURTRANSFORMER_H
#define GUARD_TENDER_VIEWCONN_ROWCOLOURTRANSFORMER_H

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include <libutil/viewconn.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration
//-------------------------------------------------------------------------------------------------//
class QTableWidget;

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
    class RowColourTransformer : public basar::gui::tie::Transformer
    {
    public:
        RowColourTransformer( const basar::VarString propertyName, basar::VarString columnName, QColor colour, QTableWidget* tableWidget );
	    ~RowColourTransformer();

	    virtual void RightToLeft( basar::gui::tie::GuiPropertyTable_YIterator guiIt, const basar::db::aspect::AccessorPropertyTable_YIterator accIt );
	    virtual void LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator guiIt, basar::db::aspect::AccessorPropertyTable_YIterator accIt );
	    virtual boost::shared_ptr < basar::gui::tie::Transformer > create();

    private:

		void setBackgroundRow( int rowIndex, QColor colour );

		basar::VarString	m_PropertyName;
		basar::VarString	m_ColumnName;
		QColor				m_Colour;
        QTableWidget*		m_TableWidget;

		QColor				m_WhiteColour;
    };
} // namespace viewConn
} // namespace tender

#endif //GUARD_TENDER_VIEWCONN_ROWCOLOURTRANSFORMER_H