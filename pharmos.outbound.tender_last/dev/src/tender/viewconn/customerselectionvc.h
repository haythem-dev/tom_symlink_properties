#ifndef GUARD_TENDER_VIEWCONN_CUSTOMERSELECTIONVC_H
#define GUARD_TENDER_VIEWCONN_CUSTOMERSELECTIONVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_customerselectionvc.h"
#pragma warning (pop)


#include <libutil/flag.h>
#include <libutil/viewconn.h>

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include <QObject>
#pragma warning (pop)

#include "libtender/domainmodule/tender/tenderdm.h"
#include <libtender/domainmodule/tender/tendertypecollectionptr.h>


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
		class CustomerSelectionVC : public libutil::viewConn::BaseVC
		{
			Q_OBJECT;

			public:
				CustomerSelectionVC( QWidget* parent = 0 );
				~CustomerSelectionVC();

				virtual basar::gui::tie::WidgetReturnEnum show();

				void matchFromCustomerCollection(basar::db::aspect::AccessorPropertyTableRef propTab); 

				void setSelectedCustomer(basar::db::aspect::AccessorPropertyTable_YIterator selected);
				basar::db::aspect::AccessorPropertyTable_YIterator getSelectedCustomer();

				void displayEmptyCustomerList();

			private:
				void wireEvents();
				void setupCustomerTable();
				void initCustomerHeaderLabels();
				basar::Int32 initMatcher(basar::db::aspect::AccessorPropertyTableRef);
				void clear();

			private slots:
				void on_DialogClose_Requested();
				void on_TakeOver_Requested();

			protected:
                virtual void reject();

			private:
                Ui::CustomerSelectionVC                            ui;
                						                           
                libutil::misc::Flag                                m_EventsWiring;
                QStringList                                        m_LstLabels;
                std::list<basar::VarString>                        m_LstOriginalAttributes;
                basar::gui::tie::MatcherRef                        m_CustomerMatcher;
				basar::db::aspect::AccessorPropertyTable_YIterator m_PickedCustomer;
		};
	}
} 

#endif //end GUARD_TENDER_VIEWCONN_CUSTOMERSELECTIONVC_H
