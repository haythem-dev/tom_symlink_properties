#ifndef GUARD_TENDER_COMPONENTMANAGER_TENDERAPPLCOMPMGR_H
#define GUARD_TENDER_COMPONENTMANAGER_TENDERAPPLCOMPMGR_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libutil/irunnableptr.h>

// vc
#include "mainvcptr.h"
#include "tenderoverviewvcptr.h"
#include "tendermaintainvcptr.h"
#include "customerselectionvcptr.h"
#include "articlepickervcptr.h"
#include "customerpickervcptr.h"
#include "pharmacygrouppickervcptr.h"
#include "partnerpickervcptr.h"
#include "supplierpickervcptr.h"
#include "pharmacychainpickervcptr.h"
#include "ischeduleddeliverymaintainvcptr.h"
#include "icscordermaintainvcptr.h"
#include "scheduleddeliverymaintainvcptr.h"
#include "tenderimport/tenderimportvcptr.h"
#include "customerlistmaintainvcptr.h"
#include "tenderinvoicesvcptr.h"
#include "tenderprotocolvcptr.h"

#include "Itendersearchcriteria.h"
#include "itendersearchcriteriaptr.h"

#include "itendercontractinfo.h"
#include "itendercontractinfoptr.h"

#include "itenderarticleinfo.h"
#include "itenderarticleinfoptr.h"
// uc
#include "iusecasegetter.h"
#include "ipickerucptr.h"
#include "tenderimport/tenderimportcheckerptr.h"

// dm
#include <domainmodule/tender/itendercollectiondmptr.h>
#include <domainmodule/article/iarticlepricedmptr.h>
#include <domainmodule/article/iarticlecollectiondmptr.h>
#include <domainmodule/customer/icustomercollectiondmptr.h>
#include <domainmodule/pharmacygroup/ipharmacygroupcollectiondmptr.h>
#include <domainmodule/partner/ipartnercollectiondmptr.h>
#include <domainmodule/supplier/isuppliercollectiondmptr.h>
#include <domainmodule/pharmacychain/ipharmacychaincollectiondmptr.h>
#include <domainmodule/scheduleddelivery/ischeduleddeliverycollectiondmptr.h>
#include <domainmodule/scheduleddelivery/ischedule.h>
#include <domainmodule/purchaseorderproposal/ipurchaseorderproposalptr.h>
#include <domainmodule/tender/itenderprogressptr.h>
#include <domainmodule/customerlist/icustomerlistdmptr.h>
#include <domainmodule/customerlist/icustomerlistdm.h>
#include <domainmodule/tender/itenderprotocolptr.h>
#include <domainmodule/tender/itenderprotocol.h>
#include <domainmodule/cscorder/icscorderptr.h>
#include <domainmodule/cscorder/icscorder.h>
#include <domainmodule/cscordercustomer/icscordercustomercollectiondmptr.h>

#include "infrastructure/cscclient/icscclientconnectionptr.h"

// libtender
#include <infrastructure/libtenderapplcompmgrptr.h>
#include <infrastructure/purchasedbconn/ipurchasedbparameterproxyptr.h>
#include <infrastructure/cscclient/icscclientparameterproxyptr.h>

// libutil
#include <libutil/applcomponent.h>
#include <boost/enable_shared_from_this.hpp>
#include <libutil/searchyiterator.h>
#include <libutil/dbconnection.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//
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

