#ifndef GUARD_TENDER_USECASE_SCHEDULEDDELIVERYMAINTAINUC_H
#define GUARD_TENDER_USECASE_SCHEDULEDDELIVERYMAINTAINUC_H

//-------------------------------------------------------------------------------------------------//

#include "ischeduleddeliverymaintainvcptr.h"
#include "tender/infrastructure/iusecasegetterptr.h"
#include "tender/usecase/ipickerucptr.h"

#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliverycollectiondmptr.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>
#include <libtender/domainmodule/purchaseorderproposal/ipurchaseorderproposalptr.h>
#include <libtender/domainmodule/tender/itenderptr.h>
#include <libtender/domainmodule/tender/itenderprogressptr.h>
#include <libtender/domainmodule/customer/icustomercollectiondmptr.h>
#include <libutil/irunnableptr.h>
#include <libutil/irunnable.h>
#include <libutil/session.h>
#include <libbasarappl_eventmacros.h>
#include <libbasarguitie.h>

//-------------------------------------------------------------------------------------------------//

namespace tender {
namespace useCase {

//-------------------------------------------------------------------------------------------------//

class ScheduledDeliveryMaintainUC : public libutil::useCase::IRunnable
{
public:
	ScheduledDeliveryMaintainUC();
	virtual ~ScheduledDeliveryMaintainUC();

	SYSTEM_EVENT_HANDLER_DECLARATION( SaveDeliveryRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( CheckDateRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( SelectPositionRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( DeselectPositionRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( SelectPositionCollectionRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( DeselectPositionCollectionRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( GetAvailableQuantitiesRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( ChangeQuantityRequested );
	SYSTEM_EVENT_HANDLER_DECLARATION( ChangeBranchNoPOPRequested );
    SYSTEM_EVENT_HANDLER_DECLARATION( SelectCustomerRequested );

    void injectUseCaseGetter(tender::componentManager::IUseCaseGetterPtr);
	void injectScheduledDeliveryMaintainVC(tender::viewConn::IScheduledDeliveryMaintainVCPtr);
	void setScheduledDelivery(libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr);
	void setTender(libtender::domMod::tender::ITenderPtr);
	void setScheduledDeliveryCollection( libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr scheduledDeliveryCollection);
	void injectPurchaseOrderProposalDM( libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr);
	void injectTenderProgressDM( libtender::domMod::tender::ITenderProgressPtr);
    void injectCustomerCollectionDM       ( libtender::domMod::customer::ICustomerCollectionDMPtr );

	virtual void run();

	void registerEventHandlers( const bool forSave );

private:
	ScheduledDeliveryMaintainUC( const ScheduledDeliveryMaintainUC& );
	ScheduledDeliveryMaintainUC & operator= ( const ScheduledDeliveryMaintainUC & );

	void init();
	void reset();

	//<PurchaseOrderProposal>
	typedef std::list<basar::db::aspect::AccessorPropertyTable_YIterator> YitList;
	YitList getPositionsToChange();
	YitList getPositionsToDelete();
	void preparePurchaseOrderProposalSearchIterator(const basar::db::aspect::AccessorPropertyTable_YIterator);
	void setPurchaseOrderProposalValues(const basar::db::aspect::AccessorPropertyTable_YIterator);
	void transferChangedPositionsToPurchase(const YitList&);
	void transferDeletedPositionsToPurchase(const YitList&);
	//</PurchaseOrderProposal>

	basar::db::aspect::AccessorPropertyTableRef					getCustomersForPharmacyChain(tender::useCase::IPickerUCPtr pharmacyChainPickerUC);

	const log4cplus::Logger&									getLogger() const;

	libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr	getScheduledDelivery();
	libtender::domMod::tender::ITenderPtr						getTender();

	tender::viewConn::IScheduledDeliveryMaintainVCPtr			getScheduledDeliveryMaintainVC();

	bool isBranchValid( int branch, int tenderType );
	basar::Int16 getDefaultBranch( int tenderType );

    libtender::domMod::customer::ICustomerCollectionDMPtr                   m_CustomerCollection;
    tender::componentManager::IUseCaseGetterPtr								m_UseCaseGetter;
	libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr m_ScheduledDeliveryCollection;
	libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr				m_ScheduledDelivery;
	libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr		m_PurchaseOrderProposal;
	tender::viewConn::IScheduledDeliveryMaintainVCPtr						m_ScheduledDeliveryMaintainVC;
	libtender::domMod::tender::ITenderPtr									m_Tender;
	libtender::domMod::tender::ITenderProgressPtr							m_TenderProgress;
	const log4cplus::Logger 												m_Logger;
	bool																	m_ModifyExistingDelivery;
};

//-------------------------------------------------------------------------------------------------//

} //end namespace useCase
} //end namespace tender

//-------------------------------------------------------------------------------------------------//

#endif //end GUARD_TENDER_USECASE_SCHEDULEDDELIVERYMAINTAINUC_H

//-------------------------------------------------------------------------------------------------//
