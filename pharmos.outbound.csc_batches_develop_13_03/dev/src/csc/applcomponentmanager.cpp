#include "applcomponentmanager.h"

// vcs
#include "mainwindowvc.h"

#include <infrastructure/libtenderapplcompmgr.h>
#include "viewconn/batchordermaintenancevc.h"
#include "viewconn/batchordermaintenancevcptr.h"
#include "viewconn/batchorderpositionvc.h"
#include "viewconn/batchorderpositionvcptr.h"

#include "batchordereditionvc.h"
#include "batchordereditionvcptr.h"

#include "transferordercreatorvc.h"
#include "transferordercreatorvcptr.h"

#include "transferorderposvc.h"
#include "transferorderposvcptr.h"

#include "changeordersvc.h"
#include "changeordersvcptr.h"

#include "usersettingsvc.h"
#include "usersettingsvcptr.h"

#include "bvostatusvc.h"
#include "bvostatusvcptr.h"

#include "collectiveorderwishconfirmationvc.h"
#include "collectiveorderwishconfirmationvcptr.h"

#include "articlefindervc.h"
#include "articlefindervcptr.h"

#include "customerfindervc.h"
#include "customerfindervcptr.h"

#include "manufacturerfindervc.h"
#include "manufacturerfindervcptr.h"

#include "orderreleaseinformationvc.h"
#include "orderreleaseinformationvcptr.h"

// ucs
#include "mainuc.h"

#include "usecase/batchordermaintenanceuc.h"
#include "usecase/batchordermaintenanceucptr.h"

#include "usecase/batchorderposuc.h"
#include "usecase/batchorderposucptr.h"

#include "batchordereditionuc.h"
#include "batchordereditionucptr.h"

#include "usecase/tendercreatoruc.h"

#include "transferorderchangeruc.h"
#include "transferorderchangerucptr.h"

#include "transferordercreatoruc.h"
#include "transferordercreatorucptr.h"

#include "usersettingsuc.h"
#include "usersettingsucptr.h"

#include "transferorderposuc.h"
#include "transferorderposucptr.h"

#include "showbvostatusuc.h"
#include "collectiveorderwishuc.h"
#include "collectiveorderwishucptr.h"

#include "findarticleuc.h"
#include "findarticleucptr.h"
#include "findcustomeruc.h"
#include "findcustomerucptr.h"
#include "findmanufactureruc.h"
#include "findmanufacturerucptr.h"
#include "usecase/tendercreatorucptr.h"


// print
#include "printprocessor.h"
#include "transferorderprint.h"
#include "transferorderprintptr.h"

// cms
#include "libabbauw/componentmanager/dmcomponentmanager.h"
#include "libabbauw/componentmanager/itransferorderheadcollectiondmgetter.h"

// dms
#include "libabbauw/itransferorderheadcollectiondm.h"
#include "libabbauw/domainmodule/orderproposal/iorderproposalcollection.h"

// old stuff - needs to be refactored and included with interfaces (libcscbatch
#include "libcscbatch/batchorderdm.h"
#include "libcscbatch/cscorderdm.h"
#include "libcscbatch/csc_batches_version.h"

// parameter
#include "libabbauw/iparameterdm.h"
#include "userrights.h"
#include "userrightsptr.h"

// factories
#include "libabbauw/componentmanager/ibatchorderconnectedfactory.h"

#include "libabbauw/componentmanager/itransferorderfactory.h"
#include "libabbauw/componentmanager/itransferorderfactoryptr.h"
#include "libabbauw/componentmanager/itransferorderheadcollectionfactory.h"

#include "libabbauw/componentmanager/iorderproposalcollectionfactory.h"

#include <libutil/util.h>
#include <libutil/dbconnection.h>