//-------------------------------------------------------------------------------------------------//
// class declaration setcion
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace componentManager
    {
        class ApplCompMgr : public libutil::componentManager::ApplCompMgrBase,
                            public IUseCaseGetter,
                            public boost::enable_shared_from_this< ApplCompMgr >,
							public libutil::infrastructure::db::dbTransaction::IDBTransactionFactory
        {
            public:
                ApplCompMgr();
                virtual ~ApplCompMgr();

				void injectPurchaseDBParameterProxy( libtender::infrastructure::db::IPurchaseDBParameterProxyPtr );
				void injectParameterGateway( libutil::infrastructure::parameter::IParameterGatewayPtr );
				void injectCscClientConnection(	libtender::infrastructure::cscclient::ICscClientConnectionPtr cscClientConnection );
				void injectParameterProxy( libtender::infrastructure::cscclient::ICscClientParameterProxyPtr );
               /////////////////////////////////////////////////////////////////////
                // use case
                /////////////////////////////////////////////////////////////////////
                libutil::useCase::IRunnablePtr                                          getMainUC();
                virtual libutil::useCase::IRunnablePtr                                  getTenderOverviewUC();
                virtual libutil::useCase::IRunnablePtr                                  getOMGOrderUC( libtender::domMod::tender::ITenderPtr tender );
				virtual libutil::useCase::IRunnablePtr                                  getTenderMaintainUC( libtender::domMod::tender::ITenderPtr tender, 
																						libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr ptr = 
																						libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr(), bool cloneDeliveries = false );
				virtual tender::useCase::IPickerUCPtr                                   getCustomerSelectionUC(basar::db::aspect::AccessorPropertyTableRef, basar::Int32 customerNo, basar::Int32 pharmacyChainNo);
                virtual tender::useCase::IPickerUCPtr                                   getArticlePickerUC ( basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::AccessorPropertyTable_YIterator() );
				virtual tender::useCase::IPickerUCPtr                                   getCustomerPickerUC( basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::AccessorPropertyTable_YIterator() );
                virtual tender::useCase::IPickerUCPtr                                   getPharmacyGroupPickerUC( basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::AccessorPropertyTable_YIterator() );
				virtual tender::useCase::IPickerUCPtr                                   getPartnerPickerUC ( basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::AccessorPropertyTable_YIterator() );
                virtual tender::useCase::IPickerUCPtr                                   getSupplierPickerUC ( basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::AccessorPropertyTable_YIterator() );
				virtual tender::useCase::IPickerUCPtr                                   getPharmacyChainPickerUC ( basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::AccessorPropertyTable_YIterator() );
				virtual libutil::useCase::IRunnablePtr									getScheduledDeliveryMaintainUC( libtender::domMod::tender::ITenderPtr tender, 
																						libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery, 
																						libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr scheduledDeliveryCollection,
																						bool needsCustomerSelection);
				virtual libutil::useCase::IRunnablePtr									getTenderImportUC();
				virtual libutil::useCase::IRunnablePtr									getCustomerListMaintainUC( libtender::domMod::customerlist::ICustomerListDMPtr );
				virtual libutil::useCase::IRunnablePtr									getTenderProtocolUC( libtender::domMod::tender::ITenderPtr tender, basar::Int16 area );
				virtual libutil::useCase::IRunnablePtr									getInvoiceUC( libtender::domMod::tender::ITenderProgressPtr tenderProgress );
                virtual libutil::useCase::IRunnablePtr                                  getCSCOrderUC(libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr );
                virtual libutil::useCase::IRunnablePtr                                  getCSCOrderMaintainUC( libtender::domMod::tender::ITenderPtr, libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr );
                virtual tender::useCase::ITenderStateUCPtr                              getTenderStateUC( );

                libtender::domMod::tender::ITenderCollectionDMPtr                       getTenderCollectionDM();
				libtender::domMod::article::IArticlePricePtr							getArticlePriceDM();
				libtender::domMod::article::IArticleCollectionDMPtr                     getArticleCollectionDM();
				libtender::domMod::customer::ICustomerCollectionDMPtr                   getCustomerCollectionDM();
				libtender::domMod::pharmacygroup::IPharmacyGroupCollectionDMPtr         getPharmacyGroupCollectionDM();
				libtender::domMod::partner::IPartnerCollectionDMPtr                     getPartnerCollectionDM();
                libtender::domMod::supplier::ISupplierCollectionDMPtr                   getSupplierCollectionDM();
				libtender::domMod::pharmacychain::IPharmacyChainCollectionDMPtr         getPharmacyChainCollectionDM();
				libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr	getScheduledDeliveryCollectionDM();
				libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr		getPurchaseOrderProposalDM();
				libtender::domMod::tender::ITenderProgressPtr							getTenderProgressDM();
				libtender::domMod::customerlist::ICustomerListDMPtr						getCustomerListDM();
				libtender::domMod::tender::ITenderProtocolPtr							getTenderProtocolDM();
				libtender::domMod::cscorder::ICscOrderPtr								getCscOrderDM();
                libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr   getCscOrderCollectionDM();

            protected:
				libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr	getDBConnectionData();
            private:
                ApplCompMgr( const ApplCompMgr& );
                ApplCompMgr& operator=( const ApplCompMgr& );

                virtual void                                                            doClear();

                /////////////////////////////////////////////////////////////////////
                // view connector
                /////////////////////////////////////////////////////////////////////
                tender::viewConn::MainVCPtr                                             getMainVC();
                tender::viewConn::TenderOverviewVCPtr                                   getTenderOverviewVC();
                tender::viewConn::TenderMaintainVCPtr                                   getTenderMaintainVC();
                tender::viewConn::CustomerSelectionVCPtr                                getCustomerSelectionVC();
				tender::viewConn::ArticlePickerVCPtr                                    getArticlePickerVC ();
				tender::viewConn::CustomerPickerVCPtr                                   getCustomerPickerVC();
                tender::viewConn::PharmacyGroupPickerVCPtr                              getPharmacyGroupPickerVC();
				tender::viewConn::PartnerPickerVCPtr                                    getPartnerPickerVC ();
                tender::viewConn::SupplierPickerVCPtr                                   getSupplierPickerVC ();
				tender::viewConn::PharmacyChainPickerVCPtr								getPharmacyChainPickerVC();
				tender::viewConn::TenderImportVCPtr                                     getTenderImportVC();
				tender::viewConn::TenderInvoicesVCPtr									getTenderInvoiceVC();
				tender::viewConn::CustomerListMaintainVCPtr								getCustomerListMaintainVC();
				tender::viewConn::TenderProtocolVCPtr									getTenderProtocolVC();

				tender::viewConn::ITenderSearchCriteriaPtr                              getSearchCriteriaFrance();
				tender::viewConn::ITenderSearchCriteriaPtr                              getSearchCriteriaBulgaria();
				tender::viewConn::ITenderSearchCriteriaPtr                              getSearchCriteriaSerbia();
                tender::viewConn::ITenderSearchCriteriaPtr                              getSearchCriteriaSwitzerland();

				tender::viewConn::ITenderContractInfoPtr								getContractInfoFrance();
				tender::viewConn::ITenderContractInfoPtr								getContractInfoBulgaria();
				tender::viewConn::ITenderContractInfoPtr								getContractInfoSerbia();
                tender::viewConn::ITenderContractInfoPtr								getContractInfoSwitzerland();
                tender::viewConn::ITenderArticleInfoPtr                                 getArticleInfoSwitzerland();
                tender::viewConn::ITenderArticleInfoPtr                                 getArticleInfo();

				tender::viewConn::IScheduledDeliveryMaintainVCPtr                       getScheduledDeliveryMaintainVC(bool needsCustomerCollection);
                tender::viewConn::ICSCOrderMaintainVCPtr                                getCSCOrderMaintainVC();

				libtender::domMod::tender::TenderStateCollectionPtr						getStateCollectionWithTranslation();


                /////////////////////////////////////////////////////////////////////
                // domain modules
                /////////////////////////////////////////////////////////////////////

                /////////////////////////////////////////////////////////////////////
                // others
                /////////////////////////////////////////////////////////////////////
				tender::useCase::TenderImportCheckerPtr									 getTenderImportChecker();
				basar::I18nString														 getDokuLink();


                /////////////////////////////////////////////////////////////////////
                // sub component manager(s)
                /////////////////////////////////////////////////////////////////////
                libtender::componentManager::ApplCompMgrPtr                              getLIBtenderApplCompMgr();


                /////////////////////////////////////////////////////////////////////
                // db connection
                /////////////////////////////////////////////////////////////////////
                basar::db::aspect::ConnectionRef                                         getDBConnection();
				virtual libutil::infrastructure::db::dbTransaction::IDBTransactionPtr	 createDBTransaction();
				libtender::infrastructure::cscclient::ICscClientConnectionPtr			 getCscClientConnection();
                libtender::infrastructure::cscclient::ICscClientParameterProxyPtr        getCscClientParameterProxy();
																						 
                ///////////////////////////////////////////////////////////////////		 
                // member variables														 
                ///////////////////////////////////////////////////////////////////		 
                const log4cplus::Logger	                                                 m_Logger;
                libtender::componentManager::ApplCompMgrPtr                              m_LIBtenderApplCompMgr;
                tender::viewConn::MainVCPtr                                              m_MainVC;
                tender::viewConn::TenderOverviewVCPtr                                    m_TenderOverviewVC;
                tender::viewConn::TenderMaintainVCPtr                                    m_TenderMaintainVC;
                tender::viewConn::CustomerSelectionVCPtr                                 m_CustomerSelectionVC;
				tender::viewConn::ArticlePickerVCPtr                                     m_ArticlePickerVC;
				tender::viewConn::CustomerPickerVCPtr                                    m_CustomerPickerVC;
                tender::viewConn::PharmacyGroupPickerVCPtr                               m_PharmacyGroupPickerVC;
				tender::viewConn::PartnerPickerVCPtr                                     m_PartnerPickerVC;
				tender::viewConn::SupplierPickerVCPtr                                    m_SupplierPickerVC;
				tender::viewConn::PharmacyChainPickerVCPtr								 m_PharmacyChainPickerVC;
				tender::viewConn::ITenderSearchCriteriaPtr                               m_SearchCriteria;
				tender::viewConn::ITenderContractInfoPtr								 m_ContractInfo;
                tender::viewConn::ITenderArticleInfoPtr                                  m_ArticleInfo;
				tender::viewConn::IScheduledDeliveryMaintainVCPtr                        m_ScheduledDeliveryMaintainVC;
                tender::viewConn::ICSCOrderMaintainVCPtr                                 m_CSCOrderMaintainVC;
				tender::viewConn::CustomerListMaintainVCPtr								 m_CustomerListMaintainVC;
				tender::viewConn::TenderInvoicesVCPtr									 m_TenderInvoicesVC;
				tender::viewConn::TenderProtocolVCPtr									 m_TenderProtocolVC;

				libutil::infrastructure::parameter::IParameterGatewayPtr				 m_ParameterGateway;
				libtender::infrastructure::cscclient::ICscClientConnectionPtr			 m_CscClientConnection;
				libtender::infrastructure::cscclient::ICscClientParameterProxyPtr        m_ParameterProxy;
		};

    } // end namnespace componentManager
} // end namnespace tender

#endif
