//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer which ignores 0 in integer colums
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------

#ifndef GUARD_IGNORE_ZERO_TRANSFORMER_H
#define GUARD_IGNORE_ZERO_TRANSFORMER_H

#include <libbasarguitie_transformer.h>

namespace CSC_Batches
{
namespace viewConn
{

class IgnoreZeroTransformer : public basar::gui::tie::Transformer
{
public:
	~IgnoreZeroTransformer();	
	IgnoreZeroTransformer( const basar::VarString &, const basar::VarString &  );

	void	RightToLeft	 ( basar::gui::tie::GuiPropertyTable_YIterator, 
		                   const basar::db::aspect::AccessorPropertyTable_YIterator );
	void	LeftToRight	 ( const basar::gui::tie::GuiPropertyTable_YIterator guiIt, 
		                   basar::db::aspect::AccessorPropertyTable_YIterator );

	basar::gui::tie::Transformer::TransformSharedPtr create();

private:
	basar::VarString	                    m_AccPropTblCol;
	basar::VarString                        m_GUIPropertyName;

private:
	IgnoreZeroTransformer();
};

} // namespace viewConn
} // namespace CSC_Batches
#endif // GUARD_IGNORE_ZERO_TRANSFORMER_H
