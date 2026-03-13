#ifndef GUARD_TENDER_VIEWCONN_CSCORDERMAINTAINVC_H
#define GUARD_TENDER_VIEWCONN_CSCORDERMAINTAINVC_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
    #include "ui_cscordermaintainvc.h"
    #include <QObject>
#pragma warning (pop)

#include <libtender/domainmodule/tender/itenderptr.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>
#include <libutil/flag.h>
#include <libutil/viewconn.h>
#include "icscordermaintain.h"

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
class CSCOrderMaintainVC : public ICSCOrderMaintain
{
	Q_OBJECT
            
public:
	CSCOrderMaintainVC( QWidget* parent = 0 );
	virtual ~CSCOrderMaintainVC();

	virtual basar::gui::tie::WidgetReturnEnum show();

	virtual void matchFromTender( libtender::domMod::tender::ITenderPtr tender );
	virtual void matchFromOrderPositions( basar::db::aspect::AccessorPropertyTableRef positionPropTab );

	virtual void displayQuantityExceedance( int maxQuantity );
	virtual void displayPositionHasNoAvailableQuantity();
	virtual void displayCscOrderCreationDetails( const std::string orderInfo );

	virtual basar::db::aspect::AccessorPropertyTable_YIterator getCurrentTenderPosition( basar::db::aspect::AccessorPropertyTableRef positionPropTab );
	virtual basar::db::aspect::AccessorPropertyTable_YIterator getCurrentOrderPosition( basar::db::aspect::AccessorPropertyTableRef positionPropTab );

public slots:
	void onCreateCSCOrderRequested();
	void onDialogCloseRequested();
	void onSelectItemRequested();
	void onSelectedItemsCellChanged( int, int );

private:
	void wireEvents();
	void setupTableWidgets();
	void initTableHeaderLabels();
	bool isArticleSelected();
	bool isArticleSelected( int row );
	bool isSavable();
	bool areQuantitiesSet();
	void displayArticleAlreadySelected();
	void displayNoArticlesSelected();
	void displayQuantitiesNotSet();
	basar::Int32 initTenderPositionsMatcher( basar::db::aspect::AccessorPropertyTableRef propTab );
	basar::Int32 initOrderPositionsMatcher ( basar::db::aspect::AccessorPropertyTableRef propTab );
                
	Ui::CSCOrderMaintenance			ui;

	libutil::misc::Flag            m_EventsWiring;
	QStringList                    m_LstLabelsTenderPositions;
	std::list<basar::VarString>    m_LstOriginalAttributesTenderPositions;
	QStringList                    m_LstLabelsOrderPositions;
	std::list<basar::VarString>    m_LstOriginalAttributesOrderPositions;
	basar::gui::tie::MatcherRef    m_TenderPositionsMatcher;
	basar::gui::tie::MatcherRef    m_OrderPositionsMatcher;

	QColor                         m_ColumnColour;
	static const int               COLUMN_QUANTITY = 0;

};

} // end namespace viewConn
} // end namespace tender

#endif //end GUARD_TENDER_VIEWCONN_CSCORDERMAINTAINVC_H
