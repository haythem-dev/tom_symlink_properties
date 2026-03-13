//-------------------------------------------------------------------------------------------------//
/*! \file 
*   \brief ViewConnector for input of articles.
*   \author 
*   \date 
*/
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_CSC_BATCHES_VIEWCONN_CUSTOMERFINDER_H
#define GUARD_CSC_BATCHES_VIEWCONN_CUSTOMERFINDER_H


#include "icustomerfindervc.h"
#include <libbasarguitie_iviewconnector.h>

#pragma warning (push)
#pragma warning(disable: 4127 4481 4512)
#include <QtWidgets/QDialog>
#include "ui_customerfindervc.h"
#pragma warning (pop)

#include <libbasarguitie_matcher.h>
#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libutil/flag.h>

namespace CSC_Batches
{
namespace viewConn
{
class CustomerFinderVC : public QDialog, public ICustomerFinderVC, public basar::gui::tie::IViewConnector
{
	Q_OBJECT

public:
	CustomerFinderVC( QWidget *parent = 0 );
	~CustomerFinderVC();

    // from ICustomerFinderVC
	void                                                                init( basar::db::aspect::AccessorPropertyTableRef );
    basar::gui::tie::WidgetReturnEnum	                                show();
    void								                                hide();
    bool                                                                shutdown();

	void                                                                info( const basar::I18nString& );
    void                                                                error( const basar::I18nString& );
    const basar::gui::tie::MessageBoxButtonEnum                         question( const basar::I18nString& );

    void                                                                matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator );
	void                                                                matchSearchResults();

private slots:
	void                                                                on_Close();
	void                                                                on_Ok();
	void                                                                on_Search();	
	void	                                                            on_TextChangedCustomerNo(const QString &newVal);
	void																on_TextChangedMatchCode(const QString &newVal);
	void	                                                            on_SelectionChanged();
    void                                                                onTableDoubleClicked( QModelIndex );

private:
	void                                                                wireEvents();
	void								                                initSearchCriteria();
	void								                                initTable( QStringList& );
	void								                                initTitle();
	void								                                initHeaderLables( QStringList&, std::list<basar::VarString>& );
	void								                                initMatcher( std::list<basar::VarString>& );
	void																clear();

private:
	Ui::frCustomerFinder								                ui;
	basar::db::aspect::AccessorPropertyTable_YIterator	                m_SearchYit;
	basar::db::aspect::AccessorPropertyTableRef			                m_Results;

	basar::gui::tie::MatcherRef							                m_SearchResultMatcher;
};
}
}

#endif // GUARD_CSC_BATCHES_VIEWCONN_CUSTOMERFINDER_H