// basar
#include <libbasardbsql_databaseinfo.h>
#include <libbasarappl.h>
#include <libbasarlogin.h>
#include <libbasardbsql.h>
#include <libbasarproperty.h>
#include <libbasarqtwidget.h>
#include <libbasarguitie.h>
#include "definitions_uadm.h"
#include "definitions_gui.h"
#include <boost/make_shared.hpp>
#include <icscsession.h>

namespace CSC_Batches
{
	namespace componentManager
	{

		ApplComponentManager::ApplComponentManager()
		{
		}

		ApplComponentManager::~ApplComponentManager()
		{
		}

		void ApplComponentManager::setCSCSession(infrastructure::session::ICSCSessionPtr session)
		{
			m_CSCSession = session;
		}

		void ApplComponentManager::injectParameterGateway(libutil::infrastructure::parameter::IParameterGatewayPtr parameterGateway)
		{
			m_ParameterGateway = parameterGateway;
		}

		libutil::infrastructure::parameter::IParameterGatewayPtr ApplComponentManager::getParameterGateway() const
		{
			return m_ParameterGateway;
		}

		infrastructure::rights::IUserRightsPtr ApplComponentManager::getUserRights()
		{
			infrastructure::rights::UserRightsPtr userRights = boost::make_shared< infrastructure::rights::UserRights >();
			userRights->injectParameterGateway(getParameterGateway());
			userRights->injectSession(getSession());

			return userRights;
		}

		void ApplComponentManager::doClear()
		{
		}

		// ####################################################################################################################
		// # CMS
		// ####################################################################################################################
		libabbauw::componentManager::DMComponentManagerPtr ApplComponentManager::getDMComponentManager()
		{
			if (0 == m_DMComponentManagerPtr.get())
			{
				m_DMComponentManagerPtr = boost::make_shared< libabbauw::componentManager::DMComponentManager >();
				m_DMComponentManagerPtr->injectDBConnection(getDBConnectionData());
				m_DMComponentManagerPtr->setSession(getSession());
				m_DMComponentManagerPtr->setProgramName("CSCBatches");
				m_DMComponentManagerPtr->injectCSCOrderCreator(getCSCOrderDM());
			}
			return m_DMComponentManagerPtr;
		}

		// print processor
		::print::IPrintProcessorPtr ApplComponentManager::getPrintProcessor()
		{
			return boost::make_shared< ::print::PrintProcessor >();
		}

		// ####################################################################################################################
		// # VCS
		// ####################################################################################################################
		viewConn::MainWindowVCPtr ApplComponentManager::getMainVC()
		{
			if (0 == m_MainVC.get())
			{
				m_MainVC = boost::make_shared< viewConn::MainWindowVC >(getSession(), getDBConnectionData()->getDBInfo().dbServer,
					getDBConnectionData()->getDBInfo().database, getUserRights());
			}

			return m_MainVC;
		}

		basar::gui::qt::ApplicationConfigVCPtr ApplComponentManager::getApplicationConfigVC()
		{
			if (0 == m_ApplicationConfigVC.get())
			{
				m_ApplicationConfigVC = boost::make_shared< basar::gui::qt::About >();
				configAboutInformation();
			}

			return m_ApplicationConfigVC;
		}

		void ApplComponentManager::configAboutInformation()
		{
			configApplicationAndDocuInformation();
			configLibraryInformation();
		}

		void ApplComponentManager::configApplicationAndDocuInformation()
		{
			basar::InitStruct aboutInformation;

			basar::I18nString value;
			m_ParameterGateway->getParameterValue("ABBAUW", "USER_MANUAL_REFERENCE", value);

			aboutInformation.database = getDBConnectionData()->getDBInfo().database;
			aboutInformation.host = getDBConnectionData()->getDBInfo().dbServer;
			aboutInformation.dokulink = value;

			basar::gui::qt::Manager::getInstance().init(aboutInformation);
		}

