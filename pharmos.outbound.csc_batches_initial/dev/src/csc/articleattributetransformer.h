//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for colorful cells in table
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------

#ifndef GUARD_CSC_BATCHES_VIEWCONN_ARTICLE_ATTRIBUTETRANSFORMER_H
#define GUARD_CSC_BATCHES_VIEWCONN_ARTICLE_ATTRIBUTETRANSFORMER_H

#include <libbasarguitie_transformer.h>

#pragma warning (push)
#pragma warning(disable: 4512)
#include <QtGui/QIcon>
#pragma warning (pop)

class BasarTableWidget;

namespace CSC_Batches
{
namespace viewConn
{

class ArticleAttributeTransformer : public basar::gui::tie::Transformer
{
public:
	~ArticleAttributeTransformer();	
	ArticleAttributeTransformer( const basar::VarString &, const basar::VarString &, BasarTableWidget* );

	void	RightToLeft( basar::gui::tie::GuiPropertyTable_YIterator, 
		                 const basar::db::aspect::AccessorPropertyTable_YIterator );
	void	LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator guiIt, 
		                 basar::db::aspect::AccessorPropertyTable_YIterator );

	basar::gui::tie::Transformer::TransformSharedPtr create();

private:
	ArticleAttributeTransformer();

private:
	basar::VarString	                    m_AccPropTblCol;
	basar::VarString                        m_GuiPropTblCol;
	BasarTableWidget*                       m_Table;
};

} // namespace viewConn
} // namespace CSC_Batches
#endif // GUARD_CSC_BATCHES_VIEWCONN_ARTICLE_ATTRIBUTETRANSFORMER_H
