#ifndef GUARD_TENDER_USECASE_TENDERMAINTAINUC_H
#define GUARD_TENDER_USECASE_TENDERMAINTAINUC_H

#include "tendermaintainvcptr.h"
#include "scheduleddeliverymaintainvcptr.h"

#include "iusecasegetterptr.h"
#include "omgorderucptr.h"
#include "itenderstateucptr.h"
#include <libtender/domainmodule/tender/tenderstateenum.h>

#include <libtender/domainmodule/tender/itenderptr.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleptr.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliverycollectiondmptr.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>
#include <libtender/domainmodule/customerlist/icustomerlistdmptr.h>
#include <libtender/domainmodule/purchaseorderproposal/ipurchaseorderproposalptr.h>
#include <domainmodule/tender/itendercollectiondmptr.h>
#include <libtender/domainmodule/article/iarticlepricedmptr.h>
#include <libutil/irunnable.h>
#include <libutil/session.h>

#include <libbasarappl_eventmacros.h>
#include <libbasarguitie.h>

namespace tender
{
namespace useCase
{
class TenderMaintainUC : public libutil::useCase::IRunnable
{
public:
	TenderMaintainUC();
	virtual ~TenderMaintainUC();

	void injectTenderMaintainVC( tender::viewConn::TenderMaintainVCPtr tenderMaintainVC );
	void injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter );
	void injectPurchaseOrderProposalDM( libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr );
	void injectArticlePriceDM(libtender::domMod::article::IArticlePricePtr);
    void injectOMGOrderUC(tender::useCase::OMGOrderUCPtr omgucPtr);
    void injectTenderCollectionDM( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM );
    void injectTenderStateUC( tender::useCase::ITenderStateUCPtr );
		
	void setArea( basar::Int16 area );
	void setTender( libtender::domMod::tender::ITenderPtr tender );
	void setSchedule( libtender::domMod::scheduledDelivery::ISchedulePtr schedule );
    void setTenderEndDateOffset( basar::Int32 days);
	void setCloneDeliveriesFlag(bool cloneDeliveries);

	virtual void run();
	void registerEventHandlers( const bool forSave );

	SYSTEM_EVENT_HANDLER_DECLARATION( AddItemRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( ModifyItemRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( DeleteItemRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( SaveTenderRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( SaveTenderPositionRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( SaveAndNewTenderPositionRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( ModifyTenderPositionRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( CancelModifyTenderPositionRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( DeleteTenderPositionRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchArticleRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( CheckArticleRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchCustomerRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( CheckCustomerRequested );
    SYSTEM_EVENT_HANDLER_DECLARATION( SearchPharmacyGroupRequested );
    SYSTEM_EVENT_HANDLER_DECLARATION( CheckPharmacyGroupRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchPartnerRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( CheckPartnerRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchPharmacyChainRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( CheckPharmacyChainRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( CheckCustomerGroupIDRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( AddDeliveryRequested    );
    SYSTEM_EVENT_HANDLER_DECLARATION( CreateCSCOrdersRequested    );
	SYSTEM_EVENT_HANDLER_DECLARATION( ModifyDeliveryRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( DeleteDeliveryRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( DeliverySelectionChangeRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( ScheduleRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( ChangeStateToClosedRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( ChangeStateFromClosedRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( CheckIfPositionsWereModifiedRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( CloneDeliveryRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( CheckIfTenderWasModifiedRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( CheckIfContractPeriodIsValid );
	SYSTEM_EVENT_HANDLER_DECLARATION( ContractInfoChanged );
	SYSTEM_EVENT_HANDLER_DECLARATION( CustomerListRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( CheckCustomerListRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( ProtocolsRequested );

private:
	TenderMaintainUC( const TenderMaintainUC& );
	TenderMaintainUC & operator= ( const TenderMaintainUC & );

	void init();
	void reset();
	const log4cplus::Logger & getLogger() const;

	basar::appl::EventReturnType AddPosition( basar::db::aspect::AccessorPropertyTable_YIterator );
	basar::appl::EventReturnType ModifyPosition( basar::db::aspect::AccessorPropertyTable_YIterator );

	void getPricesForArticle(basar::db::aspect::AccessorPropertyTable_YIterator);

	void saveClonedDeliveries();
	bool isQuantityUndershot( basar::db::aspect::AccessorPropertyTable_YIterator positionToBeModified );

	typedef std::list<basar::db::aspect::AccessorPropertyTable_YIterator> YitList;
	YitList getDeliveryPositionsToSave(libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr);
	void transferDeliveryPositionsToPurchase(const YitList&, libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr);
	void deletePurchaseOrderProposals(libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr);
    libtender::domMod::tender::ITenderCollectionDMPtr getTenderCollectionDM();
    bool validateTenderState( const libtender::domMod::tender::TenderStateEnum oldTenderState );
    void setTenderState( basar::Int16 tenderState );

	bool needsCustomerSelection();
	bool hasCustomerListChanged(basar::db::aspect::AccessorPropertyTableRef customers);

	tender::viewConn::TenderMaintainVCPtr				getTenderMaintainVC();
	tender::componentManager::IUseCaseGetterPtr			getUseCaseGetter();

    tender::useCase::OMGOrderUCPtr                                          m_OMGOrderUC;
    tender::useCase::ITenderStateUCPtr                                      m_TenderStateUC;
	tender::viewConn::TenderMaintainVCPtr									m_TenderMaintainVC;
	tender::viewConn::ScheduledDeliveryMaintainVCPtr						m_ScheduledDeliveryMaintainVC;
    libtender::domMod::tender::ITenderCollectionDMPtr						m_TenderCollection;
	tender::componentManager::IUseCaseGetterPtr								m_UseCaseGetter;
	basar::Int16															m_Area;
	libtender::domMod::tender::ITenderPtr									m_Tender;
	libtender::domMod::scheduledDelivery::ISchedulePtr						m_Schedule;
	libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr		m_PurchaseOrderProposal;
	libtender::domMod::article::IArticlePricePtr							m_ArticlePrice;

	const log4cplus::Logger 												m_Logger;
	bool																	m_ModifyExistingPosition;
	bool																	m_CloneDeliveries;
};

} // namespace useCase
} // namespace tender

#endif
