#ifndef GUARD_TENDER_VIEWCONN_TENDERPROTOCOLVC_H
#define GUARD_TENDER_VIEWCONN_TENDERPROTOCOLVC_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
	#include "ui_tenderprotocolvc.h"
	#include <QObject>
#pragma warning (pop)

#include <libtender/domainmodule/tender/itenderprotocolptr.h>
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
        class TenderProtocolVC : public libutil::viewConn::BaseVC
        {
			Q_OBJECT

			public:
				TenderProtocolVC( QWidget* parent = 0 );
                ~TenderProtocolVC();

                virtual basar::gui::tie::WidgetReturnEnum show();

				void matchFromTenderProtocol( basar::db::aspect::AccessorPropertyTableRef protocol );
				void matchToTenderProtocol( basar::db::aspect::AccessorPropertyTableRef protocol );

				int showInvoicedOrders();
				basar::db::aspect::AccessorPropertyTable_YIterator getCurrentOrder( basar::db::aspect::AccessorPropertyTableRef positionPropTab );

				void setQuantityToCharge( basar::db::aspect::AccessorPropertyTableRef protocols );

				void displayNoProtocolFound();
				void displayQuantityExceedance( int maxQuantity );
				void displayQuantityToChargeNotSet();


			private:
				void wireEvents();
                void setupProtocolTable();
                void initProtocolHeaderLabels();
				basar::Int32 initProtocolMatcher( basar::db::aspect::AccessorPropertyTableRef propTab );


			public slots:
				void onDialogCloseRequested();
				void onQuantityToChargeChanged( int row, int column  );
				void onQuantityToChargeChanged(QTableWidgetItem * item);
				void onCreateInvoiceRequested();
				void onShowCriteriaChanged( int );
				void onDoubleClickOrder();


			private:
				Ui::TenderProtocolVC			ui;

                libutil::misc::Flag				m_EventsWiring;
                QStringList						m_LstLabels;
                std::list<basar::VarString>		m_LstOriginalAttributes;
				basar::gui::tie::MatcherRef		m_ProtocolMatcher;

				static const int COLUMN_QUANTITY_TO_CHARGE = 7;
				QColor			 m_rowColour;
		};
	} //end namespace viewConn
} //end namespace tender
#endif //end GUARD_TENDER_VIEWCONN_TENDERPROTOCOLVC_H
