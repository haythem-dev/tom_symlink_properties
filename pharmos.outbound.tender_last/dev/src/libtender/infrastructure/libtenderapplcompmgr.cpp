#include "libtenderapplcompmgr.h"
#include <loggerpool/tenderloggerpool.h>

#include <domainmodule/tender/tendercollectiondmptr.h>
#include <domainmodule/tender/tendercollectiondm.h>
#include <domainmodule/tender/tendertypecollection.h>
#include <domainmodule/tender/tenderstatecollection.h>
#include "domainmodule/scheduleddelivery/schedule.h"
#include "domainmodule/article/articlepricedmptr.h"
#include "domainmodule/article/articlepricedm.h"
#include "domainmodule/article/articlecollectiondmptr.h"
#include "domainmodule/article/articlecollectiondm.h"
#include "domainmodule/customer/customercollectiondmptr.h"
#include "domainmodule/customer/customercollectiondm.h"
#include "domainmodule/pharmacygroup/pharmacygroupcollectiondmptr.h"
#include "domainmodule/pharmacygroup/pharmacygroupcollectiondm.h"
#include "domainmodule/partner/partnercollectiondmptr.h"
#include "domainmodule/partner/partnercollectiondm.h"
#include "domainmodule/supplier/suppliercollectiondmptr.h"
#include "domainmodule/supplier/suppliercollectiondm.h"
#include "domainmodule/pharmacychain/pharmacychaincollectiondmptr.h"
#include "domainmodule/pharmacychain/pharmacychaincollectiondm.h"
#include <domainmodule/scheduleddelivery/scheduleddeliverycollectiondmptr.h>
#include <domainmodule/scheduleddelivery/scheduleddeliverycollectiondm.h>
#include <domainmodule/purchaseorderproposal/purchaseorderproposaldmptr.h>
#include <domainmodule/purchaseorderproposal/purchaseorderproposaldm.h>
#include <domainmodule/tender/tenderprogressdmptr.h>
#include <domainmodule/tender/tenderprogressdm.h>
#include <domainmodule/tender/tenderimportcollection.h>
#include <domainmodule/tender/tenderimportcollectionptr.h>
#include <domainmodule/scheduleddelivery/scheduleddeliveryimportcollectionptr.h>
#include <domainmodule/scheduleddelivery/scheduleddeliveryimportcollection.h>
#include <domainmodule/customerlist/customerlistdm.h>
#include <domainmodule/customerlist/customerlistdmptr.h>
#include <domainmodule/tender/tenderprotocoldm.h>
#include <domainmodule/tender/tenderprotocoldmptr.h>
#include <domainmodule/cscorder/cscorderdm.h>
#include <domainmodule/cscorder/cscorderdmptr.h>
#include <domainmodule/cscordercustomer/cscordercustomercollectiondmptr.h>
#include <domainmodule/cscordercustomer/cscordercustomercollectiondm.h>

