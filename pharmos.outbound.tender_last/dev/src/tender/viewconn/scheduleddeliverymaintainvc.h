#ifndef GUARD_TENDER_VIEWCONN_SCHEDULEDDELIVERYMAINTAINVC_H
#define GUARD_TENDER_VIEWCONN_SCHEDULEDDELIVERYMAINTAINVC_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
	#include "ui_scheduleddeliverymaintain.h"
	#include <QObject>
#pragma warning (pop)

#include <libtender/domainmodule/tender/itenderptr.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>
#include <libutil/flag.h>
#include <libutil/viewconn.h>
#include "ischeduleddeliverymaintain.h"

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

//-------------------------------------------------------------------------------------------------//
// class declaration setcion
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace viewConn
    {
        //-------------------------------------------------------------------------------------------------//
        // class declaration
        //-------------------------------------------------------------------------------------------------//
        class ScheduledDeliveryMaintainVC : public IScheduledDeliveryMaintain
        {
			Q_OBJECT

			public:
				ScheduledDeliveryMaintainVC( QWidget* parent = 0 );
                ~ScheduledDeliveryMaintainVC();

				void setPOPFunctionality( basar::I18nString popFunctionality );
                void setRegion(basar::Int16 region);
                void setCustomer(basar::db::aspect::AccessorPropertyTable_YIterator);
				void setNeedsCustomerSelection(bool needsCustomerSelection);

                virtual basar::gui::tie::WidgetReturnEnum show();

				void matchFromScheduledDelivery	( libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery );
				void matchToScheduledDelivery	( libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery );
				void matchFromTender			( libtender::domMod::tender::ITenderPtr tender );
				void matchFromPositionCollection( basar::db::aspect::AccessorPropertyTableRef positionPropTab );
				void matchToPositionCollection  ( basar::db::aspect::AccessorPropertyTableRef positionPropTab );

				void displayDeliveryAlreadyExists();
				void displayInvalidDeliveryDate();
				void displayQuantityExceedance( int maxQuantity );
				void displayPositionHasNoAvailableQuantity();
				void displayBranchNoNotValid();

				basar::db::aspect::AccessorPropertyTable_YIterator getCurrentAvailableArticle( basar::db::aspect::AccessorPropertyTableRef positionPropTab );
				basar::db::aspect::AccessorPropertyTable_YIterator getCurrentSelectedArticle( basar::db::aspect::AccessorPropertyTableRef positionPropTab );
				basar::Int32 getCustomer();

				void clear();

			private:
				void wireEvents();
                void setupDeliveryTables();
                void initDeliveryHeaderLabels();
				bool isArticleSelected();
				bool isArticleSelected( int row );
				bool isSavable();
				bool areQuantitiesSet();
				void setBranchNoCellsEditable( int row );
				void displayCustomerNotSelected();
				void displayArticleAlreadySelected();
				void displayNoArticlesSelected();
				void displayQuantitiesNotSet();
				basar::Int32 initAvailableItemsMatcher( basar::db::aspect::AccessorPropertyTableRef propTab );
				 /*!
				  * /return selected items count (without marked for delete!)
				  */
				basar::Int32 initSelectedItemsMatcher ( basar::db::aspect::AccessorPropertyTableRef propTab );
				

			public slots:
				void onSaveRequested();
				void onDialogCloseRequested();
				void onSelectItemRequested();
				void onDeselectItemRequested();
				void onSelectAllRequested();
				void onDeselectAllRequested();
				void onSelectedItemsCellChanged( int, int );
                void onSelectCustomerRequested();

			protected:


			private:
				Ui::ScheduledDileveryMaintainVC          ui;

                libutil::misc::Flag                      m_EventsWiring;
                QStringList                              m_LstLabelsAvailableItems;
                std::list<basar::VarString>              m_LstOriginalAttributesAvailableItems;
				QStringList                              m_LstLabelsSelectedItems;
                std::list<basar::VarString>              m_LstOriginalAttributesSelectedItems;
				basar::gui::tie::MatcherRef              m_AvailableItemsMatcher;
				basar::gui::tie::MatcherRef              m_SelectedItemsMatcher;

				bool									 m_POPFunctionality;
				QColor									 m_ColumnColour;
				static const int						 COLUMN_POP			= 2;
				static const int						 COLUMN_BRANCHNOPOP = 3;

                basar::Int16                             m_Region;
				bool								     m_NeedsCustomerSelection;

		};
	} //end namespace viewConn
} //end namespace tender
#endif //end GUARD_TENDER_VIEWCONN_SCHEDULEDDELIVERYMAINTAINVC_H
