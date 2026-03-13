#ifndef GUARD_TENDER_VIEWCONN_TENDEROVERVIEWVC_H
#define GUARD_TENDER_VIEWCONN_TENDEROVERVIEWVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_tenderoverviewvc.h"
#pragma warning (pop)

//#include "itendercontractinfoptr.h" //srk
#include "itendersearchcriteriaptr.h"

#include <libutil/flag.h>
#include <libutil/viewconn.h>

//#pragma warning (push)
//#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
//#include <QObject>
//#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//
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
        class ITenderSearchCriteria;
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
        class TenderOverviewVC : public libutil::viewConn::BaseVC
        {
            Q_OBJECT

            public:
                TenderOverviewVC( QWidget* parent = 0 );
                ~TenderOverviewVC();

                virtual basar::gui::tie::WidgetReturnEnum   show();

                void matchFromTenderCollection( basar::db::aspect::AccessorPropertyTableRef        propTab   );
                void matchToSearchCriteria    ( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch );

                void injectSearchCriteria     ( tender::viewConn::ITenderSearchCriteriaPtr searchCriteria);

                void setPharmacyGroupId       ( basar::db::aspect::AccessorPropertyTable_YIterator yit );
				void setArticleCode           ( basar::db::aspect::AccessorPropertyTable_YIterator yit );
				void setCustomerNo            ( basar::db::aspect::AccessorPropertyTable_YIterator yit );
				void setPartnerNo             ( basar::db::aspect::AccessorPropertyTable_YIterator yit );
				void setSupplierNo            ( basar::db::aspect::AccessorPropertyTable_YIterator yit );
				void setPharmacyChainNo		  ( basar::db::aspect::AccessorPropertyTable_YIterator yit );

				void setArea( basar::Int16 area );

                basar::db::aspect::AccessorPropertyTable_YIterator getCurrentTender();
				void selectTender( basar::db::aspect::AccessorPropertyTable_YIterator tenderYit );

				void initialize();

                void displayNoTenderFound();
				void displayDeleteNotPossible(basar::VarString message);
                void displayTenderStateWarning(basar::VarString warning);
				void displayEndDateSetToLastDateOfTheYear();
				basar::gui::tie::MessageBoxButtonEnum displayCloneDeliveriesQuestion();
				basar::gui::tie::MessageBoxButtonEnum displayOrderProposalAlreadyCreated();

				void reset();
                //srk
                //void resetPharmacyGroup(); 
                //void checkContractInfo();
                //void matchToPharmacyGroupSearch( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyGroupSearchYit ) const;
                //void matchFromPharmacyGroup( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyGroup ); 
            private:

                void wireEvents();
                void setupTenderTable();
				void initTenderHeaderLabelsFR();
				void initTenderHeaderLabelsRS();
				void initTenderHeaderLabelsBG();
                void initTenderHeaderLabelsCH();
                basar::Int32 initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab );

				void selectRowByAbsoluteIndex(BasarTableWidget* tabWidget, basar::gui::tie::MatcherRef matcher, int absoluteIndex);


            private slots:

                void onDialogClose_Requested();
                void onEditTender_Requested ();
                void onNewTender_Requested ();
                void onTenderSelected();
                void onReset_Requested();
                void onDeleteTender_Requested();
				void onCloneTender_Requested();

            protected:
                virtual void reject();

            private:

                Ui::TenderOverviewVC ui;

                //tender::viewConn::ITenderContractInfo*   m_uiContractInfo; //srk
                tender::viewConn::ITenderSearchCriteria* m_uiSearchCriteria;
                libutil::misc::Flag                      m_EventsWiring;
                QStringList                              m_LstLabels;
                std::list<basar::VarString>              m_LstOriginalAttributes;
                basar::gui::tie::MatcherRef              m_TenderCollectionMatcher;

				basar::Int16							 m_area;
        };
    }
}

#endif //end GUARD_TENDER_VIEWCONN_TENDEROVERVIEWVC_H