		void ApplComponentManager::configLibraryInformation()
		{
			basar::DefinitionStruct def;
			def.copyright = "copyright by PHOENIX GmbH & Co. KG";
			def.programDescription = CSC_Batches::APPLICATION;
			def.version = CSC_BATCHES_VERSION;
			basar::gui::qt::Manager::getInstance().init(def);

			basar::DllStruct dll;

			dll.name = "libbasarcmnutil.dll";
			dll.pkgVersion = basar::cmnutil::getVersion();
			basar::gui::qt::Manager::getInstance().initDll(dll);

			dll.name = "libbasardbsql.dll";
			dll.pkgVersion = basar::db::sql::getVersion();
			basar::gui::qt::Manager::getInstance().initDll(dll);

			dll.name = "libbasarproperty.dll";
			dll.pkgVersion = basar::property::getVersion();
			basar::gui::qt::Manager::getInstance().initDll(dll);

			dll.name = "libbasardbaspect.dll";
			dll.pkgVersion = basar::db::aspect::getVersion();
			basar::gui::qt::Manager::getInstance().initDll(dll);

			dll.name = "libbasarappl.dll";
			dll.pkgVersion = basar::appl::getVersion();
			basar::gui::qt::Manager::getInstance().initDll(dll);

			dll.name = "libbasarlogin.dll";
			dll.pkgVersion = basar::login::getVersion();
			basar::gui::qt::Manager::getInstance().initDll(dll);

			dll.name = "libbasarguitie.dll";
			dll.pkgVersion = basar::gui::tie::getVersion();
			basar::gui::qt::Manager::getInstance().initDll(dll);

			dll.name = "libbasarqtwidget.dll";
			dll.pkgVersion = basar::gui::widget::getVersion();
			basar::gui::qt::Manager::getInstance().initDll(dll);

			dll.name = "libbasarguiqt.dll";
			dll.pkgVersion = basar::gui::qt::getVersion();
			basar::gui::qt::Manager::getInstance().initDll(dll);
		}

		viewConn::IBatchOrderMaintenanceVCPtr ApplComponentManager::getBatchOrderMaintenanceVC()
		{
			viewConn::BatchOrderMaintenanceVCPtr vc = boost::make_shared< viewConn::BatchOrderMaintenanceVC >();
			vc->setSession(getSession());

			return vc;
		}

		viewConn::IBatchOrderPositionVCPtr ApplComponentManager::getBatchOrderPositionVC()
		{
			viewConn::BatchOrderPositionVCPtr vc = boost::make_shared< viewConn::BatchOrderPositionVC >();
			vc->setSession(getSession());

			return vc;
		}

		viewConn::IBatchOrderEditionVCPtr ApplComponentManager::getBatchOrderEditionVC()
		{
			viewConn::BatchOrderEditionVCPtr vc = boost::make_shared< viewConn::BatchOrderEditionVC>();
			libabbauw::domMod::parameter::IParameterDMPtr param = getDMComponentManager()->getParameterDM();
			vc->injectFilterParameter(param);
			vc->injectCalcParameter(param);
			vc->injectUserRights(getUserRights());

			return vc;
		}

		viewConn::ITransferOrderCreatorVCPtr ApplComponentManager::getTransferOrderCreatorVC()
		{
			viewConn::TransferOrderCreatorVCPtr vc = boost::make_shared< viewConn::TransferOrderCreatorVC >();
			vc->injectParameter(getDMComponentManager()->getParameterDM());

			return vc;
		}

		viewConn::IChangeOrdersVCPtr ApplComponentManager::getChangeOrdersVC()
		{
			viewConn::ChangeOrdersVCPtr vc = boost::make_shared< viewConn::ChangeOrdersVC >();
			vc->injectParameter(getDMComponentManager()->getParameterDM());

			return vc;
		}

		viewConn::IBVOStatusVCPtr ApplComponentManager::getBVOStatusVC()
		{
			viewConn::BVOStatusVCPtr vc = boost::make_shared< viewConn::BVOStatusVC >();
			return vc;
		}

