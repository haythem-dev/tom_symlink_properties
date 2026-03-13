#ifndef GUARD_TENDER_VIEWCONN_PARTNERPICKERVC_H
#define GUARD_TENDER_VIEWCONN_PARTNERPICKERVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_partnerpickervc.h"
#pragma warning (pop)

#include <libutil/flag.h>
#include <libutil/viewconn.h>

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include <QObject>
#pragma warning (pop)

#include <libtender/domainmodule/tender/tendertypecollectionptr.h>

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
// class declaration setcion
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace viewConn
    {
        //-------------------------------------------------------------------------------------------------//
        // class declaration
        //-------------------------------------------------------------------------------------------------//
		class PartnerPickerVC : public libutil::viewConn::BaseVC
		{
			Q_OBJECT;

		    public:
				PartnerPickerVC( QWidget* parent = 0 );
				~PartnerPickerVC();

				void injectTenderTypeCollection( libtender::domMod::tender::TenderTypeCollectionPtr tenderTypeCollection );

				virtual basar::gui::tie::WidgetReturnEnum show();

				void matchFromPartnerCollection( basar::db::aspect::AccessorPropertyTableRef        propTab   );
				void matchToSearchCriteria     ( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch );

				basar::db::aspect::AccessorPropertyTable_YIterator getPickedPartner();
				void setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearchCriteria );

				void displayNoPartnerFound();

		    private:
				void wireEvents();
				void setupPartnerTable();
				void setupSearchCriteria();
				void initPartnerHeaderLabels();
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
                Ui::PartnerPickerVC                                ui;
                							                       
                libutil::misc::Flag                                m_EventsWiring;
                QStringList                                        m_LstLabels;
                std::list<basar::VarString>                        m_LstOriginalAttributes;
                basar::gui::tie::MatcherRef                        m_PartnerMatcher;
                basar::db::aspect::AccessorPropertyTable_YIterator m_PickedPartner;
				libtender::domMod::tender::TenderTypeCollectionPtr m_TenderTypeCollection;
		};
	} // end namespace viewConn
} // end namespace tender

#endif // end GUARD_TENDER_VIEWCONN_PARTNERPICKERVC_H
