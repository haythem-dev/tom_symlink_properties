//----------------------------------------------------------------------------//
// include section
//----------------------------------------------------------------------------//
// global
#include "tenderapplcompmgr.h"
#include "loggerpool/tenderloggerpool.h"

#include <libtender/domainmodule/constants.h>

// vc
#include "mainvc.h"
#include "tenderoverviewvc.h"
#include "tendermaintainvc.h"
#include "customerselectionvc.h"
#include "articlepickervc.h"
#include "customerpickervc.h"
#include "pharmacygrouppickervc.h"
#include "partnerpickervc.h"
#include "supplierpickervc.h"
#include "pharmacychainpickervc.h"
#include "tenderimport/tenderimportvc.h"
#include "tenderinvoicesvc.h"
#include "tenderprotocolvc.h"

#include "tendersearchcriteriafrance.h"
#include "tendersearchcriteriabulgaria.h"
#include "tendersearchcriteriaserbia.h"
#include "tendersearchcriteriaswitzerland.h"

#include "tendercontractinfofrance.h"
#include "tendercontractinfobulgaria.h"
#include "tendercontractinfoserbia.h"
#include "tendercontractinfoswitzerland.h"
#include "tenderarticleinfoswitzerland.h"
#include "tenderarticleinfo.h"

#include "scheduleddeliverymaintainvc.h"
#include "customerlistmaintainvc.h"

#include "cscordermaintainvc.h"

// uc
#include "mainucptr.h"
#include "mainuc.h"
#include "tenderoverviewucptr.h"
#include "tenderoverviewuc.h"
#include "tendermaintainucptr.h"
#include "tendermaintainuc.h"
#include "customerselectionucptr.h"
#include "customerselectionuc.h"
#include "articlepickerucptr.h"
#include "articlepickeruc.h"
#include "customerpickerucptr.h"
#include "customerpickeruc.h"
#include "pharmacygrouppickerucptr.h"
#include "pharmacygrouppickeruc.h"
#include "partnerpickerucptr.h"
#include "partnerpickeruc.h"
#include "supplierpickerucptr.h"
#include "supplierpickeruc.h"
#include "pharmacychainpickerucptr.h"
#include "pharmacychainpickeruc.h"
#include "scheduleddeliverymaintainucptr.h"
#include "scheduleddeliverymaintainuc.h"
#include "tenderimport/tenderimportuc.h"
#include "tenderimport/tenderimportucptr.h"
#include "tenderimport/tenderimportfilereader.h"
#include "tenderimport/tenderimportfilereaderptr.h"
#include "tenderimport/iprogress.h"
#include "tenderimport/iprogressptr.h"
#include "tenderimport/ierrorhandler.h"
#include "tenderimport/ierrorhandlerptr.h"
#include "tenderimport/tenderimportchecker.h"
#include "customerlistmaintainucptr.h"
#include "customerlistmaintainuc.h"
#include "tenderprotocolucptr.h"
#include "tenderprotocoluc.h"
#include "tenderinvoiceucptr.h"
#include "tenderinvoiceuc.h"
#include "omgorderuc.h"
#include "omgorderucptr.h"
#include "cscorderuc.h"
#include "cscorderucptr.h"
#include "cscordermaintainucptr.h"
#include "cscordermaintainuc.h"

#include "ipickeruc.h"
#include "ipickerucptr.h"
#include "tenderstateucptr.h"
#include "tenderstateuc.h"

#include "infrastructure/cscclient/icscclientconnection.h"

//dommod
#include "domainmodule/cscorder/cscorderdmptr.h"
#include "domainmodule/cscorder/cscorderdm.h"

// libtender
#include <infrastructure/libtenderapplcompmgr.h>
#include <libutil/infrastructure/parameter/iparametergateway.h>

#include <libtender\domainmodule\tender\tenderstatecollection.h>

// libutil
#include <libutil/util.h>
#include <libutil/dbconnection.h>

// basar
#include <libbasarcmnutil_logging.h>
#include <libbasardbsql_databaseinfo.h>
#include <libbasarlogin.h>

//----------------------------------------------------------------------------//
// class definition setcion
//----------------------------------------------------------------------------//
namespace tender
{
    namespace componentManager
    {
        ApplCompMgr::ApplCompMgr()
        : m_Logger( libtender::LoggerPool::getLoggerTender() )
        {
            METHODNAME_DEF( ApplCompMgr, ApplCompMgr )
            BLOG_TRACE_METHOD( m_Logger, fun );
        }

        ApplCompMgr::~ApplCompMgr()
        {
            METHODNAME_DEF( ApplCompMgr, ~ApplCompMgr )
            BLOG_TRACE_METHOD( m_Logger, fun );
        }
		
        void ApplCompMgr::doClear()
        {
            METHODNAME_DEF( ApplCompMgr, doClear )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if(0 != m_MainVC.get() ) 
			{ 
				m_MainVC->shutdown();
			}
        }

        libtender::componentManager::ApplCompMgrPtr ApplCompMgr::getLIBtenderApplCompMgr()
        {
            METHODNAME_DEF( ApplCompMgr, getLIBtenderApplCompMgr )
            BLOG_TRACE_METHOD( m_Logger, fun );

            if( 0 == m_LIBtenderApplCompMgr.get() )
            {
                m_LIBtenderApplCompMgr = libtender::componentManager::ApplCompMgrPtr( new libtender::componentManager::ApplCompMgr() );
                m_LIBtenderApplCompMgr->injectDBConnection( getDBConnection() );
                m_LIBtenderApplCompMgr->injectSession(getSession());
            }

            return m_LIBtenderApplCompMgr;
        }

		void ApplCompMgr::injectPurchaseDBParameterProxy( libtender::infrastructure::db::IPurchaseDBParameterProxyPtr proxy )
		{
			getLIBtenderApplCompMgr()->injectPurchaseDBParameterProxy( proxy );
		}

		void ApplCompMgr::injectParameterGateway( libutil::infrastructure::parameter::IParameterGatewayPtr parameterGateway )
		{
			m_ParameterGateway = parameterGateway;
		}

		void ApplCompMgr::injectCscClientConnection( libtender::infrastructure::cscclient::ICscClientConnectionPtr cscClientConnection )
		{
			m_CscClientConnection = cscClientConnection;
		}

