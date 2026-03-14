//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer which marks next main order date red if severals conditions are fullfilled
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------

#ifndef GUARD_MAINORDERDATETRANSFORMER_H
#define GUARD_MAINORDERDATETRANSFORMER_H

#include <libbasarguitie_transformer.h>

class BasarTableWidget;

namespace CSC_Batches
{
namespace viewConn
{

class MainOrderDateTransformer : public basar::gui::tie::Transformer
{
public:
	~MainOrderDateTransformer();	
	MainOrderDateTransformer( const basar::VarString &, const basar::VarString &, BasarTableWidget* );

	void	RightToLeft	 ( basar::gui::tie::GuiPropertyTable_YIterator, 
		                   const basar::db::aspect::AccessorPropertyTable_YIterator );
	void	LeftToRight	 ( const basar::gui::tie::GuiPropertyTable_YIterator guiIt, 
		                   basar::db::aspect::AccessorPropertyTable_YIterator );

	basar::gui::tie::Transformer::TransformSharedPtr create();

private:
    // forbidden
	MainOrderDateTransformer();	

private:
	basar::VarString	                    m_AccPropTblCol;
	basar::VarString                        m_GUIPropertyName;
    BasarTableWidget*                       m_TableWidget;
};

} // namespace viewConn
} // namespace CSC_Batches

#endif // GUARD_MAINORDERDATETRANSFORMER_H
