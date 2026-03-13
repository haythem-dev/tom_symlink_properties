#ifndef GUARD_CSC_BATCHES_COMPONENTMANAGER_APPLCOMPONENTMANAGER_H
#define GUARD_CSC_BATCHES_COMPONENTMANAGER_APPLCOMPONENTMANAGER_H

#include "iusecasegetter.h"
#include "icsvexportgetter.h"

#include "csvdataexportservicebaseptr.h"
#include "icsvdataexportserviceptr.h"

#include <infrastructure/libtenderapplcompmgrptr.h>
// component manager and libabbauw includes
#include "libabbauw/componentmanager/dmcomponentmanagerptr.h"
#include "libabbauw/icscordercreatorptr.h"
#include "libabbauw/iparameterdmptr.h"
#include "iuserrightsptr.h"

// vc
#include "mainwindowvcptr.h"
#include "applicationconfigvcptr.h"

#include "viewconn/ibatchordermaintenancevcptr.h"
#include "viewconn/ibatchorderpositionvcptr.h"

#include "ibatchordereditionvcptr.h"
#include "itransferordercreatorvcptr.h"
#include "ichangeordersvcptr.h"
#include "iusersettingsvcptr.h"
#include "itransferorderposvcptr.h"
#include "ibvostatusvcptr.h"
#include "icollectiveorderwishconfirmationvcptr.h"
#include "iorderreleaseinformationvcptr.h"

#include "iarticlefindervcptr.h"
#include "icustomerfindervcptr.h"
#include "imanufacturerfindervcptr.h"

#include "itransferorderprintptr.h"

// print
#include "iprintlayoutptr.h"
#include "iprintprocessorptr.h"



// uc
#include "showbvostatusucptr.h"
#include "usecase/itendercreatorptr.h"

// dm for old stuff
#include "libcscbatch/cscorderdmptr.h"

#include <boost/enable_shared_from_this.hpp>
#include <libutil/applcomponent.h>

#include <libbasar_definitions.h>
#include <libbasardbaspect.h>
#include "icscsessionptr.h"


namespace CSC_Batches
{
	namespace componentManager
	{

		class ApplComponentManager : public libutil::componentManager::ApplCompMgrBase,
			public boost::enable_shared_from_this < ApplComponentManager >,
			public IUseCaseGetter,
			public ICSVExportGetter
		{
		public:
			ApplComponentManager();
			~ApplComponentManager();

			void                                                                    setCSCSession(infrastructure::session::ICSCSessionPtr);
			void                                                                    injectParameterGateway(libutil::infrastructure::parameter::IParameterGatewayPtr);

			// from IUseCaseGetter interface
			libutil::useCase::IRunnablePtr                                          getMainUC();

			libutil::useCase::IRunnablePtr				                            getBatchOrderMaintenanceUC();
			libutil::useCase::IRunnablePtr                                          getBatchOrderPosUC(basar::db::aspect::AccessorPropertyTable_YIterator);

			libutil::useCase::IRunnablePtr				                            getBVOStatusUC();
			libutil::useCase::IRunnablePtr			                                getBVOOrderStatusUC(basar::Int32 orderNo, bool isOrderViewActive);
			libutil::useCase::IRunnablePtr			                                getCollectiveOrderWishUC(const YIterators&, bool isOrderViewActive);
			libutil::useCase::IRunnablePtr			                                getFilteredCollectiveOrderWishUC(const YIterators&, bool isOrderViewActive);

			libutil::useCase::IRunnablePtr                                          getFindCustomerUC();
			libutil::useCase::IRunnablePtr                                          getFindManufacturerUC();
			libutil::useCase::IRunnablePtr                                          getFindArticleUC();

			useCase::ITenderCreatorPtr                                              getTenderCreatorUC();
			libutil::useCase::IRunnablePtr                                          getBatchOrderEditionUC();
			libutil::useCase::IRunnablePtr                                          getTransferOrderPosUC(basar::db::aspect::AccessorPropertyTable_YIterator, bool isOrderViewActive);
			libutil::useCase::IRunnablePtr                                          getUserSettingsUC();

			libutil::useCase::IRunnablePtr                                          getTransferOrderChangerUC(const YIterators&);
			libutil::useCase::IRunnablePtr                                          getTransferOrderCreatorUC();

			// from ICSVExportGetter interface
			CSC_Batches::services::ICSVDataExportServicePtr							getBVOStatusExportService(QWidget* widget);

		private:
			// don't allow assignment and copying of this class
			ApplComponentManager(const ApplComponentManager&);
			ApplComponentManager& operator = (const ApplComponentManager&);

			void                                                                    doClear();

			libtender::componentManager::ApplCompMgrPtr                             getLIBtenderApplCompMgr();

			// other component managers
			libabbauw::componentManager::DMComponentManagerPtr                      getDMComponentManager();

			libutil::infrastructure::parameter::IParameterGatewayPtr                getParameterGateway() const;
			infrastructure::rights::IUserRightsPtr                                  getUserRights();

			// print
			::print::IPrintProcessorPtr                                             getPrintProcessor();
			CSC_Batches::print::ITransferOrderPrintPtr                              getTransferOrderPrintPtr();

			// private uc getters
			useCase::ShowBVOStatusUCPtr                                             getConcreteBVOStatusUC();

			libcscbatch::domMod::CSCOrderDMPtr                                      getCSCOrderDM();

			// main
			viewConn::MainWindowVCPtr                                               getMainVC();
			basar::gui::qt::ApplicationConfigVCPtr                                  getApplicationConfigVC();

			void                                                                    configAboutInformation();
			void                                                                    configApplicationAndDocuInformation();
			void                                                                    configLibraryInformation();

			// vcs
			viewConn::IBatchOrderMaintenanceVCPtr                                   getBatchOrderMaintenanceVC();
			viewConn::IBatchOrderPositionVCPtr                                      getBatchOrderPositionVC();
			viewConn::IBatchOrderEditionVCPtr                                       getBatchOrderEditionVC();
			viewConn::ITransferOrderCreatorVCPtr                                    getTransferOrderCreatorVC();
			viewConn::IChangeOrdersVCPtr                                            getChangeOrdersVC();
			viewConn::ITransferOrderPosVCPtr                                        getTransferOrderPosVC();
			viewConn::IUserSettingsVCPtr                                            getUserSettingsVC();
			viewConn::IBVOStatusVCPtr                                               getBVOStatusVC();
			viewConn::ICollectiveOrderWishConfirmationVCPtr                         getCollectiveOrderWishConfirmationVC(bool isFiltered);
			viewConn::ICustomerFinderVCPtr                                          getCustomerFinderVC();
			viewConn::IManufacturerFinderVCPtr                                      getManufacturerFinderVC();
			viewConn::IArticleFinderVCPtr                                           getArticleFinderVC();
			viewConn::IOrderReleaseInformationVCPtr                                 getOrderReleaseInformationVC();

		private:
			libtender::componentManager::ApplCompMgrPtr                             m_LIBtenderApplCompMgr;
			libabbauw::componentManager::DMComponentManagerPtr				        m_DMComponentManagerPtr;

			viewConn::MainWindowVCPtr                                               m_MainVC;
			basar::gui::qt::ApplicationConfigVCPtr                                  m_ApplicationConfigVC;
			infrastructure::session::ICSCSessionPtr                                 m_CSCSession;
			libutil::infrastructure::parameter::IParameterGatewayPtr                m_ParameterGateway;
		};
	} // end namespace componentManager
} // end namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_COMPONENTMANAGER_APPLCOMPONENTMANAGER_H
