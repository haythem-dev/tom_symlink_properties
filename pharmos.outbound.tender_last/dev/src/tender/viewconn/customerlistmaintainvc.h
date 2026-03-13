#ifndef GUARD_TENDER_VIEWCONN_CUSTOMERLISTMAINTAINVC_H
#define GUARD_TENDER_VIEWCONN_CUSTOMERLISTMAINTAINVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_customerlistmaintainvc.h"
#pragma warning (pop)

#include "libtender/domainmodule/customerlist/icustomerlistdmptr.h"

#include <libutil/flag.h>
#include <libutil/viewconn.h>

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include <QObject>
#include <QAction>
#pragma warning (pop)

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

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace viewConn
    {
        //-------------------------------------------------------------------------------------------------//
        // class declaration
        //-------------------------------------------------------------------------------------------------//
        class CustomerListMaintainVC : public libutil::viewConn::BaseVC
        {
            Q_OBJECT

            public:
                CustomerListMaintainVC( QWidget* parent = 0 );
                ~CustomerListMaintainVC();

                virtual basar::gui::tie::WidgetReturnEnum show();
				
				void matchToCustomerList( basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer );
                void matchFromCustomerList( libtender::domMod::customerlist::ICustomerListDMPtr customerList   );

				void matchToCustomer(  basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer );
                void matchFromCustomer( basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer );
				void matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer );

                basar::db::aspect::AccessorPropertyTable_YIterator getSelectedCustomer( basar::db::aspect::AccessorPropertyTableRef positionPropTab );

				void initialize();
				void resetCustomer();

				int getCustomerNoToBeAdded();
				int getBranchNo();

				void displayCustomerAlreadySelected();
				void displayInvalidCustomer();

            private:
                void wireEvents();
                void setupTable();
				void initHeaderLabels();
                basar::Int32 initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab );

				bool isCustomerAlreadySelected();

            private slots:
				void onAddCustomerRequested();
				void onEditCustomerRequested();
				void onDeleteCustomerRequested();
				void onSearchCustomerRequested();
				void onCheckCustomerRequested();
				void onCancelRequested();


            private:
                Ui::CustomerListMaintainVC			ui;

                libutil::misc::Flag					m_EventsWiring;
                QStringList							m_LstLabels;
                std::list<basar::VarString>			m_LstOriginalAttributes;
                basar::gui::tie::MatcherRef			m_CustomerCollectionMatcher;

				QAction *							m_StartSearchAction;
        };
    }
}

#endif //end GUARD_TENDER_VIEWCONN_CUSTOMERLISTMAINTAINVC_H