		viewConn::ICollectiveOrderWishConfirmationVCPtr ApplComponentManager::getCollectiveOrderWishConfirmationVC(bool isFiltered)
		{
			QWidget* widgetPtr = 0; // we miss nullptr to let compiler correctly deduct type (sadly no c++11 yet)
			viewConn::CollectiveOrderWishConfirmationVCPtr vc = boost::make_shared< viewConn::CollectiveOrderWishConfirmationVC >(widgetPtr, isFiltered);
			vc->setSession(getSession());

			return vc;
		}

		viewConn::ITransferOrderPosVCPtr ApplComponentManager::getTransferOrderPosVC()
		{
			viewConn::TransferOrderPosVCPtr vc = boost::make_shared< viewConn::TransferOrderPosVC >();
			vc->injectUserRights(getUserRights());
			return vc;
		}

		viewConn::IUserSettingsVCPtr ApplComponentManager::getUserSettingsVC()
		{
			viewConn::UserSettingsVCPtr vc = boost::make_shared< viewConn::UserSettingsVC >();
			vc->injectParameter(getDMComponentManager()->getParameterDM());
			vc->injectUserRights(getUserRights());

			return vc;
		}

		viewConn::ICustomerFinderVCPtr ApplComponentManager::getCustomerFinderVC()
		{
			viewConn::CustomerFinderVCPtr vc = boost::make_shared< viewConn::CustomerFinderVC >();
			return vc;
		}

		viewConn::IManufacturerFinderVCPtr ApplComponentManager::getManufacturerFinderVC()
		{
			viewConn::ManufacturerFinderVCPtr vc = boost::make_shared< viewConn::ManufacturerFinderVC >();
			return vc;
		}

		viewConn::IArticleFinderVCPtr ApplComponentManager::getArticleFinderVC()
		{
			viewConn::ArticleFinderVCPtr vc = boost::make_shared< viewConn::ArticleFinderVC >();
			return vc;
		}

		viewConn::IOrderReleaseInformationVCPtr ApplComponentManager::getOrderReleaseInformationVC()
		{
			viewConn::OrderReleaseInformationVCPtr vc = boost::make_shared< viewConn::OrderReleaseInformationVC >();
			return vc;
		}

		libcscbatch::domMod::CSCOrderDMPtr ApplComponentManager::getCSCOrderDM()
		{
			libcscbatch::domMod::CSCOrderDMPtr dm = boost::make_shared< libcscbatch::domMod::CSCOrderDM >(getDBConnectionData()->getCurrentConnection(), getSession()->getUser()->getAreaID(), m_CSCSession->getServer(), m_CSCSession->getPort());
			dm->injectParameter(getDMComponentManager()->getParameterDM());
			return dm;
		}

		CSC_Batches::print::ITransferOrderPrintPtr ApplComponentManager::getTransferOrderPrintPtr()
		{
			CSC_Batches::print::TransferOrderPrintPtr transferOrderPrintPtr = boost::make_shared< CSC_Batches::print::TransferOrderPrint >();
			return transferOrderPrintPtr;
		}

		// ####################################################################################################################
		// # UCS
		// ####################################################################################################################
		libutil::useCase::IRunnablePtr ApplComponentManager::getMainUC()
		{
			boost::shared_ptr< useCase::MainUC > uc = boost::make_shared< useCase::MainUC >();
			uc->injectApplicationConfigVC(getApplicationConfigVC());
			uc->injectMainVC(getMainVC());
			uc->injectQApplication(getQApplication());
			uc->injectUseCaseGetter(shared_from_this());
			uc->injectParameter(getDMComponentManager()->getParameterDM());
			uc->injectUserRights(getUserRights());

			return uc;
		}

