#ifndef GUARD_TENDER_COMPONENTMANAGER_IUSECASEGETTER_H
#define GUARD_TENDER_COMPONENTMANAGER_IUSECASEGETTER_H


//------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------//
#include <libtender/domainmodule/tender/itenderptr.h>
#include <libtender/domainmodule/tender/itenderprotocolptr.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliverycollectiondmptr.h>
#include <libtender/domainmodule/cscordercustomer/icscordercustomercollectiondmptr.h>
#include <libtender/domainmodule/scheduleddelivery/ischeduleddeliveryptr.h>
#include <libtender/domainmodule/customerlist/icustomerlistdmptr.h>
#include <libtender/domainmodule/tender/itenderprogressptr.h>

#include <libutil/irunnableptr.h>
#include <libutil/accessor.h>

#include "ipickerucptr.h"
#include "itenderstateucptr.h"
//#include "omgorderucptr.h"

//------------------------------------------------------------------------------//
// forward declaration
//------------------------------------------------------------------------------//

//------------------------------------------------------------------------------//
// namespaces
//------------------------------------------------------------------------------//
namespace tender
{
    namespace componentManager
    {
        //------------------------------------------------------------------------------//
        // class IUseCaseGetter
        //------------------------------------------------------------------------------//
        class IUseCaseGetter
        {
            public:
                virtual ~IUseCaseGetter(){};
                virtual libutil::useCase::IRunnablePtr getTenderOverviewUC() = 0;
				virtual libutil::useCase::IRunnablePtr getTenderMaintainUC( libtender::domMod::tender::ITenderPtr, libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr ptr = libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr(), bool cloneDeliveries = false ) = 0;
				virtual libutil::useCase::IRunnablePtr getScheduledDeliveryMaintainUC( libtender::domMod::tender::ITenderPtr, libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr,  libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr, bool needsCustomerSelection ) = 0;
				virtual tender::useCase::IPickerUCPtr  getCustomerSelectionUC(basar::db::aspect::AccessorPropertyTableRef, basar::Int32 customerNo, basar::Int32 pharmacyChainNo) = 0;
                virtual tender::useCase::IPickerUCPtr  getArticlePickerUC ( basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::AccessorPropertyTable_YIterator() ) = 0;
		        virtual tender::useCase::IPickerUCPtr  getCustomerPickerUC( basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::AccessorPropertyTable_YIterator() ) = 0;
                virtual tender::useCase::IPickerUCPtr  getPharmacyGroupPickerUC( basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::AccessorPropertyTable_YIterator() ) = 0;
		        virtual tender::useCase::IPickerUCPtr  getPartnerPickerUC ( basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::AccessorPropertyTable_YIterator() ) = 0;
                virtual tender::useCase::IPickerUCPtr  getSupplierPickerUC ( basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::AccessorPropertyTable_YIterator() ) = 0;
				virtual tender::useCase::IPickerUCPtr  getPharmacyChainPickerUC ( basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::AccessorPropertyTable_YIterator() ) = 0;
				virtual libutil::useCase::IRunnablePtr getTenderImportUC() = 0;
				virtual libutil::useCase::IRunnablePtr getCustomerListMaintainUC( libtender::domMod::customerlist::ICustomerListDMPtr ) = 0;
				virtual libutil::useCase::IRunnablePtr getTenderProtocolUC( libtender::domMod::tender::ITenderPtr tender, basar::Int16 area ) = 0;
				virtual libutil::useCase::IRunnablePtr getInvoiceUC( libtender::domMod::tender::ITenderProgressPtr tenderProgress ) = 0;
                virtual libutil::useCase::IRunnablePtr getOMGOrderUC( libtender::domMod::tender::ITenderPtr ) = 0;
                virtual libutil::useCase::IRunnablePtr getCSCOrderUC(libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr ) = 0;
                virtual libutil::useCase::IRunnablePtr getCSCOrderMaintainUC( libtender::domMod::tender::ITenderPtr, libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr ) = 0;
                virtual tender::useCase::ITenderStateUCPtr getTenderStateUC( ) =0;
        };
    }
}

#endif //end GUARD_TENDER_COMPONENTMANAGER_IUSECASEGETTER_H
