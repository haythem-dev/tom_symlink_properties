#ifndef GUARD_TENDER_VIEWCONN_TENDERCONTRACTINFOSWITZERLAND_H
#define GUARD_TENDER_VIEWCONN_TENDERCONTRACTINFOSWITZERLAND_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_tendercontractinfoswitzerland.h"
#include <QObject>
#pragma warning (pop)

#include "itendercontractinfo.h"

#include <libtender/domainmodule/tender/tendertypecollectionptr.h>
#include <libtender/domainmodule/tender/tenderstatecollectionptr.h>

#include <libutil/flag.h>
#include <libutil/viewconn.h>
#include "inputvalidator.h"


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

namespace log4cplus
{
    class Logger;
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
        class TenderContractInfoSwitzerland : public ITenderContractInfo
        {
            Q_OBJECT

            public:
                TenderContractInfoSwitzerland();
                ~TenderContractInfoSwitzerland();

                void injectStatusMapping( libtender::domMod::tender::TenderStateCollectionPtr statusMapping );
                void injectTypeMapping( libtender::domMod::tender::TenderTypeCollectionPtr typeMapping );
                
                void matchToTender( basar::db::aspect::AccessorPropertyTable_YIterator yitTender );
                void matchFromTender( basar::db::aspect::AccessorPropertyTable_YIterator yitTender, bool hasTenderArticles );
                
                void matchFromCustomer( basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer );
                void matchToCustomerSearch( basar::db::aspect::AccessorPropertyTable_YIterator customerSearchYit );

                void matchFromPharmacyGroup( basar::db::aspect::AccessorPropertyTable_YIterator yitPharmacyGroup );
                void matchToPharmacyGroupSearch( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyGroupSearchYit );
                
                void setCustomerNo( basar::db::aspect::AccessorPropertyTable_YIterator yit );
                void setPharmacyGroupId( basar::db::aspect::AccessorPropertyTable_YIterator yit );
                void setTenderState( libtender::domMod::tender::TenderStateEnum );

                const libtender::domMod::tender::TenderTypeCollectionPtr    getTypeMapping();
                const libtender::domMod::tender::TenderStateEnum            getStateIndex();
                const libtender::domMod::tender::TenderStateEnum            getStateData();
                const libtender::domMod::tender::TenderTypeEnum             getType();
                const basar::Date                                           getDateTo();
                
                bool isFilled();
				bool isCustomerSelected();
                bool isCustomerInFocus();
                bool isPharmacyGroupInFocus();
                bool isContractPeriodValid();
                
                void resetAllFields();
                void resetCustomer();
                void resetPharmacyGroup();
                
                void setFocusOnFirstEmptyField();
                void disableAll();
                void disableAllExceptState( bool disable, bool hasTenderArticles );

            private slots:

                void onStartSearch();
                void onCustomerListClicked();
                void onInvoiceClicked();
                void onCheckCustomer();
                void onCheckPharmacyGroup();
                void onContractPeriodClicked( bool isChecked );
                void onContractDateChanged();
                void onStateChanged( int index );
                void onTypeChanged( int index );
                void onContractInfoChanged();
                void onShowProtocols();

            protected:
                virtual void focusInEvent( QFocusEvent* event);
                virtual void focusOutEvent( QFocusEvent* event);

            private:
                void wireEvents();
                void setupContractInfo();

                Ui::TenderContractInfoSwitzerland                   ui;
                const log4cplus::Logger                             m_Logger;
                libutil::misc::Flag                                 m_EventsWiring;
                libtender::domMod::tender::TenderStateCollectionPtr m_TenderStateCollection;
                libtender::domMod::tender::TenderTypeCollectionPtr  m_TenderTypeCollection;
                QAction *                                           m_StartSearchAction;
        };
    }
}

#endif //end GUARD_TENDER_VIEWCONN_TENDERCONTRACTINFOSWITZERLAND_H