		libutil::useCase::IRunnablePtr ApplComponentManager::getBatchOrderMaintenanceUC()
		{
			useCase::BatchOrderMaintenanceUCPtr uc = boost::make_shared< useCase::BatchOrderMaintenanceUC >();
			uc->injectBatchOrderCollectionFinder(getDMComponentManager()->getBatchOrderCollectionFinder());
			uc->injectBatchOrderMaintenanceVC(getBatchOrderMaintenanceVC());
			uc->injectOrderReleaseInformationVC(getOrderReleaseInformationVC());
			uc->injectUseCaseGetter(shared_from_this());
			uc->setSession(getSession());

			return uc;
		}

		libutil::useCase::IRunnablePtr ApplComponentManager::getBatchOrderPosUC(basar::db::aspect::AccessorPropertyTable_YIterator yit)
		{
			useCase::BatchOrderPosUCPtr uc = boost::make_shared< useCase::BatchOrderPosUC >();
			uc->setSession(getSession());
			uc->injectBatchOrder(getDMComponentManager()->getBatchOrder(yit));
			uc->injectCustomer(getDMComponentManager()->getCustomerFinder());
			uc->injectBatchOrderPosVC(getBatchOrderPositionVC());

			return uc;
		}

		useCase::ShowBVOStatusUCPtr ApplComponentManager::getConcreteBVOStatusUC()
		{
			useCase::ShowBVOStatusUCPtr uc = boost::make_shared< useCase::ShowBVOStatusUC >();
			uc->setSession(getSession());
			uc->injectVC(getBVOStatusVC());
			uc->injectDM(getDMComponentManager()->getOrderProposalCollectionFinder());
			uc->injectDM(getDMComponentManager()->getOrderProposalUpdater());
			uc->injectDM(getDMComponentManager()->getOrderProtocolDM());

			return uc;
		}

		libutil::useCase::IRunnablePtr ApplComponentManager::getBVOStatusUC()
		{
			useCase::ShowBVOStatusUCPtr uc = getConcreteBVOStatusUC();
			uc->init();
			uc->injectCSVExportGetter(shared_from_this());

			return uc;
		}

		libutil::useCase::IRunnablePtr ApplComponentManager::getBVOOrderStatusUC(basar::Int32 orderNo, bool isOrderViewActive)
		{
			useCase::ShowBVOStatusUCPtr uc = getConcreteBVOStatusUC();
			uc->init(orderNo, isOrderViewActive);
			uc->injectDM(getDMComponentManager()->getOrderProtocolDM());
			uc->injectCSVExportGetter(shared_from_this());

			return uc;
		}

		libutil::useCase::IRunnablePtr ApplComponentManager::getCollectiveOrderWishUC(const YIterators& orderYitCollection, bool isOrderViewActive)
		{
			useCase::CollectiveOrderWishUCPtr uc = boost::make_shared< useCase::CollectiveOrderWishUC >();
			uc->injectVC(getCollectiveOrderWishConfirmationVC(false));
			uc->injectDM(getDMComponentManager()->getOrderProposalCollectionFactory()->create(orderYitCollection));
			uc->injectDM(getDMComponentManager()->getOrderProtocolDM());
			uc->setIsOverViewActive(isOrderViewActive);

			return uc;
		}

		libutil::useCase::IRunnablePtr ApplComponentManager::getFilteredCollectiveOrderWishUC(const YIterators& orderYitCollection, bool isOrderViewActive)
		{
			useCase::CollectiveOrderWishUCPtr uc = boost::make_shared< useCase::CollectiveOrderWishUC >();
			uc->injectVC(getCollectiveOrderWishConfirmationVC(true));
			uc->injectDM(getDMComponentManager()->getOrderProposalCollectionFactory()->create(orderYitCollection));
			uc->injectDM(getDMComponentManager()->getOrderProtocolDM());
			uc->setIsOverViewActive(isOrderViewActive);

			return uc;
		}

		libutil::useCase::IRunnablePtr ApplComponentManager::getFindCustomerUC()
		{
			useCase::FindCustomerUCPtr uc = boost::make_shared< useCase::FindCustomerUC >();
			uc->injectVC(getCustomerFinderVC());
			uc->injectDM(getDMComponentManager()->getCustomerCollectionFinder());

			return uc;
		}

