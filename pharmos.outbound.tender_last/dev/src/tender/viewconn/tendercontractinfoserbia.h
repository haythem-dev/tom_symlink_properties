#ifndef GUARD_TENDER_VIEWCONN_TENDERCONTRACTINFOSERBIA_H
#define GUARD_TENDER_VIEWCONN_TENDERCONTRACTINFOSERBIA_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_tendercontractinfoserbia.h"
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
        class TenderContractInfoSerbia : public ITenderContractInfo
        {
            Q_OBJECT

            public:
                TenderContractInfoSerbia();
                ~TenderContractInfoSerbia();

				void injectStatusMapping( libtender::domMod::tender::TenderStateCollectionPtr statusMapping );
                void injectTypeMapping( libtender::domMod::tender::TenderTypeCollectionPtr typeMapping );

                void matchToTender( basar::db::aspect::AccessorPropertyTable_YIterator yitTender );
				void matchFromTender( basar::db::aspect::AccessorPropertyTable_YIterator yitTender, bool hasTenderArticles );

				void matchFromCustomer( basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer );
				void matchFromPharmacyChainNo( basar::db::aspect::AccessorPropertyTable_YIterator yitPharmacyChain );
                
				void matchToCustomerSearch( basar::db::aspect::AccessorPropertyTable_YIterator customerSearchYit );
				void matchToPharmacyChainSearch( basar::db::aspect::AccessorPropertyTable_YIterator pharmacyChainSearchYit );

				void setCustomerNo( basar::db::aspect::AccessorPropertyTable_YIterator yit );
				void setPharmacyChainNo( basar::db::aspect::AccessorPropertyTable_YIterator yit );

				const libtender::domMod::tender::TenderTypeCollectionPtr getTypeMapping();
				const libtender::domMod::tender::TenderStateEnum		 getStateIndex();
				const libtender::domMod::tender::TenderStateEnum		 getStateData();
				const libtender::domMod::tender::TenderTypeEnum			 getType();
				const basar::Date										 getDateTo();

				bool isFilled();
				bool isCustomerSelected(); 

				bool isPharmacyChainInFocus();
				bool isCustomerInFocus();
				bool isContractPeriodValid();

				void resetAllFields();
				void resetCustomer();
				void prepareForCloning();

				void setFocusOnFirstEmptyField();
				void disableAll();
				void disableAllExceptState( bool disable, bool hasTenderArticles );

            private slots:

				void onStartSearch();
				void onCheckCustomer();
				void onCheckPharmacyChain();
				void onContractPeriodClicked( bool isChecked );
				void onContractDateChanged();
				void onStateChanged( int index );
				void onContractInfoChanged();
				void onCustomerChecked( bool checked );
				void onPharmacyChainChecked( bool checked );
				void onShowProtocols();

            protected:
                virtual void focusInEvent( QFocusEvent* event);
                virtual void focusOutEvent( QFocusEvent* event);

            private:
                void wireEvents();
                void setupContractInfo();
				void setCustomerVisible(bool visible);
				void setPharmacyChainVisible( bool visible );

                Ui::TenderContractInfoSerbia						ui;
				const log4cplus::Logger								m_Logger;
                libutil::misc::Flag									m_EventsWiring;
                libtender::domMod::tender::TenderStateCollectionPtr m_TenderStateCollection;
				libtender::domMod::tender::TenderTypeCollectionPtr	m_TenderTypeCollection;
				QAction *											m_StartSearchAction;
        };
    }
}

#endif //end GUARD_TENDER_VIEWCONN_TENDERCONTRACTINFOSERBIA_H