#include <infrastructure/accessor/tender/tendercollectionacc_definitions.h>
#include <infrastructure/accessor/tender/tendercollectionacc.h>
#include "infrastructure/accessor/scheduleddelivery/scheduleddeliverycollectionacc_definitions.h"
#include "infrastructure/accessor/scheduleddelivery/scheduleddeliverycollectionacc.h"
#include "infrastructure/accessor/article/articlepriceacc_definitions.h"
#include "infrastructure/accessor/article/articlepriceacc.h"
#include "infrastructure/accessor/article/articlecollectionacc_definitions.h"
#include "infrastructure/accessor/article/articlecollectionacc.h"
#include "infrastructure/accessor/customer/customercollectionacc_definitions.h"
#include "infrastructure/accessor/customer/customercollectionacc.h"
#include "infrastructure/accessor/pharmacygroup/pharmacygroupcollectionacc_definitions.h"
#include "infrastructure/accessor/pharmacygroup/pharmacygroupcollectionacc.h"
#include "infrastructure/accessor/partner/partnercollectionacc_definitions.h"
#include "infrastructure/accessor/partner/partnercollectionacc.h"
#include "infrastructure/accessor/supplier/suppliercollectionacc_definitions.h"
#include "infrastructure/accessor/supplier/suppliercollectionacc.h"
#include "infrastructure/accessor/pharmacychain/pharmacychaincollectionacc_definitions.h"
#include "infrastructure/accessor/pharmacychain/pharmacychaincollectionacc.h"
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliverycollectionacc_definitions.h>
#include <infrastructure/accessor/scheduleddelivery/scheduleddeliverycollectionacc.h>
#include <infrastructure/accessor/purchaseorderproposal/purchaseorderproposalacc_definitions.h>
#include <infrastructure/accessor/purchaseorderproposal/purchaseorderproposalacc.h>
#include <infrastructure/accessor/tender/tenderprogressacc_definitions.h>
#include <infrastructure/accessor/tender/tenderprogressacc.h>
#include <infrastructure/accessor/customerlist/customerlistacc_definitions.h>
#include <infrastructure/accessor/customerlist/customerlistacc.h>
#include <infrastructure/accessor/tender/tenderprotocolacc_definitions.h>
#include <infrastructure/accessor/tender/tenderprotocolacc.h>
#include <infrastructure/accessor/cscordercustomer/cscordercustomercollectionacc_definitions.h>
#include <infrastructure/accessor/cscordercustomer/cscordercustomercollectionacc.h>

#include <componentmanager/tenderconnectedfactory.h>
#include <componentmanager/tenderconnectedfactoryptr.h>
#include <componentmanager/scheduleddeliveryconnectedfactory.h>
#include <componentmanager/scheduleddeliveryconnectedfactoryptr.h>

#include <libutil/session.h>
#include <libutil/util.h>
#include "countrycodemapper.h"