		libutil::useCase::IRunnablePtr ApplComponentManager::getFindManufacturerUC()
		{
			useCase::FindManufacturerUCPtr uc = boost::make_shared< useCase::FindManufacturerUC >();
			uc->injectVC(getManufacturerFinderVC());
			uc->injectDM(getDMComponentManager()->getFindManufacturerDM());

			return uc;
		}

		libutil::useCase::IRunnablePtr ApplComponentManager::getFindArticleUC()
		{
			useCase::FindArticleUCPtr uc = boost::make_shared< useCase::FindArticleUC >();
			uc->injectVC(getArticleFinderVC());
			uc->injectDM(getDMComponentManager()->getFindArticleCollectionDM());

			return uc;
		}

		libtender::componentManager::ApplCompMgrPtr ApplComponentManager::getLIBtenderApplCompMgr()
		{
			if (0 == m_LIBtenderApplCompMgr.get())
			{
				m_LIBtenderApplCompMgr = libtender::componentManager::ApplCompMgrPtr(new libtender::componentManager::ApplCompMgr());
				m_LIBtenderApplCompMgr->injectDBConnection(getDBConnectionData()->getCurrentConnection());
				m_LIBtenderApplCompMgr->injectSession(getSession());
			}

			return m_LIBtenderApplCompMgr;
		}

		useCase::ITenderCreatorPtr ApplComponentManager::getTenderCreatorUC()
		{
			useCase::TenderCreatorUCPtr uc(new useCase::TenderCreatorUC());
			uc->injectTenderCollectionDM(getLIBtenderApplCompMgr()->getTenderCollectionDM());
			uc->injectPharmacyGroupCollectionDM(getLIBtenderApplCompMgr()->getPharmacyGroupCollectionDM());
			uc->injectOrderProtocol(getDMComponentManager()->getOrderProtocolDM());
			uc->injectTransferOrderHead(getDMComponentManager()->getTransferOrderHead());

			basar::I18nString strTenderEndDateOffset;

			if (!getParameterGateway()->getParameterValue(getSession()->getUser()->getAreaID(), UADM_TENDER_CREATION_END_DATE_OFFSET, strTenderEndDateOffset))
			{
				getParameterGateway()->getParameterValue(UADM_TENDER_CREATION_END_DATE_OFFSET, strTenderEndDateOffset);
			}

			if (strTenderEndDateOffset.empty())
			{
				strTenderEndDateOffset = "1";
			}

			uc->setTenderEndDateOffset(atoi(strTenderEndDateOffset.c_str()));

			basar::I18nString omgDuration = "";
			getParameterGateway()->getParameterValue("ABBAUW", UADM_OMG_DURATION, omgDuration);
			uc->setOMGDuration(atoi(omgDuration.c_str()));

			return uc;
		}

		libutil::useCase::IRunnablePtr ApplComponentManager::getBatchOrderEditionUC()
		{
			useCase::BatchOrderEditionUCPtr uc = boost::make_shared< useCase::BatchOrderEditionUC >();

			uc->injectDM(getDMComponentManager()->getTransferOrderHeadCollectionDMGetter()->getTransferOrderHeadCollectionDM());
			uc->injectDM(getDMComponentManager()->getOrderProtocolDM());
			uc->injectDM(getDMComponentManager()->getParameterDM());

			uc->injectVC(getBatchOrderEditionVC());
			uc->injectVC(getOrderReleaseInformationVC());
			uc->injectTenderCreatorUC(getTenderCreatorUC());
			uc->injectUseCaseGetter(shared_from_this());

			return uc;
		}

