//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for colorful cells in table
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------

#ifndef GUARD_TABLE_COLOR_TRANSFORMER_H
#define GUARD_TABLE_COLOR_TRANSFORMER_H

#include <libbasarguitie_transformer.h>

class BasarTableWidget;

namespace CSC_Batches
{
namespace viewConn
{

class TableColorTransformer : public basar::gui::tie::Transformer
{
public:
	~TableColorTransformer();	
	TableColorTransformer( const basar::VarString &, const basar::VarString &, BasarTableWidget*  );

	void	RightToLeft	 ( basar::gui::tie::GuiPropertyTable_YIterator, 
		                   const basar::db::aspect::AccessorPropertyTable_YIterator );
	void	LeftToRight	 ( const basar::gui::tie::GuiPropertyTable_YIterator guiIt, 
		                   basar::db::aspect::AccessorPropertyTable_YIterator );

	basar::gui::tie::Transformer::TransformSharedPtr create();

private:
	basar::VarString	                    m_AccPropTblCol;
	basar::Int32                            m_BasarTblCol;
	BasarTableWidget*                       m_Table;

private:
	TableColorTransformer();
};

} // namespace viewConn
} // namespace CSC_Batches
#endif // GUARD_TABLE_COLOR_TRANSFORMER_H
