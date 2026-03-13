#ifndef GUARD_TENDER_VIEWCONN_TENDERINVOICESVC_H
#define GUARD_TENDER_VIEWCONN_TENDERINVOICESVC_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
	#include "ui_tenderinvoicesvc.h"
	#include <QObject>
#pragma warning (pop)

//#include <libtender/domainmodule/tender/itenderptr.h>
//#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>
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
        class TenderInvoicesVC : public libutil::viewConn::BaseVC
        {
			Q_OBJECT

			public:
				TenderInvoicesVC( QWidget* parent = 0 );
                ~TenderInvoicesVC();

				void matchFromTenderProtocol( basar::db::aspect::AccessorPropertyTableRef protocol );

                virtual basar::gui::tie::WidgetReturnEnum show();

			private:
				void wireEvents();

			public slots:
				void onDialogCloseRequested();
				void onCreateInvoicesRequested();

			private:
				Ui::TenderInvoicesVC					ui;

                libutil::misc::Flag                      m_EventsWiring;

		};
	} //end namespace viewConn
} //end namespace tender
#endif //end GUARD_TENDER_VIEWCONN_TENDERINVOICESVC_H
