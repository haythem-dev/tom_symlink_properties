#ifndef GUARD_TENDER_VIEWCONN_TENDERSEARCHCRITERIASERBIA_H
#define GUARD_TENDER_VIEWCONN_TENDERSEARCHCRITERIASERBIA_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#include "Itendersearchcriteria.h"
#include <libutil/viewconn/qmessageboxext.h>

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_tendersearchcriteriaserbia.h"
#include <QObject>
#include <QAction>
#pragma warning (pop)

#include <libutil/flag.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//

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
        class TenderSearchCriteriaSerbia : public ITenderSearchCriteria
        {
            Q_OBJECT

            public:
                TenderSearchCriteriaSerbia();
                ~TenderSearchCriteriaSerbia();

                virtual void matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch );
                virtual void prepareForNewSearch();
                
                virtual void injectStatusMapping( libtender::domMod::tender::TenderStateCollectionPtr statusMapping );
                virtual void injectTypeMapping( libtender::domMod::tender::TenderTypeCollectionPtr typeMapping );
                
                virtual void setArticleCode( basar::db::aspect::AccessorPropertyTable_YIterator yit );
                virtual void setCustomerNo ( basar::db::aspect::AccessorPropertyTable_YIterator yit );
                virtual void setPharmacyChainNo ( basar::db::aspect::AccessorPropertyTable_YIterator yit );

                virtual const libtender::domMod::tender::TenderTypeCollectionPtr    getTypeMapping();
                virtual const libtender::domMod::tender::TenderStateCollectionPtr   getStatusMapping();
            private slots:
                void onArticlePickerClicked();
                void onCustomerPickerClicked();
                void onPharmacyChainPickerClicked();
                void onSearchClicked();
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

                Ui::TenderSearchCriteriaSerbia                      ui;
                libutil::misc::Flag                                 m_EventsWiring;
                libtender::domMod::tender::TenderStateCollectionPtr m_statusMapping;
                libtender::domMod::tender::TenderTypeCollectionPtr  m_typeMapping;
                
                basar::Int16                                        m_area;
        };
    }
}

#endif //end GUARD_TENDER_VIEWCONN_TENDERSEARCHCRITERIASERBIA_H