		libutil::useCase::IRunnablePtr ApplComponentManager::getTransferOrderChangerUC(const YIterators& orderYitCollection)
		{
			useCase::TransferOrderChangerUCPtr uc = boost::make_shared< useCase::TransferOrderChangerUC >();
			uc->injectUseCaseGetter(shared_from_this());

			uc->injectDM(getDMComponentManager()->getCustomerFinder());
			uc->injectDM(getDMComponentManager()->getFindManufacturerDM());
			uc->injectDM(getDMComponentManager()->getLockedTransferOrderHeadCollectionFactory()->create(orderYitCollection));
			uc->injectDM(getDMComponentManager()->getOrderProtocolDM());

			uc->injectVC(getChangeOrdersVC());

			return uc;
		}

		libutil::useCase::IRunnablePtr ApplComponentManager::getTransferOrderCreatorUC()
		{
			useCase::TransferOrderCreatorUCPtr uc = boost::make_shared< useCase::TransferOrderCreatorUC >();

			uc->injectDM(getDMComponentManager()->getCustomerFinder());
			uc->injectDM(getDMComponentManager()->getFindManufacturerDM());
			uc->injectDM(getDMComponentManager()->getFindArticleCollectionDM());
			uc->injectDM(getDMComponentManager()->getTransferOrderInPreparationDM());
			uc->injectDM(getDMComponentManager()->getTransferOrderCreationDM());
			uc->injectDM(getDMComponentManager()->getFindPurchaseSupplier());
			uc->injectDM(getDMComponentManager()->getOrderProtocolDM());

			uc->injectParameter(getDMComponentManager()->getParameterDM());

			uc->injectVC(getTransferOrderCreatorVC());

			uc->injectUseCaseGetter(shared_from_this());

			return uc;
		}

		libutil::useCase::IRunnablePtr ApplComponentManager::getTransferOrderPosUC(basar::db::aspect::AccessorPropertyTable_YIterator yit, bool isOrderViewActive)
		{
			useCase::TransferOrderPosUCPtr uc = boost::make_shared< useCase::TransferOrderPosUC >();

			uc->injectDM(getDMComponentManager()->getTransferOrderProtectionProxyFactory()->create(yit));
			uc->injectDM(getDMComponentManager()->getOrderProposalCreator());
			uc->injectDM(getDMComponentManager()->getOrderProposalUpdater());
			uc->injectDM(getDMComponentManager()->getCustomerFinder());
			uc->injectDM(getDMComponentManager()->getOrderProtocolDM());
			uc->injectDM(getDMComponentManager()->getParameterDM());

			uc->injectColorSplitDM(getDMComponentManager()->getTransferOrderColorSplitDM());
			uc->injectNarcoticsRefrigeratedSplitDM(getDMComponentManager()->getTransferOrderNarcoticsRefrigeratedSplitDM());

			uc->injectPrint(getTransferOrderPrintPtr());
			uc->injectPrintProcessor(getPrintProcessor());

			uc->injectVC(getTransferOrderPosVC());

			uc->injectUseCaseGetter(shared_from_this());

			uc->setIsOrderViewActive(isOrderViewActive);

			return uc;
		}

		libutil::useCase::IRunnablePtr ApplComponentManager::getUserSettingsUC()
		{
			useCase::UserSettingsUCPtr uc = boost::make_shared< useCase::UserSettingsUC >();
			uc->injectVC(getUserSettingsVC());
			uc->injectParameter(getDMComponentManager()->getParameterDM());

			return uc;
		}

		CSC_Batches::services::ICSVDataExportServicePtr	ApplComponentManager::getBVOStatusExportService(QWidget* widget)
		{
			CSC_Batches::services::CSVDataExportServiceBasePtr service = boost::make_shared< CSC_Batches::services::BVOStatusDataExportSerivce >();

			const char separator = *getDMComponentManager()->getParameterDM()->getCSVExportSeparator().c_str();

			service->setBranch(getSession()->getUser()->getAreaID());
			service->setBaseExportLocation("C:");
			service->setParentWidget(widget);
			service->setSeparator(separator);

			return service;
		}
	} // end namespace componentManager
} // end namespace CSC_Batches
