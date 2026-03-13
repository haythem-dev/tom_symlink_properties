#ifndef GUARD_TENDER_VIEWCONN_CUSTOMERPICKERVC_H
#define GUARD_TENDER_VIEWCONN_CUSTOMERPICKERVC_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_customerpickervc.h"
#include <QObject>
#pragma warning (pop)

#include <libutil/flag.h>
#include <libutil/viewconn.h>


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
		class CustomerPickerVC : public libutil::viewConn::BaseVC
		{
			Q_OBJECT;

		    public:
				CustomerPickerVC( QWidget* parent = 0 );
				~CustomerPickerVC();

				virtual basar::gui::tie::WidgetReturnEnum show();

				void matchFromCustomerCollection( basar::db::aspect::AccessorPropertyTableRef        propTab   );
				void matchToSearchCriteria      ( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch );

				basar::db::aspect::AccessorPropertyTable_YIterator getPickedCustomer();
				void setCustomerNo( basar::db::aspect::AccessorPropertyTable_YIterator yit );

				void displayNoCustomerFound();

		    private:
				void wireEvents();
				void setupCustomerTable();
				void setupSearchCriteria();
				void initCustomerHeaderLabels();
				basar::Int32 initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab );
				void clear();
			
			private slots:
				void on_DialogClose_Requested();
				void on_Search_Requested();
				void on_TakeOver_Requested();

		    protected:
                virtual void focusInEvent( QFocusEvent* event );
				virtual void reject();

			private:
                Ui::CustomerPickerVC                               ui;
                							                       
                libutil::misc::Flag                                m_EventsWiring;
                QStringList                                        m_LstLabels;
                std::list<basar::VarString>                        m_LstOriginalAttributes;
                basar::gui::tie::MatcherRef                        m_CustomerMatcher;
                basar::db::aspect::AccessorPropertyTable_YIterator m_PickedCustomer;
		};
	} // end namespace viewConn
} // end namespace tender

#endif // end GUARD_TENDER_VIEWCONN_CUSTOMERPICKERVC_H
