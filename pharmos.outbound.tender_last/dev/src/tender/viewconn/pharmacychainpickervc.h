#ifndef GUARD_TENDER_VIEWCONN_PHARMACYCHAINPICKERVC_H
#define GUARD_TENDER_VIEWCONN_PHARMACYCHAINPICKERVC_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_pharmacychainpickervc.h"
#include <QObject>
#pragma warning (pop)

#include "libtender/domainmodule/tender/tenderdm.h"

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
		class PharmacyChainPickerVC : public libutil::viewConn::BaseVC
		{
			Q_OBJECT;

			public:
				PharmacyChainPickerVC( QWidget* parent = 0 );
				~PharmacyChainPickerVC();

				virtual basar::gui::tie::WidgetReturnEnum show();

				void matchFromPharmacyChainCollection( basar::db::aspect::AccessorPropertyTableRef        propTab   ); 
				void matchToSearchCriteria			 ( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch );

				basar::db::aspect::AccessorPropertyTable_YIterator getPickedPharmacyChain();
				void setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit );

				void displayNoPharmacyChainFound();

			private:
				void wireEvents();
				void setupPharmacyChainTable();
				void setupSearchCriteria();
				void initPharmacyChainHeaderLabels();
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
				Ui::PharmacyChainPickerVC                          ui;
                						                           
                libutil::misc::Flag                                m_EventsWiring;
                QStringList                                        m_LstLabels;
                std::list<basar::VarString>                        m_LstOriginalAttributes;
                basar::gui::tie::MatcherRef                        m_PharmacyChainMatcher;
				basar::db::aspect::AccessorPropertyTable_YIterator m_PickedPharmacyChain;
		};
	} //end namespace viewConn
} //end namespace tender

#endif //end GUARD_TENDER_VIEWCONN_PHARMACYCHAINPICKERVC_H