namespace libtender
{
namespace componentManager
{
ApplCompMgr::ApplCompMgr()
: m_Logger( libtender::LoggerPool::getLoggerTender() ),
  m_AccessorDefined( false )
{
    METHODNAME_DEF( ApplCompMgr, ApplCompMgr )
    BLOG_TRACE_METHOD( m_Logger, fun );
}

ApplCompMgr::~ApplCompMgr()
{
    METHODNAME_DEF( ApplCompMgr, ~ApplCompMgr )
    BLOG_TRACE_METHOD( m_Logger, fun );
}

void ApplCompMgr::injectDBConnection( basar::db::aspect::ConnectionRef dbConnection )
{
    METHODNAME_DEF( ApplCompMgr, injectDBConnection )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_DBConnection = dbConnection;
}

void ApplCompMgr::injectSession( libutil::infrastructure::session::ISessionPtr session )
{
    METHODNAME_DEF( ApplCompMgr, injectSession )
    BLOG_TRACE_METHOD( m_Logger, fun );

    m_Session = session;
}

void ApplCompMgr::injectPurchaseDBParameterProxy( infrastructure::db::IPurchaseDBParameterProxyPtr proxy)
{
	m_PurchaseDBParameterProxy = proxy;
}

libtender::domMod::tender::ITenderCollectionDMPtr ApplCompMgr::getTenderCollectionDM()
{
    METHODNAME_DEF( ApplCompMgr, getTenderCollectionDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

    libtender::domMod::tender::TenderCollectionDMPtr tenderCollection( new libtender::domMod::tender::TenderCollectionDM );
    tenderCollection->injectTenderAccessor( getTenderCollectionAccessor() );
    tenderCollection->injectTenderFactory ( getTenderConnectedFactory() );

//    tenderCollection->setBranchNo(getSession()->getUser()->getAreaID());

//        customerOrderPositionCollection->injectValidator( getCustomerOrderPositionCollectionValidators(customerOrderPositionCollection->get()) );

    return tenderCollection;
}

libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr ApplCompMgr::getScheduledDeliveryCollectionDM()
{
	METHODNAME_DEF( ApplCompMgr, getScheduledDeliveryCollectionDM )
	BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::domMod::scheduledDelivery::ScheduledDeliveryCollectionDMPtr scheduledDeliveryCollection( new libtender::domMod::scheduledDelivery::ScheduledDeliveryCollectionDM() );

	scheduledDeliveryCollection->injectScheduledDeliveryAccessor( getScheduledDeliveryCollectionAccessor() );
	scheduledDeliveryCollection->injectScheduledDeliveryFactory ( getScheduledDeliveryConnectedFactory()   );

	return scheduledDeliveryCollection;
}

libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr ApplCompMgr::getPurchaseOrderProposalDM()
{
	METHODNAME_DEF( ApplCompMgr, getPurchaseOrderProposalDM )
	BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::domMod::purchaseOrderProposal::PurchaseOrderProposalDMPtr purchaseOrderProposal(
		new libtender::domMod::purchaseOrderProposal::PurchaseOrderProposalDM() );

	purchaseOrderProposal->injectPurchaseOrderProposalAccessor( getPurchaseOrderProposalAccessor() );
	purchaseOrderProposal->injectPurchaseDBParameterProxy( m_PurchaseDBParameterProxy );

	return purchaseOrderProposal;
}

libtender::domMod::tender::ITenderProgressPtr ApplCompMgr::getTenderProgressDM()
{
	METHODNAME_DEF( ApplCompMgr, getTenderProgressDM )
	BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::domMod::tender::TenderProgressDMPtr tenderProgress(
		new libtender::domMod::tender::TenderProgressDM() );

	tenderProgress->injectTenderProgressAccessor( getTenderProgressAccessor() );

	return tenderProgress;
}

libtender::domMod::article::IArticleCollectionDMPtr ApplCompMgr::getArticleCollectionDM()
{
	METHODNAME_DEF( ApplCompMgr, getArticleCollectionDM )
	BLOG_TRACE_METHOD( m_Logger, fun );
	
	libtender::domMod::article::ArticleCollectionDMPtr articleCollectionDM( new libtender::domMod::article::ArticleCollectionDM );
		
	articleCollectionDM->injectArticleAccessor( getArticleCollectionAccessor() );
	articleCollectionDM->setAreaID( m_Session->getUser()->getAreaID() );

	return articleCollectionDM;
}

libtender::domMod::article::IArticlePricePtr ApplCompMgr::getArticlePriceDM()
{
	METHODNAME_DEF( ApplCompMgr, getArticlePriceDM )
	BLOG_TRACE_METHOD( m_Logger, fun );
	
	libtender::domMod::article::ArticlePriceDMPtr articlePriceDM( new libtender::domMod::article::ArticlePriceDM() );
		
	articlePriceDM->injectArticlePriceAccessor( getArticlePriceAccessor() );
	articlePriceDM->setAreaID( m_Session->getUser()->getAreaID() );
	
	return articlePriceDM;
}

libtender::domMod::customer::ICustomerCollectionDMPtr ApplCompMgr::getCustomerCollectionDM()
{
	METHODNAME_DEF( ApplCompMgr, getCustomerCollectionDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::domMod::customer::CustomerCollectionDMPtr customerCollectionDM( new libtender::domMod::customer::CustomerCollectionDM );

	customerCollectionDM->injectCustomerAccessor( getCustomerCollectionAccessor() );

	return customerCollectionDM;
}

libtender::domMod::pharmacygroup::IPharmacyGroupCollectionDMPtr ApplCompMgr::getPharmacyGroupCollectionDM()
{
    METHODNAME_DEF( ApplCompMgr, getPharmacyGroupCollectionDM )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    libtender::domMod::pharmacygroup::PharmacyGroupCollectionDMPtr pharmacyGroupCollectionDM( new libtender::domMod::pharmacygroup::PharmacyGroupCollectionDM );
    
    pharmacyGroupCollectionDM->injectPharmacyGroupAccessor( getPharmacyGroupCollectionAccessor() );
    
    return pharmacyGroupCollectionDM;
}

libtender::domMod::partner::IPartnerCollectionDMPtr ApplCompMgr::getPartnerCollectionDM()
{
	METHODNAME_DEF( ApplCompMgr, getPartnerCollectionDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::domMod::partner::PartnerCollectionDMPtr partnerCollectionDM( new libtender::domMod::partner::PartnerCollectionDM );

	partnerCollectionDM->injectPartnerAccessor( getPartnerCollectionAccessor() );

	return partnerCollectionDM;
}

libtender::domMod::supplier::ISupplierCollectionDMPtr ApplCompMgr::getSupplierCollectionDM()
{
    METHODNAME_DEF( ApplCompMgr, getSupplierCollectionDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

    libtender::domMod::supplier::SupplierCollectionDMPtr supplierCollectionDM( new libtender::domMod::supplier::SupplierCollectionDM );
    supplierCollectionDM->injectSupplierAccessor( getSupplierCollectionAccessor() );

    return supplierCollectionDM;
}

libtender::domMod::pharmacychain::IPharmacyChainCollectionDMPtr ApplCompMgr::getPharmacyChainCollectionDM()
{
	METHODNAME_DEF( ApplCompMgr, getPharmacyChainCollectionDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::domMod::pharmacychain::PharmacyChainCollectionDMPtr pharmacyChainCollectionDM( new libtender::domMod::pharmacychain::PharmacyChainCollectionDM );

	pharmacyChainCollectionDM->injectPharmacyChainAccessor( getPharmacyChainCollectionAccessor() );

	return pharmacyChainCollectionDM;
}

libtender::domMod::scheduledDelivery::ISchedulePtr ApplCompMgr::getSchedule()
{
	METHODNAME_DEF( ApplCompMgr, getSchedule )
    BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::domMod::scheduledDelivery::ISchedulePtr schedule( new libtender::domMod::scheduledDelivery::Schedule );
	schedule->injectScheduledDeliveryCollection( getScheduledDeliveryCollectionDM() );

	return schedule;
}

libtender::domMod::tender::TenderTypeCollectionPtr ApplCompMgr::getTenderTypeCollection()
{
	static libtender::domMod::tender::TenderTypeCollectionPtr tenderTypeCollection;
	if( NULL == tenderTypeCollection.get() )
	{
		tenderTypeCollection = libtender::domMod::tender::TenderTypeCollectionPtr( new libtender::domMod::tender::TenderTypeCollection( getCountryID() ) );
	}

	return tenderTypeCollection;
}

libtender::domMod::tender::TenderStateCollectionPtr ApplCompMgr::getTenderStateCollection()
{
	static libtender::domMod::tender::TenderStateCollectionPtr tenderStateCollection;
	if( NULL == tenderStateCollection.get() )
	{
		tenderStateCollection = libtender::domMod::tender::TenderStateCollectionPtr( new libtender::domMod::tender::TenderStateCollection() );
	}

	return tenderStateCollection;
}

libtender::domMod::tender::ITenderImportCollectionPtr ApplCompMgr::getTenderImportCollection()
{
	METHODNAME_DEF( ApplCompMgr, getTenderImportCollection )
    BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::domMod::tender::TenderImportCollectionPtr importCollection( new libtender::domMod::tender::TenderImportCollection( libtender::LoggerPool::getLoggerTenderImport() ) );
	importCollection->injectTenderFactory( getTenderConnectedFactory() );
	importCollection->injectTransactionFactory( shared_from_this() );

	return importCollection;
}

libtender::domMod::scheduledDelivery::IScheduledDeliveryImportCollectionPtr ApplCompMgr::getScheduledDeliveryImportCollection(
	libtender::domMod::tender::ITenderImportCollectionPtr tenderImportCollection
		)
{
	METHODNAME_DEF( ApplCompMgr, getScheduledDeliveryImportCollection )
    BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::domMod::scheduledDelivery::ScheduledDeliveryImportCollectionPtr importCollection( new libtender::domMod::scheduledDelivery::ScheduledDeliveryImportCollection( libtender::LoggerPool::getLoggerTenderImport() ) );
	importCollection->injectScheduledDeliveryFactory( getScheduledDeliveryConnectedFactory() );
	importCollection->injectTransactionFactory( shared_from_this() );
	importCollection->injectTenderImportCollection( tenderImportCollection );

	return importCollection;
}

libtender::domMod::customerlist::ICustomerListDMPtr ApplCompMgr::getCustomerListDM()
{
	METHODNAME_DEF( ApplCompMgr, getCustomerListDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::domMod::customerlist::CustomerListDMPtr customerListDM( new libtender::domMod::customerlist::CustomerListDM() );
	customerListDM->injectCustomerAccessor( getCustomerListAccessor() );
	
	return customerListDM;
}

libtender::domMod::tender::ITenderProtocolPtr ApplCompMgr::getTenderProtocolDM()
{
	METHODNAME_DEF( ApplCompMgr, getTenderProtocolDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::domMod::tender::TenderProtocolDMPtr tenderProtocolDM( new libtender::domMod::tender::TenderProtocolDM() );
	tenderProtocolDM->injectTenderProtocolAccessor( getTenderProtocolAccessor() );
	
	return tenderProtocolDM;
}

libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr ApplCompMgr::getCscOrderCustomerCollectionDM()
{
    METHODNAME_DEF( ApplCompMgr, getTenderProtocolDM )
    BLOG_TRACE_METHOD( m_Logger, fun );

    libtender::domMod::cscordercustomer::CscOrderCustomerCollectionDMPtr cscOrderCollectionDM( new libtender::domMod::cscordercustomer::CscOrderCustomerCollectionDM() );
    cscOrderCollectionDM->injectCscOrderAccessor( getCscOrderAccessor() );
    
    return cscOrderCollectionDM;
}

IScheduledDeliveryConnectedFactoryPtr ApplCompMgr::getScheduledDeliveryConnectedFactory()
{
	METHODNAME_DEF( ApplCompMgr, getScheduledDeliveryConnectedFactory )
    BLOG_TRACE_METHOD( m_Logger, fun );

	ScheduledDeliveryConnectedFactoryPtr scheduledDeliveryConnectedFactory( new ScheduledDeliveryConnectedFactory() );
	scheduledDeliveryConnectedFactory->injectDBConnection( getDBConnection() );
	scheduledDeliveryConnectedFactory->injectPurchaseDBParameterProxy( m_PurchaseDBParameterProxy );
	scheduledDeliveryConnectedFactory->injectSession( getSession() );
	scheduledDeliveryConnectedFactory->injectTransactionFactory( shared_from_this() );

	return scheduledDeliveryConnectedFactory;
}

ITenderConnectedFactoryPtr ApplCompMgr::getTenderConnectedFactory()
{
    METHODNAME_DEF( ApplCompMgr, getTenderConnectedFactory )
    BLOG_TRACE_METHOD( m_Logger, fun );

    TenderConnectedFactoryPtr tenderConnectedFactory( new TenderConnectedFactory() );
    tenderConnectedFactory->injectDBConnection( getDBConnection() );
	tenderConnectedFactory->injectSession( getSession() );
	tenderConnectedFactory->injectTransactionFactory( shared_from_this() );

    return tenderConnectedFactory;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getTenderCollectionAccessor()
{
    METHODNAME_DEF( ApplCompMgr, getTenderCollectionAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );

	using namespace basar::db::aspect;
    libtender::infrastructure::CountryIDEnum cid = getCountryID();

	if( false == m_AccessorDefined )
	{
		// define the accessor
		AccessorDefinitionRef accessorDefinition = Manager::getInstance().getAccessorDefinition( libtender::infrastructure::accessor::tender::lit::TENDER_COLLECTION_ACC );
		AccessMethodRef am = accessorDefinition.getAccessMethod( libtender::infrastructure::accessor::tender::lit::SELECT_TENDER_BY_PATTERN );
		if( cid == libtender::infrastructure::CID_CH )
		{
			libtender::infrastructure::accessor::tender::SelectTenderByPatternCH sqlStringBuilder;
			am.push_back( sqlStringBuilder );
		}
		else
		{
			libtender::infrastructure::accessor::tender::SelectTenderByPattern sqlStringBuilder;
			am.push_back( sqlStringBuilder );
		}
		m_AccessorDefined = true;
	}

	return libutil::infrastructure::accessor::AccessorProxyPtr(
		new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::tender::lit::TENDER_COLLECTION_ACC, getDBConnection() )
			);
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getScheduledDeliveryCollectionAccessor()
{
	METHODNAME_DEF( ApplCompMgr, getScheduledDeliveryCollectionAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::infrastructure::accessor::scheduledDelivery::SelectScheduledDeliveryByPattern(); // pseudo reference
    libutil::infrastructure::accessor::IAccessorPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
													new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::scheduledDelivery::lit::SCHEDULEDDELIVERY_COLLECTION_ACC, getDBConnection() )
                                                   );
       return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getArticlePriceAccessor()
{
	METHODNAME_DEF( ApplCompMgr, getArticlePriceAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::infrastructure::accessor::article::SelectPriceByArticleCodeBG(); // pseudo reference
	libutil::infrastructure::accessor::IAccessorPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
																new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::article::lit::ARTICLE_PRICE_ACC, getDBConnection() ));
	return accessor;
}


libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getArticleCollectionAccessor()
{
	METHODNAME_DEF( ApplCompMgr, getArticleCollectionAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::infrastructure::accessor::article::SelectArticleByPattern();
	libutil::infrastructure::accessor::IAccessorPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
			                                                    new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::article::lit::ARTICLE_COLLECTION_ACC, getDBConnection() ));

	return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getCustomerCollectionAccessor()
{
	METHODNAME_DEF( ApplCompMgr, getCustomerCollectionAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::infrastructure::accessor::customer::SelectCustomerByPattern();
	libutil::infrastructure::accessor::IAccessorPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
			                                                    new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::customer::lit::CUSTOMER_COLLECTION_ACC, getDBConnection() ));
	return accessor;
}


libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getPharmacyGroupCollectionAccessor()
{
    METHODNAME_DEF( ApplCompMgr, getPharmacyGroupCollectionAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );
    
    libtender::infrastructure::accessor::pharmacygroup::SelectPharmacyGroupByPattern();
    libutil::infrastructure::accessor::IAccessorPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
        new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::pharmacygroup::lit::PHARMACY_GROUP_COLLECTION_ACC, getDBConnection() ));
    return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getPartnerCollectionAccessor()
{
	METHODNAME_DEF( ApplCompMgr, getPartnerCollectionAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::infrastructure::accessor::partner::SelectPartnerByPattern();
	libutil::infrastructure::accessor::AccessorProxyPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
			                                                        new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::partner::lit::PARTNER_COLLECTION_ACC, getDBConnection() ));
		
	return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getSupplierCollectionAccessor()
{
    METHODNAME_DEF( ApplCompMgr, getSupplierCollectionAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );

    libtender::infrastructure::accessor::supplier::SelectSupplierByPattern();
    libutil::infrastructure::accessor::AccessorProxyPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
                                                                    new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::supplier::lit::SUPPLIER_COLLECTION_ACC, getDBConnection() ));

    return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getPharmacyChainCollectionAccessor()
{
	METHODNAME_DEF( ApplCompMgr, getPharmacyChainCollectionAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );

	libtender::infrastructure::accessor::pharmacychain::SelectPharmacyChainByPattern();
	libutil::infrastructure::accessor::AccessorProxyPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
			                                                        new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::pharmacychain::lit::PHARMACYCHAIN_COLLECTION_ACC, getDBConnection() ));
		
	return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getPurchaseOrderProposalAccessor()
{
	METHODNAME_DEF(ApplCompMgr, getPurchaseOrderProposalAccessor)
	BLOG_TRACE_METHOD(m_Logger, fun);

	libtender::infrastructure::accessor::purchaseOrderProposal::SelectPurchaseOrderProposalByKey();
	libutil::infrastructure::accessor::AccessorProxyPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
		new libutil::infrastructure::accessor::AccessorProxy(libtender::infrastructure::accessor::purchaseOrderProposal::lit::PURCHASEORDERPROPOSAL_ACC, getDBConnection()));
	return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getTenderProgressAccessor()
{
	METHODNAME_DEF(ApplCompMgr, getTenderProgressAccessor)
	BLOG_TRACE_METHOD(m_Logger, fun);

	libtender::infrastructure::accessor::tenderprogress::SelectTenderProgressByPattern();
	libutil::infrastructure::accessor::AccessorProxyPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
		new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::tenderprogress::lit::TENDERPROGRESS_ACC, getDBConnection()));
	return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getCustomerListAccessor()
{
	METHODNAME_DEF(ApplCompMgr, getCustomerListAccessor)
	BLOG_TRACE_METHOD(m_Logger, fun);

	libtender::infrastructure::accessor::customerlist::SelectCustomerByPattern();
	libutil::infrastructure::accessor::AccessorProxyPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
		new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::customerlist::lit::CUSTOMER_LIST_ACC, getDBConnection(), basar::db::aspect::FULL_CACHING, false ));
	return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getTenderProtocolAccessor()
{
	METHODNAME_DEF(ApplCompMgr, getTenderProtocolAccessor)
	BLOG_TRACE_METHOD(m_Logger, fun);

	libtender::infrastructure::accessor::tenderprotocol::InsertTenderProtocol();
	libutil::infrastructure::accessor::AccessorProxyPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
		new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::tenderprotocol::lit::TENDERPROTOCOL_ACC, getDBConnection() ));
	return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getCscOrderAccessor()
{
    METHODNAME_DEF(ApplCompMgr, getCscOrderAccessor)
    BLOG_TRACE_METHOD(m_Logger, fun);

    libtender::infrastructure::accessor::cscordercustomer::SelectCscOrderCustCollByPattern();
    libutil::infrastructure::accessor::AccessorProxyPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
        new libutil::infrastructure::accessor::AccessorProxy( libtender::infrastructure::accessor::cscordercustomer::lit::CSCORDER_CUSTOMERCOLLECTION_ACC, getDBConnection() ));
    return accessor;
}

libutil::infrastructure::db::dbTransaction::IDBTransactionPtr ApplCompMgr::createDBTransaction()
{
    METHODNAME_DEF( ApplCompMgr, createDBTransaction )
    BLOG_TRACE_METHOD( m_Logger, fun );

    return libutil::infrastructure::db::dbTransaction::IDBTransactionPtr( new libutil::infrastructure::db::dbTransaction::TransactionGuard(getDBConnection()) );
}

basar::db::aspect::ConnectionRef ApplCompMgr::getDBConnection()
{
    METHODNAME_DEF( ApplCompMgr, getDBConnection() )
    BLOG_TRACE_METHOD( m_Logger, fun );

    return m_DBConnection;
}

libutil::infrastructure::session::ISessionPtr ApplCompMgr::getSession()
{
    METHODNAME_DEF( ApplCompMgr, getSession() )
    BLOG_TRACE_METHOD( m_Logger, fun );
	
    return m_Session;
}

infrastructure::CountryIDEnum ApplCompMgr::getCountryID()
{
	return infrastructure::CountryCodeMapper::getCountryID( getSession()->getUser()->getAreaID() );
}

} // end namnespace componentManager
} // end namnespace libtender
