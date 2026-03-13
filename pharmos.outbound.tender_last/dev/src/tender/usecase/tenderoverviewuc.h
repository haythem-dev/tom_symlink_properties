#ifndef GUARD_TENDER_USECASE_TENDEROVERVIEWUC_H
#define GUARD_TENDER_USECASE_TENDEROVERVIEWUC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "tenderoverviewvcptr.h"
#include "iusecasegetterptr.h"
#include "itenderstateucptr.h"

#include <domainmodule/tender/itendercollectiondmptr.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliverycollectiondmptr.h>
#include <libtender/domainmodule/tender/itenderprogressptr.h>
#include <libtender/domainmodule/tender/itenderptr.h>
#include <libtender/domainmodule/purchaseorderproposal/ipurchaseorderproposalptr.h>

// libs
#include <libutil/irunnable.h>
#include <libutil/session.h>
#include <libutil/dbtransaction.h>

#include <libbasarappl_eventmacros.h>
#include <libbasarguitie.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace useCase
    {
        //-------------------------------------------------------------------------------------------------//
        // class declaration
        //-------------------------------------------------------------------------------------------------//
        class TenderOverviewUC : public libutil::useCase::IRunnable
        {
            public:
                TenderOverviewUC();
                virtual ~TenderOverviewUC();

                SYSTEM_EVENT_HANDLER_DECLARATION( SearchTenderRequested			);
                SYSTEM_EVENT_HANDLER_DECLARATION( EditTenderRequested			);
				SYSTEM_EVENT_HANDLER_DECLARATION( DeleteTenderRequested			);
				SYSTEM_EVENT_HANDLER_DECLARATION( CloneTenderRequested			);
                SYSTEM_EVENT_HANDLER_DECLARATION( NewTenderRequested			);
				SYSTEM_EVENT_HANDLER_DECLARATION( SearchArticleRequested		);
				SYSTEM_EVENT_HANDLER_DECLARATION( SearchCustomerRequested		);
				SYSTEM_EVENT_HANDLER_DECLARATION( SearchPartnerRequested		);
                SYSTEM_EVENT_HANDLER_DECLARATION( SearchSupplierRequested       );
				SYSTEM_EVENT_HANDLER_DECLARATION( SearchPharmacyChainRequested	);
                SYSTEM_EVENT_HANDLER_DECLARATION( SearchPharmacyGroupRequested  );

                void injectTenderOverviewVC				( tender::viewConn::TenderOverviewVCPtr tenderOverviewVC );
                void injectTenderCollectionDM			( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM );
				void injectScheduledDeliveryCollectionDM( libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr scheduledDeliveryCollectionDM );
                void injectUseCaseGetter				( tender::componentManager::IUseCaseGetterPtr useCaseGetter );
				void injectTenderProgressDM				( libtender::domMod::tender::ITenderProgressPtr );
				void injectPurchaseOrderProposalDM		( libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr );
				void injectTransactionFactory			( libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory );
                void injectTenderStateUC                ( tender::useCase::ITenderStateUCPtr );
                virtual void run();
                void setArea( basar::Int16 area );
            private:
                TenderOverviewUC( const TenderOverviewUC& );
                TenderOverviewUC& operator= ( const TenderOverviewUC& );

                void																	init();
                void																	reset();
																
                void																	registerEventHandlers( const bool forSave );

				bool																	isPurchaseOrderExisting(basar::db::aspect::AccessorPropertyTableRef);
				void																	deletePurchaseOrderProposals(basar::db::aspect::AccessorPropertyTableRef);
																
                const log4cplus::Logger&												getLogger() const;
																
                tender::viewConn::TenderOverviewVCPtr									getTenderOverviewVC();
                libtender::domMod::tender::ITenderCollectionDMPtr						getTenderCollectionDM();
				libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr getScheduledDeliveryCollectionDM();
                tender::componentManager::IUseCaseGetterPtr								getUseCaseGetter();
                bool                                                                    validateTenderState( const basar::Int32 articleNo, const basar::Int32 tenderNo,
                                                                                                             const basar::VarString & pharmGrpID, const basar::Date & endDate  );

                //////////////////////////////////////////////////////////////////////////////////////
                // member variables
                //////////////////////////////////////////////////////////////////////////////////////

                tender::viewConn::TenderOverviewVCPtr									m_TenderOverviewVC;
                libtender::domMod::tender::ITenderCollectionDMPtr						m_TenderCollection;
				libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr m_ScheduledDeliveryCollection;
				libtender::domMod::tender::ITenderProgressPtr							m_TenderProgress;
				libtender::domMod::purchaseOrderProposal ::IPurchaseOrderProposalPtr	m_PurchaseOrderProposal;
                tender::componentManager::IUseCaseGetterPtr								m_UseCaseGetter;
                tender::useCase::ITenderStateUCPtr                                      m_TenderStateUC;
                basar::Int16                                                            m_Area;

                const log4cplus::Logger													m_Logger;
				libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr	m_TransactionFactory;
        };
    } // namespace useCase
} // namespace tender

#endif //end GUARD_TENDER_USECASE_TENDEROVERVIEWUC_H
