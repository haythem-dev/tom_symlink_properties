//-------------------------------------------------------------------------------------------------//
/*! \file 
*   \brief ViewConnector for input of articles.
*   \author 
*   \date 
*/
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_CSC_Batches_VIEWCONN_ARTICLEFINDER_H
#define GUARD_CSC_Batches_VIEWCONN_ARTICLEFINDER_H

#include "iarticlefindervc.h"

#pragma warning (push)
#pragma warning(disable: 4127 4481 4512)
#include <QtWidgets/QDialog>

#include "ui_articlefindervc.h"
#pragma warning (pop)

#include <libbasarguitie_iviewconnector.h>
#include <libbasarguitie_matcher.h>
#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libutil/flag.h>

namespace CSC_Batches
{
namespace viewConn
{
class ArticleFinderVC : public QDialog, public IArticleFinderVC, public basar::gui::tie::IViewConnector
{
	Q_OBJECT

public:
	ArticleFinderVC( QWidget *parent = 0 );
	~ArticleFinderVC();

    // from IArticleFinderVC
	void                                                                init( basar::db::aspect::AccessorPropertyTableRef );
    basar::gui::tie::WidgetReturnEnum	                                show();
    void								                                hide();
    bool                                                                shutdown();

	void                                                                info( const basar::I18nString& );
    void                                                                error( const basar::I18nString& );
    const basar::gui::tie::MessageBoxButtonEnum                         question( const basar::I18nString& );

    void                                                                matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator );
	void                                                                matchSearchResults();
    void                                                                setMatchCode( const basar::VarString& matchCode );

private slots:
	void                                                                on_Close();
	void                                                                on_Ok();
	void                                                                on_Search();
	void	                                                            on_TextChangedArticleNo(const QString &newVal);
	void																on_TextChangedMatchCode(const QString &newVal);
	void																on_TextChangedEAN(const QString &newVal);
	void	                                                            on_SelectionChanged();
    void                                                                onTableDoubleClicked( QModelIndex );

private:
	void                                                                wireEvents();
	void								                                initSearchCriteria();
	void								                                initTable( QStringList& );
	void								                                initTitle();
	void								                                initHeaderLables( QStringList&, std::list<basar::VarString>& );
	void								                                initMatcher( std::list<basar::VarString>& );
	void                                                                clear();

private:
	Ui::articleFinder									                ui;
	basar::db::aspect::AccessorPropertyTable_YIterator	                m_SearchYit;
	basar::db::aspect::AccessorPropertyTableRef			                m_Results;

	basar::gui::tie::MatcherRef							                m_SearchResultMatcher;
};
}
}

#endif // GUARD_CSC_Batches_VIEWCONN_ARTICLEFINDER_H
