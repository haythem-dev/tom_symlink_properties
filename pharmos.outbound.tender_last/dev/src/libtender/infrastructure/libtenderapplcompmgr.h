#ifndef GUARD_LIBTENDER_COMPONENTMANAGER_LIBTENDERAPPLCOMPMGR_H
#define GUARD_LIBTENDER_COMPONENTMANAGER_LIBTENDERAPPLCOMPMGR_H

#include <domainmodule/tender/itendercollectiondmptr.h>
#include <domainmodule/tender/tendertypecollectionptr.h>
#include <domainmodule/tender/tenderstatecollectionptr.h>
#include "domainmodule/scheduleddelivery/ischeduleddeliverycollectiondmptr.h"
#include "domainmodule/scheduleddelivery/ischeduleptr.h"
#include <domainmodule/article/iarticlepricedmptr.h>
#include <domainmodule/article/iarticlecollectiondmptr.h>
#include <domainmodule/customer/icustomercollectiondmptr.h>
#include <domainmodule/pharmacygroup/ipharmacygroupcollectiondmptr.h>
#include <domainmodule/partner/ipartnercollectiondmptr.h>
#include <domainmodule/supplier/isuppliercollectiondmptr.h>
#include <domainmodule/pharmacychain/ipharmacychaincollectiondmptr.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliverycollectiondmptr.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliveryimportcollectionptr.h>
#include <domainmodule/purchaseorderproposal/ipurchaseorderproposalptr.h>
#include <domainmodule/tender/itenderprogressptr.h>
#include <domainmodule/tender/itenderimportcollectionptr.h>
#include <domainmodule/customerlist/icustomerlistdmptr.h>
#include <domainmodule/tender/itenderprotocolptr.h>
#include <domainmodule/cscorder/icscorderptr.h>
#include <domainmodule/cscordercustomer/icscordercustomercollectiondmptr.h>
#include "componentmanager/itenderconnectedfactoryptr.h"
#include <componentmanager/ischeduleddeliveryconnectedfactoryptr.h>
#include <boost/enable_shared_from_this.hpp>
#include <libutil/infrastructure/session/isessionptr.h>
#include <infrastructure/purchasedbconn/ipurchasedbparameterproxyptr.h>
#include "countryidenum.h"
#include <libutil/accessor.h>
#include <libutil/dbtransaction.h>
#include <libutil/validator.h>
#include <libutil/infrastructurecomponent.h>

namespace basar
{
namespace db
{
namespace aspect
{
class ConnectionRef;
}
}
}

namespace log4cplus
{
class Logger;
}

namespace libtender
{
namespace componentManager
{
class ApplCompMgr : public libutil::infrastructure::db::dbTransaction::IDBTransactionFactory,
                    public boost::enable_shared_from_this< ApplCompMgr >
{
public:
	ApplCompMgr();
	virtual ~ApplCompMgr();

	void injectDBConnection( basar::db::aspect::ConnectionRef );
	void injectSession( libutil::infrastructure::session::ISessionPtr session );
	void injectPurchaseDBParameterProxy( infrastructure::db::IPurchaseDBParameterProxyPtr );

	libtender::domMod::tender::ITenderCollectionDMPtr							getTenderCollectionDM();
	libtender::domMod::tender::TenderTypeCollectionPtr							getTenderTypeCollection();
	libtender::domMod::tender::TenderStateCollectionPtr							getTenderStateCollection();						
	libtender::domMod::article::IArticleCollectionDMPtr							getArticleCollectionDM();
	libtender::domMod::article::IArticlePricePtr								getArticlePriceDM();
	libtender::domMod::customer::ICustomerCollectionDMPtr						getCustomerCollectionDM();
    libtender::domMod::pharmacygroup::IPharmacyGroupCollectionDMPtr getPharmacyGroupCollectionDM();
	libtender::domMod::partner::IPartnerCollectionDMPtr							getPartnerCollectionDM();
    libtender::domMod::supplier::ISupplierCollectionDMPtr getSupplierCollectionDM();
	libtender::domMod::pharmacychain::IPharmacyChainCollectionDMPtr				getPharmacyChainCollectionDM();
	libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr		getScheduledDeliveryCollectionDM();
	libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr			getPurchaseOrderProposalDM();
	libtender::domMod::tender::ITenderProgressPtr								getTenderProgressDM();
	libtender::domMod::tender::ITenderImportCollectionPtr						getTenderImportCollection();
	libtender::domMod::scheduledDelivery::IScheduledDeliveryImportCollectionPtr getScheduledDeliveryImportCollection(
		libtender::domMod::tender::ITenderImportCollectionPtr tenderImportCollection
			);
	libtender::domMod::scheduledDelivery::ISchedulePtr							getSchedule();
	libtender::domMod::customerlist::ICustomerListDMPtr							getCustomerListDM();
	libtender::domMod::tender::ITenderProtocolPtr								getTenderProtocolDM();
    libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr getCscOrderCustomerCollectionDM();

private:
	ApplCompMgr( const ApplCompMgr& );
	ApplCompMgr& operator=( const ApplCompMgr& );

	libutil::infrastructure::accessor::IAccessorPtr								getTenderCollectionAccessor();
	libutil::infrastructure::accessor::IAccessorPtr								getScheduledDeliveryCollectionAccessor();
	libutil::infrastructure::accessor::IAccessorPtr								getArticlePriceAccessor();
	libutil::infrastructure::accessor::IAccessorPtr								getArticleCollectionAccessor();
	libutil::infrastructure::accessor::IAccessorPtr								getCustomerCollectionAccessor();
    libutil::infrastructure::accessor::IAccessorPtr getPharmacyGroupCollectionAccessor();
	libutil::infrastructure::accessor::IAccessorPtr								getPartnerCollectionAccessor();
    libutil::infrastructure::accessor::IAccessorPtr getSupplierCollectionAccessor();
	libutil::infrastructure::accessor::IAccessorPtr								getPharmacyChainCollectionAccessor();
	libutil::infrastructure::accessor::IAccessorPtr								getPurchaseOrderProposalAccessor();
	libutil::infrastructure::accessor::IAccessorPtr								getTenderProgressAccessor();
	libutil::infrastructure::accessor::IAccessorPtr								getCustomerListAccessor();
	libutil::infrastructure::accessor::IAccessorPtr								getTenderProtocolAccessor();
	libutil::infrastructure::accessor::IAccessorPtr getCscOrderAccessor();
    
	virtual libutil::infrastructure::db::dbTransaction::IDBTransactionPtr		createDBTransaction();
	
	ITenderConnectedFactoryPtr													getTenderConnectedFactory();
	IScheduledDeliveryConnectedFactoryPtr										getScheduledDeliveryConnectedFactory();

	basar::db::aspect::ConnectionRef											getDBConnection();
	libutil::infrastructure::session::ISessionPtr								getSession();
	infrastructure::CountryIDEnum												getCountryID();

	const log4cplus::Logger 													m_Logger;
	bool																		m_AccessorDefined;
	basar::db::aspect::ConnectionRef											m_DBConnection;
	libutil::infrastructure::session::ISessionPtr								m_Session;
	infrastructure::db::IPurchaseDBParameterProxyPtr							m_PurchaseDBParameterProxy;

};

} // end namnespace componentManager
} // end namnespace libtender

#endif //end GUARD_LIBTENDER_COMPONENTMANAGER_APPLCOMPMGR_H
