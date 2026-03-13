#ifndef GUARD_TENDER_VIEWCONN_TENDERSEARCHCRITERIABULGARIA_H
#define GUARD_TENDER_VIEWCONN_TENDERSEARCHCRITERIABULGARIA_H

#include "Itendersearchcriteria.h"
#include <libutil/viewconn/qmessageboxext.h>

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_tendersearchcriteriabulgaria.h"
#include <QObject>
#include <QAction>
#pragma warning (pop)

#include <libutil/flag.h>


namespace tender
{
    namespace viewConn
    {
        class TenderSearchCriteriaBulgaria : public ITenderSearchCriteria
        {
            Q_OBJECT

            public:
                TenderSearchCriteriaBulgaria();
                ~TenderSearchCriteriaBulgaria();

                virtual void matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch );
                virtual void prepareForNewSearch();
                
                virtual void injectStatusMapping( libtender::domMod::tender::TenderStateCollectionPtr statusMapping );
                virtual void injectTypeMapping( libtender::domMod::tender::TenderTypeCollectionPtr typeMapping );
                
                virtual void setArticleCode( basar::db::aspect::AccessorPropertyTable_YIterator yit );
                virtual void setCustomerNo ( basar::db::aspect::AccessorPropertyTable_YIterator yit );
                
                virtual const libtender::domMod::tender::TenderTypeCollectionPtr getTypeMapping();
                virtual const libtender::domMod::tender::TenderStateCollectionPtr getStatusMapping();

            private slots:
                void onSearchClicked();
                void onArticlePickerClicked();
                void onCustomerPickerClicked();
                void onShortcutTriggered();
                
                void onContractPeriodClicked( bool isChecked );
                void onContractDateChanged();

            protected:
                virtual void focusInEvent ( QFocusEvent* event );
                virtual void focusOutEvent( QFocusEvent* event );

            private:
                void wireEvents();
                void setupSearchCriteria();
                void resetAllInputFields();

            private:

                Ui::TenderSearchCriteriaBulgaria					ui;
                libutil::misc::Flag									m_EventsWiring;
                libtender::domMod::tender::TenderStateCollectionPtr m_statusMapping;
                libtender::domMod::tender::TenderTypeCollectionPtr	m_typeMapping;
                
                basar::Int16										m_area;
        };
    }
}

#endif //end GUARD_TENDER_VIEWCONN_TENDERSEARCHCRITERIABULGARIA_H