		void ApplCompMgr::injectParameterProxy( libtender::infrastructure::cscclient::ICscClientParameterProxyPtr parameterProxy)
        {
			METHODNAME_DEF( ApplCompMgr, injectParameterProxy )
            BLOG_TRACE_METHOD( m_Logger, fun );

            m_ParameterProxy = parameterProxy;
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // <VC getting>
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        tender::viewConn::MainVCPtr ApplCompMgr::getMainVC()
        {
            METHODNAME_DEF( ApplCompMgr, getMainVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            if( 0 == m_MainVC.get() )
            {
                m_MainVC = tender::viewConn::MainVCPtr( new tender::viewConn::MainVC( getDBConnectionData()->getDBInfo().dbServer,
																					  getDBConnectionData()->getDBInfo().database,
																					  getSession()->getUser()->getUserName(),
																					  getDokuLink() ) );
            }

            return m_MainVC;
        }

        tender::viewConn::TenderOverviewVCPtr ApplCompMgr::getTenderOverviewVC()
        {
            METHODNAME_DEF( ApplCompMgr, getTenderOverviewVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            if( 0 == m_TenderOverviewVC.get() )
            {
                m_TenderOverviewVC = tender::viewConn::TenderOverviewVCPtr( new tender::viewConn::TenderOverviewVC( getMainVC().get() ) );

                tender::viewConn::ITenderSearchCriteriaPtr searchCriteria;

				switch( getSession()->getUser()->getAreaID() )
				{
					case constants::REGION_BG: searchCriteria = getSearchCriteriaBulgaria();
						 break;
					case constants::REGION_RS: searchCriteria = getSearchCriteriaSerbia(); 
						 break;
					case constants::REGION_FR: searchCriteria = getSearchCriteriaFrance();	
						 break;
                    case constants::REGION_CH: searchCriteria = getSearchCriteriaSwitzerland();
						 break;
				}

                searchCriteria->setParent( m_TenderOverviewVC.get() );
                m_TenderOverviewVC->injectSearchCriteria( searchCriteria );

				m_TenderOverviewVC->setArea( getSession()->getUser()->getAreaID() );
				m_TenderOverviewVC->initialize();

                Qt::WindowFlags flags = m_TenderOverviewVC->windowFlags();
                m_TenderOverviewVC->setParent( getMainVC().get() );
                m_TenderOverviewVC->setWindowFlags( flags );
            }

            return m_TenderOverviewVC;
        }

        tender::viewConn::TenderMaintainVCPtr ApplCompMgr::getTenderMaintainVC()
        {
            METHODNAME_DEF( ApplCompMgr, getTenderMaintainVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            if( 0 == m_TenderMaintainVC.get() )
            {
                m_TenderMaintainVC = tender::viewConn::TenderMaintainVCPtr( new tender::viewConn::TenderMaintainVC( getMainVC().get(), getSession()->getUser()->getAreaID() ) );

				tender::viewConn::ITenderContractInfoPtr contractInfo;
                tender::viewConn::ITenderArticleInfoPtr articleInfo;

				switch( getSession()->getUser()->getAreaID() )
				{
					case constants::REGION_BG:
                                                contractInfo = getContractInfoBulgaria();
                                                articleInfo = getArticleInfo();
						 break;
					case constants::REGION_RS:
                                                contractInfo = getContractInfoSerbia();
                                                articleInfo = getArticleInfo();
						 break;
					case constants::REGION_FR:
                                                contractInfo = getContractInfoFrance();
                                                articleInfo = getArticleInfo();
						 break;
					case constants::REGION_CH:
                                                contractInfo = getContractInfoSwitzerland();
                                                articleInfo = getArticleInfoSwitzerland();
						 break;
				}

				contractInfo->setParent( m_TenderMaintainVC.get() );
                articleInfo->setParent( m_TenderMaintainVC.get() );

				m_TenderMaintainVC->injectContractInfo( contractInfo );
                m_TenderMaintainVC->injectArticleInfo( articleInfo );

                m_TenderMaintainVC->setupScheduleTab( );
				basar::I18nString popFunctionality;
				m_ParameterGateway->getParameterValue( getSession()->getUser()->getAreaID(), "POP_FUNCTIONALITY", popFunctionality );

				m_TenderMaintainVC->setPOPFunctionality( popFunctionality );
            }

            return m_TenderMaintainVC;
        }

        tender::viewConn::CustomerSelectionVCPtr ApplCompMgr::getCustomerSelectionVC()
		{
			METHODNAME_DEF( ApplCompMgr, getCustomerSelectionVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if( 0 == m_CustomerSelectionVC.get() )
			{
                m_CustomerSelectionVC = tender::viewConn::CustomerSelectionVCPtr( new tender::viewConn::CustomerSelectionVC( getMainVC().get() ) );
			}

			return m_CustomerSelectionVC;
		}

		tender::viewConn::ArticlePickerVCPtr ApplCompMgr::getArticlePickerVC()
		{
			METHODNAME_DEF( ApplCompMgr, getArticlePickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if( 0 == m_ArticlePickerVC.get() )
			{
				m_ArticlePickerVC = tender::viewConn::ArticlePickerVCPtr( new tender::viewConn::ArticlePickerVC( getMainVC().get() ) );

				if( constants::REGION_FR ==  getSession()->getUser()->getAreaID() )
				{
					m_ArticlePickerVC->injectTenderTypeCollection( getLIBtenderApplCompMgr()->getTenderTypeCollection() );
				}
			}

			return m_ArticlePickerVC;
		}

		tender::viewConn::CustomerPickerVCPtr ApplCompMgr::getCustomerPickerVC()
		{
			METHODNAME_DEF( ApplCompMgr, getCustomerPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if( 0 == m_CustomerPickerVC.get() )
			{
				m_CustomerPickerVC = tender::viewConn::CustomerPickerVCPtr( new tender::viewConn::CustomerPickerVC( getMainVC().get() ) );
			}

			return m_CustomerPickerVC;
		}

        tender::viewConn::PharmacyGroupPickerVCPtr ApplCompMgr::getPharmacyGroupPickerVC()
        {
            METHODNAME_DEF( ApplCompMgr, getPharmacyGroupPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            if( 0 == m_PharmacyGroupPickerVC.get() )
            {
                m_PharmacyGroupPickerVC = tender::viewConn::PharmacyGroupPickerVCPtr( new tender::viewConn::PharmacyGroupPickerVC( getMainVC().get() ) );
            }
            
            return m_PharmacyGroupPickerVC;
        }

		tender::viewConn::PartnerPickerVCPtr ApplCompMgr::getPartnerPickerVC()
		{
            METHODNAME_DEF( ApplCompMgr, getPartnerPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if( 0 == m_PartnerPickerVC.get() )
			{
				m_PartnerPickerVC = tender::viewConn::PartnerPickerVCPtr( new tender::viewConn::PartnerPickerVC( getMainVC().get() ) );
				m_PartnerPickerVC->injectTenderTypeCollection( getLIBtenderApplCompMgr()->getTenderTypeCollection() );
			}

			return m_PartnerPickerVC;
		}

        tender::viewConn::SupplierPickerVCPtr ApplCompMgr::getSupplierPickerVC()
        {
            METHODNAME_DEF( ApplCompMgr, getSupplierPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            if( 0 == m_SupplierPickerVC.get() )
            {
                m_SupplierPickerVC = tender::viewConn::SupplierPickerVCPtr( new tender::viewConn::SupplierPickerVC( getMainVC().get() ) );
            }

            return m_SupplierPickerVC;
        }

		tender::viewConn::PharmacyChainPickerVCPtr ApplCompMgr::getPharmacyChainPickerVC()
		{
            METHODNAME_DEF( ApplCompMgr, getPharmacyChainPickerVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if( 0 == m_PharmacyChainPickerVC.get() )
			{
				m_PharmacyChainPickerVC = tender::viewConn::PharmacyChainPickerVCPtr( new tender::viewConn::PharmacyChainPickerVC( getMainVC().get() ) );
			}

			return m_PharmacyChainPickerVC;
		}

		tender::viewConn::TenderImportVCPtr ApplCompMgr::getTenderImportVC()
		{
            METHODNAME_DEF( ApplCompMgr, getTenderImportVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			tender::viewConn::TenderImportVCPtr tenderImportVC( new tender::viewConn::TenderImportVC( getMainVC().get() ) );

			return tenderImportVC;
		}

		tender::viewConn::TenderInvoicesVCPtr ApplCompMgr::getTenderInvoiceVC()
		{
			METHODNAME_DEF( ApplCompMgr, getTenderInvoicesVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			tender::viewConn::TenderInvoicesVCPtr tenderInvoiceVC( new tender::viewConn::TenderInvoicesVC( getMainVC().get() ) );

			return tenderInvoiceVC;
		}

		tender::viewConn::TenderProtocolVCPtr ApplCompMgr::getTenderProtocolVC()
		{
			METHODNAME_DEF( ApplCompMgr, getTenderProtocolVCPtr )
            BLOG_TRACE_METHOD( m_Logger, fun );

			tender::viewConn::TenderProtocolVCPtr tenderProtocolVC( new tender::viewConn::TenderProtocolVC( getMainVC().get() ) );

			return tenderProtocolVC;
		}

		tender::viewConn::ITenderSearchCriteriaPtr ApplCompMgr::getSearchCriteriaFrance()
		{
			METHODNAME_DEF( ApplCompMgr, getSearchCriteriaFrance )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if( 0 == m_SearchCriteria.get() )
			{
				m_SearchCriteria = tender::viewConn::ITenderSearchCriteriaPtr( new tender::viewConn::TenderSearchCriteriaFrance() );
				m_SearchCriteria->injectStatusMapping( getLIBtenderApplCompMgr()->getTenderStateCollection() );
				m_SearchCriteria->injectTypeMapping( getLIBtenderApplCompMgr()->getTenderTypeCollection() );
			}

			return m_SearchCriteria;
		}

		tender::viewConn::ITenderSearchCriteriaPtr ApplCompMgr::getSearchCriteriaBulgaria()
		{
			METHODNAME_DEF( ApplCompMgr, getSearchCriteriaBulgaria )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if( 0 == m_SearchCriteria.get() )
			{
				m_SearchCriteria = tender::viewConn::ITenderSearchCriteriaPtr( new tender::viewConn::TenderSearchCriteriaBulgaria() );
				m_SearchCriteria->injectStatusMapping( getLIBtenderApplCompMgr()->getTenderStateCollection() );
				m_SearchCriteria->injectTypeMapping( getLIBtenderApplCompMgr()->getTenderTypeCollection() );
			}

			return m_SearchCriteria;
		}

        tender::viewConn::ITenderSearchCriteriaPtr ApplCompMgr::getSearchCriteriaSwitzerland()
        {
            METHODNAME_DEF( ApplCompMgr, getSearchCriteriaSwitzerland )
            BLOG_TRACE_METHOD( m_Logger, fun );

            if( 0 == m_SearchCriteria.get() )
            {
                m_SearchCriteria = tender::viewConn::ITenderSearchCriteriaPtr( new tender::viewConn::TenderSearchCriteriaSwitzerland() );
				m_SearchCriteria->injectStatusMapping( getStateCollectionWithTranslation() ); 
                m_SearchCriteria->injectTypeMapping( getLIBtenderApplCompMgr()->getTenderTypeCollection() );
            }

            return m_SearchCriteria;
        }


		tender::viewConn::ITenderSearchCriteriaPtr ApplCompMgr::getSearchCriteriaSerbia()
		{
			METHODNAME_DEF( ApplCompMgr, getSearchCriteriaSerbia )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if( 0 == m_SearchCriteria.get() )
			{
				m_SearchCriteria = tender::viewConn::ITenderSearchCriteriaPtr( new tender::viewConn::TenderSearchCriteriaSerbia() );
				m_SearchCriteria->injectStatusMapping( getLIBtenderApplCompMgr()->getTenderStateCollection() );
				m_SearchCriteria->injectTypeMapping( getLIBtenderApplCompMgr()->getTenderTypeCollection() );
			}

			return m_SearchCriteria;
		}

		tender::viewConn::ITenderContractInfoPtr ApplCompMgr::getContractInfoFrance()
		{
			METHODNAME_DEF( ApplCompMgr, getContractInfoFrance )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if( 0 == m_ContractInfo )
			{
				m_ContractInfo = tender::viewConn::ITenderContractInfoPtr( new tender::viewConn::TenderContractInfoFrance() );
				m_ContractInfo->injectStatusMapping( getLIBtenderApplCompMgr()->getTenderStateCollection() );
				m_ContractInfo->injectTypeMapping( getLIBtenderApplCompMgr()->getTenderTypeCollection() );
			}
			
			return m_ContractInfo;
		}

		tender::viewConn::ITenderContractInfoPtr ApplCompMgr::getContractInfoBulgaria()
		{
			METHODNAME_DEF( ApplCompMgr, getContractInfoBulgaria )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if( 0 == m_ContractInfo )
			{
				m_ContractInfo = tender::viewConn::ITenderContractInfoPtr( new tender::viewConn::TenderContractInfoBulgaria() );
				m_ContractInfo->injectStatusMapping( getLIBtenderApplCompMgr()->getTenderStateCollection() );
				m_ContractInfo->injectTypeMapping( getLIBtenderApplCompMgr()->getTenderTypeCollection() );
			}

			return m_ContractInfo;
		}
		
		tender::viewConn::ITenderContractInfoPtr ApplCompMgr::getContractInfoSerbia()
		{
			METHODNAME_DEF( ApplCompMgr, getContractInfoSerbia )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if( 0 == m_ContractInfo )
			{
				m_ContractInfo = tender::viewConn::ITenderContractInfoPtr( new tender::viewConn::TenderContractInfoSerbia() );
				m_ContractInfo->injectStatusMapping( getLIBtenderApplCompMgr()->getTenderStateCollection() );
				m_ContractInfo->injectTypeMapping( getLIBtenderApplCompMgr()->getTenderTypeCollection() );
			}

			return m_ContractInfo;
		}

        tender::viewConn::ITenderContractInfoPtr ApplCompMgr::getContractInfoSwitzerland()
        {
            METHODNAME_DEF( ApplCompMgr, getContractInfoSwitzerland )
            BLOG_TRACE_METHOD( m_Logger, fun );
            if( 0 == m_ContractInfo )
            {
                m_ContractInfo = tender::viewConn::ITenderContractInfoPtr( new tender::viewConn::TenderContractInfoSwitzerland() );
                m_ContractInfo->injectStatusMapping( getStateCollectionWithTranslation() );
                m_ContractInfo->injectTypeMapping( getLIBtenderApplCompMgr()->getTenderTypeCollection() );
            }
            return m_ContractInfo;
        }
        
        tender::viewConn::ITenderArticleInfoPtr ApplCompMgr::getArticleInfoSwitzerland()
        {
            METHODNAME_DEF( ApplCompMgr, getArticleInfoSwitzerland )
            BLOG_TRACE_METHOD( m_Logger, fun );
            if( 0 == m_ArticleInfo )
            {
                m_ArticleInfo = tender::viewConn::ITenderArticleInfoPtr( new tender::viewConn::TenderArticleInfoSwitzerland( getMainVC().get() ) );
            }
            return m_ArticleInfo;
        }

        tender::viewConn::ITenderArticleInfoPtr ApplCompMgr::getArticleInfo()
        {
            METHODNAME_DEF( ApplCompMgr, getArticleInfo )
            BLOG_TRACE_METHOD( m_Logger, fun );
            if( 0 == m_ArticleInfo )
            {
                m_ArticleInfo = tender::viewConn::ITenderArticleInfoPtr( new tender::viewConn::TenderArticleInfo( getMainVC().get(), getSession()->getUser()->getAreaID() ) );
            }
            return m_ArticleInfo;
        }

        tender::viewConn::ICSCOrderMaintainVCPtr ApplCompMgr::getCSCOrderMaintainVC()
        {
            METHODNAME_DEF( ApplCompMgr, getCSCOrderMaintainVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            if( 0 == m_CSCOrderMaintainVC.get() )
            {
                m_CSCOrderMaintainVC = tender::viewConn::ICSCOrderMaintainVCPtr( new tender::viewConn::CSCOrderMaintainVC( getMainVC().get() ) );
            }
            return m_CSCOrderMaintainVC;
        }

		libtender::domMod::tender::TenderStateCollectionPtr	ApplCompMgr::getStateCollectionWithTranslation()
		{
			libtender::domMod::tender::TenderStateCollectionPtr stateCollection = getLIBtenderApplCompMgr()->getTenderStateCollection();
			stateCollection->setStates(QApplication::tr("Capturing data"), QApplication::tr("Active"),QApplication::tr("Closed manually"),QApplication::tr("Closed auto")); 
			return stateCollection;
		}

		tender::viewConn::IScheduledDeliveryMaintainVCPtr ApplCompMgr::getScheduledDeliveryMaintainVC(bool needsCustomerSelection)
		{
			METHODNAME_DEF( ApplCompMgr, getScheduledDeliveryMaintainVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if( 0 == m_ScheduledDeliveryMaintainVC.get() )
			{
				m_ScheduledDeliveryMaintainVC = tender::viewConn::ScheduledDeliveryMaintainVCPtr( new tender::viewConn::ScheduledDeliveryMaintainVC( getMainVC().get() ) );

				basar::I18nString popFunctionality;
				m_ParameterGateway->getParameterValue( getSession()->getUser()->getAreaID(), "POP_FUNCTIONALITY", popFunctionality );

				m_ScheduledDeliveryMaintainVC->setPOPFunctionality( popFunctionality );
                m_ScheduledDeliveryMaintainVC->setRegion(getSession()->getUser()->getAreaID());
			}

			m_ScheduledDeliveryMaintainVC->setNeedsCustomerSelection(needsCustomerSelection);

			return m_ScheduledDeliveryMaintainVC;
		}

		tender::viewConn::CustomerListMaintainVCPtr ApplCompMgr::getCustomerListMaintainVC()
		{
			METHODNAME_DEF( ApplCompMgr, getCustomerListMaintainVC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			if( 0 == m_CustomerListMaintainVC.get() )
			{
				m_CustomerListMaintainVC = tender::viewConn::CustomerListMaintainVCPtr( new tender::viewConn::CustomerListMaintainVC( getMainVC().get() ) );
			}
			
			return m_CustomerListMaintainVC;
		}

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // </VC getting>
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // <UC getting>
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        libutil::useCase::IRunnablePtr ApplCompMgr::getMainUC()
        {
            METHODNAME_DEF( ApplCompMgr, getMainUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            tender::useCase::MainUCPtr mainUC = tender::useCase::MainUCPtr( new tender::useCase::MainUC );

            mainUC->injectMainVC       ( getMainVC()        );
            mainUC->injectQApplication ( getQApplication()  );
            mainUC->injectUseCaseGetter( shared_from_this() );
			mainUC->injectTenderCollectionDM( getTenderCollectionDM() );

            return mainUC;
        }

        libutil::useCase::IRunnablePtr ApplCompMgr::getTenderOverviewUC()
        {
            METHODNAME_DEF( ApplCompMgr, getTenderOverviewUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            tender::useCase::TenderOverviewUCPtr searchUC = tender::useCase::TenderOverviewUCPtr( new tender::useCase::TenderOverviewUC );

            searchUC->injectTenderOverviewVC			 ( getTenderOverviewVC()			    );
            searchUC->injectTenderCollectionDM			 ( getTenderCollectionDM()			    );
			searchUC->injectScheduledDeliveryCollectionDM( getScheduledDeliveryCollectionDM()   );
            searchUC->injectUseCaseGetter				 ( shared_from_this()				    );
			searchUC->injectTenderProgressDM			 ( getTenderProgressDM()			    );
			searchUC->injectPurchaseOrderProposalDM		 ( getPurchaseOrderProposalDM()		    );
			searchUC->injectTransactionFactory			 ( shared_from_this()				    );
			searchUC->setArea							 ( getSession()->getUser()->getAreaID() );
            switch( getSession()->getUser()->getAreaID() )
            {
                case constants::REGION_CH: 
                    searchUC->injectTenderStateUC( getTenderStateUC() );
                    break;
                default:
                    break;
            }
            return searchUC;
        }

        libutil::useCase::IRunnablePtr ApplCompMgr::getTenderMaintainUC(libtender::domMod::tender::ITenderPtr tender, libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr deliveryCollection, bool cloneDeliveries )
        {
            METHODNAME_DEF( ApplCompMgr, getTenderMaintainUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            tender::useCase::TenderMaintainUCPtr maintainUC = tender::useCase::TenderMaintainUCPtr( new tender::useCase::TenderMaintainUC );

            maintainUC->injectTenderMaintainVC( getTenderMaintainVC() );
			maintainUC->injectUseCaseGetter ( shared_from_this() );
			maintainUC->injectPurchaseOrderProposalDM( getPurchaseOrderProposalDM() );
			maintainUC->injectArticlePriceDM( getArticlePriceDM() );
			maintainUC->injectTenderCollectionDM( getTenderCollectionDM() );
			maintainUC->setArea( getSession()->getUser()->getAreaID() );
            maintainUC->setTender( tender );
			cloneDeliveries;

            switch( getSession()->getUser()->getAreaID() )
            {
                case constants::REGION_CH: 
                    maintainUC->injectTenderStateUC( getTenderStateUC() );
                    //maintainUC->injectOMGOrderUC( boost::dynamic_pointer_cast<tender::useCase::OMGOrderUC>( getOMGOrderUC(tender) ) );
                    break;
                default:
                    break;
            }

			libtender::domMod::scheduledDelivery::ISchedulePtr schedule = getLIBtenderApplCompMgr()->getSchedule();
			schedule->injectScheduledDeliveryCollection( deliveryCollection );
			maintainUC->setSchedule( schedule );

            return maintainUC;
        }

        tender::useCase::ITenderStateUCPtr ApplCompMgr::getTenderStateUC()
        {
            METHODNAME_DEF( ApplCompMgr, getTenderStateUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			basar::I18nString omgDuration;
			m_ParameterGateway->getParameterValue(getSession()->getUser()->getAreaID(), "OMG_DURATION", omgDuration);

            tender::useCase::TenderStateUCPtr tenderStateUC = tender::useCase::TenderStateUCPtr(new tender::useCase::TenderStateUC());
            tenderStateUC->injectTenderCollectionDM(getTenderCollectionDM());
			tenderStateUC->setOMGDuration(omgDuration.stoi());

            return tenderStateUC;
        }

        libutil::useCase::IRunnablePtr ApplCompMgr::getOMGOrderUC( libtender::domMod::tender::ITenderPtr tender )
        {
            METHODNAME_DEF( ApplCompMgr, getOMGOrderUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            tender::useCase::OMGOrderUCPtr omgOrderUC = tender::useCase::OMGOrderUCPtr( new tender::useCase::OMGOrderUC(tender) );
            omgOrderUC->injectCscOrderDM( getCscOrderDM() );
			omgOrderUC->injectCscParamProxy(getCscClientParameterProxy());
            return omgOrderUC;
        }

		tender::useCase::IPickerUCPtr ApplCompMgr::getArticlePickerUC( basar::db::aspect::AccessorPropertyTable_YIterator yit /* = basar::db::aspect::AccessorPropertyTable_YIterator() */ )
		{
			METHODNAME_DEF( ApplCompMgr, getArticlePickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			tender::useCase::ArticlePickerUCPtr articlePickerUC = tender::useCase::ArticlePickerUCPtr( new tender::useCase::ArticlePickerUC );
			articlePickerUC->injectArticlePickerVC( getArticlePickerVC() );
			articlePickerUC->injectArticleCollectionDM( getArticleCollectionDM() );
			if( false == yit.isNull() )
			{
				articlePickerUC->setSearchCriteria( yit );
			}

			return articlePickerUC;
		}

        tender::useCase::IPickerUCPtr ApplCompMgr::getPharmacyGroupPickerUC( basar::db::aspect::AccessorPropertyTable_YIterator yit /* = basar::db::aspect::AccessorPropertyTable_YIterator() */ )
        {
            METHODNAME_DEF( ApplCompMgr, getPharmacyGroupPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            tender::useCase::PharmacyGroupPickerUCPtr pharmacyGroupPickerUC = tender::useCase::PharmacyGroupPickerUCPtr( new tender::useCase::PharmacyGroupPickerUC );
            pharmacyGroupPickerUC->injectPharmacyGroupPickerVC( getPharmacyGroupPickerVC() );
            pharmacyGroupPickerUC->injectPharmacyGroupCollectionDM( getPharmacyGroupCollectionDM() );
            if( false == yit.isNull() )
            {
                pharmacyGroupPickerUC->setSearchCriteria( yit );
            }
            
            return pharmacyGroupPickerUC;
        }

        tender::useCase::IPickerUCPtr ApplCompMgr::getCustomerSelectionUC(basar::db::aspect::AccessorPropertyTableRef propTab, basar::Int32 customerNo, basar::Int32 pharmacyChainNo)
		{
			METHODNAME_DEF(ApplCompMgr, getCustomerSelectionUC)
            BLOG_TRACE_METHOD(m_Logger, fun);

			tender::useCase::CustomerSelectionUCPtr customerSelectionUC = tender::useCase::CustomerSelectionUCPtr( new tender::useCase::CustomerSelectionUC );
            customerSelectionUC->injectCustomerSelectionVC( getCustomerSelectionVC() );
            customerSelectionUC->setCustomerList(propTab);
			//customerSelectionUC->setCustomerNo(customerNo);
			//customerSelectionUC->setPharmacyChainNo(pharmacyChainNo);
			customerNo;
			pharmacyChainNo;

			
			return customerSelectionUC;
		}

		tender::useCase::IPickerUCPtr ApplCompMgr::getCustomerPickerUC( basar::db::aspect::AccessorPropertyTable_YIterator yit /* = basar::db::aspect::AccessorPropertyTable_YIterator() */ )
		{
			METHODNAME_DEF( ApplCompMgr, getCustomerPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			tender::useCase::CustomerPickerUCPtr customerPickerUC = tender::useCase::CustomerPickerUCPtr( new tender::useCase::CustomerPickerUC );
			customerPickerUC->injectCustomerPickerVC( getCustomerPickerVC() );
			customerPickerUC->injectCustomerCollectionDM( getCustomerCollectionDM() );
			if( false == yit.isNull() )
			{
				customerPickerUC->setSearchCriteria( yit );
			}

			return customerPickerUC;
		}

		tender::useCase::IPickerUCPtr ApplCompMgr::getPartnerPickerUC( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( ApplCompMgr, getPartnerPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			tender::useCase::PartnerPickerUCPtr partnerPickerUC = tender::useCase::PartnerPickerUCPtr( new tender::useCase::PartnerPickerUC );
			partnerPickerUC->injectPartnerPickerVC( getPartnerPickerVC() );
			partnerPickerUC->injectPartnerCollectionDM( getPartnerCollectionDM() );
			if( false == yit.isNull() )
			{
				partnerPickerUC->setSearchCriteria( yit );
			}

			return partnerPickerUC;
		}

        tender::useCase::IPickerUCPtr ApplCompMgr::getSupplierPickerUC( basar::db::aspect::AccessorPropertyTable_YIterator yit )
        {
            METHODNAME_DEF( ApplCompMgr, getSupplierPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );
            
            tender::useCase::SupplierPickerUCPtr supplierPickerUC = tender::useCase::SupplierPickerUCPtr( new tender::useCase::SupplierPickerUC );
            supplierPickerUC->injectSupplierPickerVC( getSupplierPickerVC() );
            supplierPickerUC->injectSupplierCollectionDM( getSupplierCollectionDM() );
            if( false == yit.isNull() )
            {
            	supplierPickerUC->setSearchCriteria( yit );
            }
            
            return supplierPickerUC;
        }

		tender::useCase::IPickerUCPtr ApplCompMgr::getPharmacyChainPickerUC( basar::db::aspect::AccessorPropertyTable_YIterator yit )
		{
			METHODNAME_DEF( ApplCompMgr, getPharmacyChainPickerUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			tender::useCase::PharmacyChainPickerUCPtr pharmacyChainPickerUC = tender::useCase::PharmacyChainPickerUCPtr( new tender::useCase::PharmacyChainPickerUC );
			pharmacyChainPickerUC->injectPharmacyChainPickerVC( getPharmacyChainPickerVC() );
			pharmacyChainPickerUC->injectPharmacyChainCollectionDM( getPharmacyChainCollectionDM() );
			if( false == yit.isNull() )
			{
				pharmacyChainPickerUC->setSearchCriteria( yit );
			}

			return pharmacyChainPickerUC;
		}
		
		libutil::useCase::IRunnablePtr ApplCompMgr::getScheduledDeliveryMaintainUC(libtender::domMod::tender::ITenderPtr tender, libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery, 
                                                                                   libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr scheduledDeliveryCollection, bool needsCustomerSelection)
		{
			METHODNAME_DEF(ApplCompMgr, getScheduledDeliveryMaintainUC)
            BLOG_TRACE_METHOD(m_Logger, fun);

			tender::useCase::ScheduledDeliveryMaintainUCPtr scheduledDeliveryMaintainUC = tender::useCase::ScheduledDeliveryMaintainUCPtr(new tender::useCase::ScheduledDeliveryMaintainUC());

            scheduledDeliveryMaintainUC->injectUseCaseGetter(shared_from_this());

			scheduledDeliveryMaintainUC->injectScheduledDeliveryMaintainVC(getScheduledDeliveryMaintainVC(needsCustomerSelection));
			scheduledDeliveryMaintainUC->injectCustomerCollectionDM( getCustomerCollectionDM() );
			scheduledDeliveryMaintainUC->injectPurchaseOrderProposalDM(getPurchaseOrderProposalDM());
			scheduledDeliveryMaintainUC->injectTenderProgressDM(getTenderProgressDM());
			scheduledDeliveryMaintainUC->setTender(tender);
			scheduledDeliveryMaintainUC->setScheduledDelivery(scheduledDelivery);
			scheduledDeliveryMaintainUC->setScheduledDeliveryCollection(scheduledDeliveryCollection);

			return scheduledDeliveryMaintainUC;
		}

        libutil::useCase::IRunnablePtr ApplCompMgr::getCSCOrderMaintainUC( libtender::domMod::tender::ITenderPtr tender, libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery)
        {
            METHODNAME_DEF( ApplCompMgr, getCSCOrderMaintainUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            tender::useCase::CSCOrderMaintainUCPtr cscorderMaintainUC = tender::useCase::CSCOrderMaintainUCPtr( new tender::useCase::CSCOrderMaintainUC );

            cscorderMaintainUC->injectCSCOrderMaintainVC( getCSCOrderMaintainVC() );
            cscorderMaintainUC->injectCustomerCollectionDM( getCustomerCollectionDM() );
            cscorderMaintainUC->injectTenderProgressDM( getTenderProgressDM() );
            cscorderMaintainUC->setTender( tender );
            cscorderMaintainUC->setScheduledDelivery( scheduledDelivery );
            // creating a dummy DM and accessor for displaying list of customers for selected pharmacy groupid and to enter the quatity for creation of cscorder.
            libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr cscOrdercollection = getCscOrderCollectionDM();
            cscorderMaintainUC->injectCSCorderUC( getCSCOrderUC(cscOrdercollection) );
            cscorderMaintainUC->injectCscOrderCustomerCollectionDM(cscOrdercollection );

            return cscorderMaintainUC;
        }

		libutil::useCase::IRunnablePtr ApplCompMgr::getTenderImportUC()
			//libtender::domMod::tender::ITenderPtr tender, libtender::domMod::scheduledDelivery::IScheduledDeliveryPtr scheduledDelivery, libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr scheduledDeliveryCollection )
		{
			METHODNAME_DEF( ApplCompMgr, getTenderImportUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			tender::useCase::TenderImportUCPtr tenderImportUC( new tender::useCase::TenderImportUC );
			tenderImportUC->inject( getTenderImportVC() );

			libtender::domMod::tender::ITenderImportCollectionPtr tenderImportCollection( getLIBtenderApplCompMgr()->getTenderImportCollection() );
			tenderImportUC->inject( tenderImportCollection );
			tenderImportUC->inject( getLIBtenderApplCompMgr()->getScheduledDeliveryImportCollection( tenderImportCollection ) );

			tender::useCase::TenderImportFileReaderPtr tenderImportFileReader( new tender::useCase::TenderImportFileReader() );
			tenderImportFileReader->inject( boost::dynamic_pointer_cast<tender::useCase::IProgress>( tenderImportUC ) );
			tenderImportFileReader->inject( boost::dynamic_pointer_cast<tender::useCase::IErrorHandler>( tenderImportUC ) );
			tenderImportUC->inject( tenderImportFileReader );

			tender::useCase::TenderImportCheckerPtr tenderImportChecker( getTenderImportChecker() );
			tenderImportChecker->inject( boost::dynamic_pointer_cast<tender::useCase::IErrorHandler>( tenderImportUC ) );
			tenderImportChecker->inject( tenderImportCollection );
			tenderImportUC->inject( tenderImportChecker );

			tenderImportUC->inject( getLIBtenderApplCompMgr() );
			tenderImportUC->inject( getPurchaseOrderProposalDM() );
			tenderImportUC->setArea( getSession()->getUser()->getAreaID() );

			return tenderImportUC;
		}

		libutil::useCase::IRunnablePtr ApplCompMgr::getCustomerListMaintainUC( libtender::domMod::customerlist::ICustomerListDMPtr customerListDM )
		{
			METHODNAME_DEF( ApplCompMgr, getCustomerListMaintainUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			tender::useCase::CustomerListMaintainUCPtr customerListMaintainUC = tender::useCase::CustomerListMaintainUCPtr( new tender::useCase::CustomerListMaintainUC );

			customerListMaintainUC->injectCustomerListMaintainVC( getCustomerListMaintainVC() );
			customerListMaintainUC->injectCustomerListDM( customerListDM );
			customerListMaintainUC->injectUseCaseGetter( shared_from_this() );

			return customerListMaintainUC;
		}

		libutil::useCase::IRunnablePtr ApplCompMgr::getTenderProtocolUC( libtender::domMod::tender::ITenderPtr tender, basar::Int16 area )
		{
			METHODNAME_DEF( ApplCompMgr, getTenderProtocolUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			tender::useCase::TenderProtocolUCPtr tenderProtocolUC = tender::useCase::TenderProtocolUCPtr( new tender::useCase::TenderProtocolUC );
			tenderProtocolUC->injectTenderProtocolVC( getTenderProtocolVC() );
			tenderProtocolUC->injectTenderProgressDM( getTenderProgressDM() );
			tenderProtocolUC->injectUseCaseGetter( shared_from_this() );
			tenderProtocolUC->setTender( tender );
			tenderProtocolUC->setArea( area );

			return tenderProtocolUC;		
		}

		libutil::useCase::IRunnablePtr ApplCompMgr::getInvoiceUC( libtender::domMod::tender::ITenderProgressPtr tenderProgress )
		{
			METHODNAME_DEF( ApplCompMgr, getInvoiceUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

			tender::useCase::TenderInvoiceUCPtr invoiceUC = tender::useCase::TenderInvoiceUCPtr( new tender::useCase::TenderInvoiceUC );
			invoiceUC->injectTenderInvoiceVC( getTenderInvoiceVC() );
			invoiceUC->injectTenderProtocolDM( getTenderProtocolDM() );
			invoiceUC->injectTransactionFactory( shared_from_this()	);
			invoiceUC->injectCscOrderDM( getCscOrderDM() );

			basar::I18nString orderType;
			m_ParameterGateway->getParameterValue( getSession()->getUser()->getAreaID(), "ORDER_TYPE", orderType );
			invoiceUC->injectOrderType( orderType );
			
			invoiceUC->setTenderProgress( tenderProgress );

			invoiceUC->injectCscParamProxy(getCscClientParameterProxy());
			
			return invoiceUC;
		}

        libutil::useCase::IRunnablePtr ApplCompMgr::getCSCOrderUC( libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr cscOrderCollection )
        {
            METHODNAME_DEF( ApplCompMgr, getCSCOrderUC )
            BLOG_TRACE_METHOD( m_Logger, fun );

            tender::useCase::CSCOrderUCPtr cscOrderUC = tender::useCase::CSCOrderUCPtr( new tender::useCase::CSCOrderUC(cscOrderCollection) );
            cscOrderUC->injectCscOrderDM( getCscOrderDM() );
            cscOrderUC->injectParameterProxy(getCscClientParameterProxy());
            cscOrderUC->injectCSCOrderMaintainVC(getCSCOrderMaintainVC());
            cscOrderUC->setArea( getSession()->getUser()->getAreaID() );
            return cscOrderUC;
        }

		tender::useCase::TenderImportCheckerPtr ApplCompMgr::getTenderImportChecker()
		{
			tender::useCase::TenderImportCheckerPtr tenderImportChecker( new tender::useCase::TenderImportChecker() );
			tenderImportChecker->injectUseCaseGetter( shared_from_this() );
			tenderImportChecker->inject( getLIBtenderApplCompMgr()->getTenderTypeCollection() );
			tenderImportChecker->inject( getLIBtenderApplCompMgr()->getTenderStateCollection() );
			tenderImportChecker->inject( getArticleCollectionDM() );
			tenderImportChecker->inject( getCustomerCollectionDM() );
			tenderImportChecker->inject( getPharmacyChainCollectionDM() );
			tenderImportChecker->inject( getPartnerCollectionDM() );
			tenderImportChecker->setArea( getSession()->getUser()->getAreaID() );

			return tenderImportChecker;
		}

		basar::I18nString ApplCompMgr::getDokuLink()
		{
			basar::I18nString areaID;
			basar::I18nString dokuLink;
			areaID.itos(basar::login::Manager::getInstance().getAreaID());
			if (!m_ParameterGateway->getParameterValue(areaID, "DOKU", dokuLink))
			{
				m_ParameterGateway->getParameterValue("DOKU", dokuLink);
			}

			return dokuLink;
		}

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // </UC getting>
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // <DM getting>
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        libtender::domMod::tender::ITenderCollectionDMPtr ApplCompMgr::getTenderCollectionDM()
        {
            METHODNAME_DEF( ApplCompMgr, getTenderCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return getLIBtenderApplCompMgr()->getTenderCollectionDM();
        }

		libtender::domMod::article::IArticleCollectionDMPtr ApplCompMgr::getArticleCollectionDM()
		{
			METHODNAME_DEF( ApplCompMgr, getArticleCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getLIBtenderApplCompMgr()->getArticleCollectionDM();
		}

		libtender::domMod::article::IArticlePricePtr ApplCompMgr::getArticlePriceDM()
		{
			METHODNAME_DEF( ApplCompMgr, getArticlePriceDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getLIBtenderApplCompMgr()->getArticlePriceDM();
		}

		libtender::domMod::customer::ICustomerCollectionDMPtr ApplCompMgr::getCustomerCollectionDM()
		{
			METHODNAME_DEF( ApplCompMgr, getCustomerCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getLIBtenderApplCompMgr()->getCustomerCollectionDM();
		}

		libtender::domMod::partner::IPartnerCollectionDMPtr ApplCompMgr::getPartnerCollectionDM()
		{
			METHODNAME_DEF( ApplCompMgr, getPartnerCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getLIBtenderApplCompMgr()->getPartnerCollectionDM(); 
		}

        libtender::domMod::pharmacygroup::IPharmacyGroupCollectionDMPtr ApplCompMgr::getPharmacyGroupCollectionDM()
		{
			METHODNAME_DEF( ApplCompMgr, getPharmacyGroupCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getLIBtenderApplCompMgr()->getPharmacyGroupCollectionDM();
		}

        libtender::domMod::supplier::ISupplierCollectionDMPtr ApplCompMgr::getSupplierCollectionDM()
        {
            METHODNAME_DEF( ApplCompMgr, getSupplierCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return getLIBtenderApplCompMgr()->getSupplierCollectionDM(); 
        }

		libtender::domMod::pharmacychain::IPharmacyChainCollectionDMPtr ApplCompMgr::getPharmacyChainCollectionDM()
		{
			METHODNAME_DEF( ApplCompMgr, getPharmacyChainCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getLIBtenderApplCompMgr()->getPharmacyChainCollectionDM(); 
		}

		libtender::domMod::scheduledDelivery::IScheduledDeliveryCollectionDMPtr ApplCompMgr::getScheduledDeliveryCollectionDM()
		{
			METHODNAME_DEF( ApplCompMgr, getScheduledDeliveryCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getLIBtenderApplCompMgr()->getScheduledDeliveryCollectionDM();
		}

		libtender::domMod::purchaseOrderProposal::IPurchaseOrderProposalPtr ApplCompMgr::getPurchaseOrderProposalDM()
		{
			METHODNAME_DEF( ApplCompMgr, getPurchaseOrderProposalDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getLIBtenderApplCompMgr()->getPurchaseOrderProposalDM();
		}

		libtender::domMod::tender::ITenderProgressPtr ApplCompMgr::getTenderProgressDM()
		{
			METHODNAME_DEF( ApplCompMgr, getTenderProgressDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getLIBtenderApplCompMgr()->getTenderProgressDM();
		}

		libtender::domMod::customerlist::ICustomerListDMPtr ApplCompMgr::getCustomerListDM()
		{
			METHODNAME_DEF( ApplCompMgr, getCustomerListDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getLIBtenderApplCompMgr()->getCustomerListDM();
		}

		libtender::domMod::tender::ITenderProtocolPtr ApplCompMgr::getTenderProtocolDM()
		{
			METHODNAME_DEF( ApplCompMgr, getTenderProtocolDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return getLIBtenderApplCompMgr()->getTenderProtocolDM();
		}

		libtender::domMod::cscorder::ICscOrderPtr ApplCompMgr::getCscOrderDM()
		{
			METHODNAME_DEF( ApplCompMgr, getTenderProtocolDM )
            BLOG_TRACE_METHOD( m_Logger, fun );
		
			libtender::domMod::cscorder::CscOrderDMPtr p = libtender::domMod::cscorder::CscOrderDMPtr(new libtender::domMod::cscorder::CscOrderDM(libtender::LoggerPool::getLoggerDomModules()));
			p->injectCscClientConnection(getCscClientConnection());
		
			return p;
		}

        libtender::domMod::cscordercustomer::ICscOrderCustomerCollectionDMPtr ApplCompMgr::getCscOrderCollectionDM()
        {
            METHODNAME_DEF( ApplCompMgr, getCscOrderCollectionDM )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return getLIBtenderApplCompMgr()->getCscOrderCustomerCollectionDM();
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // </DM getting>
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // <OTHERS>
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        basar::db::aspect::ConnectionRef ApplCompMgr::getDBConnection()
        {
            METHODNAME_DEF( ApplCompMgr, getDBConnection() )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return getDBConnectionData()->getCurrentConnection();
        }

		libtender::infrastructure::cscclient::ICscClientConnectionPtr ApplCompMgr::getCscClientConnection()
		{
			METHODNAME_DEF( ApplCompMgr, getCscClientConnection() )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return m_CscClientConnection;
		}

        libtender::infrastructure::cscclient::ICscClientParameterProxyPtr ApplCompMgr::getCscClientParameterProxy()
        {
            METHODNAME_DEF( ApplCompMgr, getCscClientParameterProxy() )
            BLOG_TRACE_METHOD( m_Logger, fun );

            return m_ParameterProxy;
        }

		libutil::infrastructure::db::dbTransaction::IDBTransactionPtr ApplCompMgr::createDBTransaction()
		{
		    METHODNAME_DEF( ApplCompMgr, createDBTransaction )
		    BLOG_TRACE_METHOD( m_Logger, fun );
		
		    return libutil::infrastructure::db::dbTransaction::IDBTransactionPtr( new libutil::infrastructure::db::dbTransaction::TransactionGuard( getDBConnection() ) );
		}

		libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr ApplCompMgr::getDBConnectionData()
		{
            METHODNAME_DEF( ApplCompMgr, getTenderDBConnectionData() )
            BLOG_TRACE_METHOD( m_Logger, fun );

			return libutil::componentManager::ApplCompMgrBase::getDBConnectionData();
		}

    } // end namnespace componentManager
} // end namnespace tender
