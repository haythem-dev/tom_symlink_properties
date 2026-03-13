#ifndef GUARD_TENDER_USECASE_CSCORDERMAINTAINUC_H
#define GUARD_TENDER_USECASE_CSCORDERMAINTAINUC_H

//-------------------------------------------------------------------------------------------------//

#include "icscordermaintainvcptr.h"
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>
#include <libtender/domainmodule/tender/itenderptr.h>
#include <libtender/domainmodule/tender/itenderprogressptr.h>
#include <libtender/domainmodule/customer/icustomercollectiondmptr.h>
#include <libtender/domainmodule/cscordercustomer/icscordercustomercollectiondmptr.h>
#include <libutil/irunnableptr.h>
#include <libutil/irunnable.h>
#include <libutil/session.h>
#include <libbasarappl_eventmacros.h>
#include <libbasarguitie.h>

//-------------------------------------------------------------------------------------------------//

namespace tender {
namespace useCase {

//-------------------------------------------------------------------------------------------------//

class CSCOrderMaintainUC : public libutil::useCase::IRunnable
{
public:
    CSCOrderMaintainUC();
    virtual ~CSCOrderMaintainUC();
    
    SYSTEM_EVENT_HANDLER_DECLARATION( CreateCSCOrderRequested );
    SYSTEM_EVENT_HANDLER_DECLARATION( SelectPositionRequested );
    //SYSTEM_EVENT_HANDLER_DECLARATION( GetAvailableQuantitiesRequested );
    SYSTEM_EVENT_HANDLER_DECLARATION( ChangeQuantityRequested );
    
    void injectCSCOrderMaintainVC           ( tender::viewConn::ICSCOrderMaintainVCPtr cscOrderMaintainVC );
    void setScheduledDelivery               ( libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery );
    void setTender                          ( libtender::domMod::tender::ITenderPtr tender );
    void injectCustomerCollectionDM         ( libtender::domMod::customer::ICustomerCollectionDMPtr );
    void injectTenderProgressDM             ( libtender::domMod::tender::ITenderProgressPtr );
    void injectCscOrderCustomerCollectionDM ( libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr );
    void injectCSCorderUC                   ( libutil::useCase::IRunnablePtr );
    
    virtual void                            run();
    
    void registerEventHandlers( const bool forSave );

private:
    CSCOrderMaintainUC( const CSCOrderMaintainUC& );
    CSCOrderMaintainUC & operator= ( const CSCOrderMaintainUC & );
    
    const log4cplus::Logger&                                                getLogger() const;

	void																	getAvailableQuantities();
    libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr             getScheduledDelivery(); 
    libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr   getCscOrderCustomerCollection(); 
    libutil::useCase::IRunnablePtr                                          getCSCOrderUC();
    libtender::domMod::tender::ITenderPtr                                   getTender();
    tender::viewConn::ICSCOrderMaintainVCPtr                                getCSCOrderMaintainVC();
    basar::Int16                                                            getDefaultBranch( int tenderType );
    
    libtender::domMod::customer::ICustomerCollectionDMPtr                   m_CustomerCollection;
    libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr   m_CscOrderCustomerCollection;
    libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr             m_ScheduledDelivery;
    tender::viewConn::ICSCOrderMaintainVCPtr                                m_CSCOrderMaintainVC;
    libtender::domMod::tender::ITenderPtr                                   m_Tender;
    libtender::domMod::tender::ITenderProgressPtr                           m_TenderProgress;
    libutil::useCase::IRunnablePtr                                          m_CSCOrderUC;
    const log4cplus::Logger                                                 m_Logger;
};

//-------------------------------------------------------------------------------------------------//

} //end namespace useCase
} //end namespace tender

//-------------------------------------------------------------------------------------------------//

#endif //end GUARD_TENDER_USECASE_CSCORDERMAINTAINUC_H

//-------------------------------------------------------------------------------------------------//
