#include "dmcomponentmanager.h"

#include "domainmodule/batchorder/batchordercollectiondm.h"
#include "domainmodule/batchorder/batchordercollectiondmptr.h"

#include "transferorderheadcollectiondm.h"
#include "transferorderdmptr.h"
#include "transferorderhead.h"
#include "transferorderheadptr.h"
#include "transferorderposcollection.h"
#include "transferorderposcollectionptr.h"

#include "transferordercreationdm.h"
#include "transferordercreationdmptr.h"

#include "transferordersplitdm.h"
#include "transferordersplitdmptr.h"
#include "colorsplittingpolicy.h"
#include "narcoticsrefrigeratedsplittingpolicy.h"

#include "stockcollectiondm.h"

#include "cscorderdm.h"
#include "iordergetter.h"

#include "domainmodule/orderproposal/orderproposaldm.h"
#include "domainmodule/orderproposal/orderproposaldmptr.h"
#include "domainmodule/orderproposal/orderproposalupdatedm.h"
#include "domainmodule/orderproposal/orderproposalupdatedmptr.h"
#include "domainmodule/orderproposal/orderproposalcreatordm.h"
#include "domainmodule/orderproposal/orderproposalcreatordmptr.h"
#include "domainmodule/orderproposal/orderproposalcollectiondm.h"
#include "domainmodule/orderproposal/orderproposalcollectiondmptr.h"
#include "domainmodule/orderproposal/orderproposalcollectionprotectionproxy.h"
#include "domainmodule/orderproposal/orderproposalcollectionprotectionproxyptr.h"

#include "iorderprotocolgetter.h"
#include "domainmodule/orderprotocol/orderprotocoldm.h"
#include "domainmodule/orderprotocol/orderprotocoldmptr.h"
#include "domainmodule/orderprotocol/orderheadprotocoldm.h"
#include "domainmodule/orderprotocol/orderposcollectionprotocoldm.h"


#include "domainmodule/orderprotocol/orderheadprotocoldmptr.h"
#include "domainmodule/orderprotocol/orderposcollectionprotocoldmptr.h"


#include "purchasebatchorderposdm.h"
#include "purchasebatchorderposdmptr.h"

#include "customerdm.h"
#include "customercollectiondm.h"

#include "manufacturerdm.h"

#include "articlecollectiondm.h"
#include "articleconsultantdm.h"

#include "purchaseordersupplierdmptr.h"
#include "purchaseordersupplierdm.h"
#include "parameterdm.h"

#include "transferorderinpreparationdm.h"
#include "transferorderinpreparationdmptr.h"
#include "transferorderheadinpreparation.h"
#include "transferorderheadinpreparationptr.h"
#include "transferorderposinpreparationcollection.h"
#include "transferorderposinpreparationcollectionptr.h"

#include "transferorderdmgetter.h"
#include "transferorderlockeddmgetter.h"
#include "transferorderfactory.h"
#include "transferorderfactoryptr.h"

#include "transferorderheadfactory.h"
#include "transferorderheadfactoryptr.h"

#include "transferorderheadcollectiondmgetter.h"
#include "transferorderheadcollectionlockeddmgetter.h"
#include "transferorderheadcollectionfactory.h"
#include "transferorderheadcollectionfactoryptr.h"

#include "itransferorderheadcollectionfinderptr.h"


// ACCESSORS
#include "infrastructure/accessor/batchorder/batchordercollectionacc.h"
#include "infrastructure/accessor/batchorder/batchordercollectionacc_definitions.h"

#include "infrastructure/accessor/orderproposal/orderproposalacc.h"
#include "infrastructure/accessor/orderproposal/orderproposalacc_definitions.h"

#include "infrastructure/accessor/orderproposal/orderproposalcollectionacc.h"
#include "infrastructure/accessor/orderproposal/orderproposalcollectionacc_definitions.h"

#include "infrastructure/accessor/purchasebatchorder/purchasebatchorderposacc.h"
#include "infrastructure/accessor/purchasebatchorder/purchasebatchorderposacc_definitions.h"

#include "infrastructure/accessor/orderprotocol/orderheadprotocolacc.h"
#include "infrastructure/accessor/orderprotocol/orderheadprotocolacc_definitions.h"

#include "infrastructure/accessor/orderprotocol/orderposprotocolacc.h"
#include "infrastructure/accessor/orderprotocol/orderposprotocolacc_definitions.h"


// FACTORIES
#include "orderproposalfactory.h"
#include "orderproposalfactoryptr.h"
#include "batchorderconnectedfactory.h"
#include "batchorderconnectedfactoryptr.h"

#include "orderproposalcollectionfactory.h"
#include "orderproposalcollectionfactoryptr.h"

#include "transferorderpositionconnectedfactory.h"
#include "transferorderpositionconnectedfactoryptr.h"

#include "reservequantityfactorgetter.h"
#include "reservequantityfactorgetterptr.h"

#include "orderlockfactory.h"
#include "orderlockfactoryptr.h"

// logging
#include "loggerpool/libabbauw_loggerpool.h"
#include <libutil/misc/metaclassmacro_definitions.h>

#include <boost/make_shared.hpp>
#include <lockmanager.h>
#include <lockmanagerptr.h>
#include <lockguardfactory.h>
#include <lockguardfactoryptr.h>
#include <lockaccessor.h>
#include <lockaccessorptr.h>

namespace libabbauw
{
	namespace componentManager
	{
		DMComponentManager::DMComponentManager()
			: m_Logger(LoggerPool::loggerLibAbbaUW)
		{
		}

		DMComponentManager::~DMComponentManager()
		{
		}

		const log4cplus::Logger& DMComponentManager::getLogger() const
		{
			return m_Logger;
		}

		void DMComponentManager::setSession(libutil::infrastructure::session::ISessionPtr session)
		{
			m_Session = session;
		}

		void DMComponentManager::setProgramName(const basar::VarString& programName)
		{
			m_ProgramName = programName;
		}

		void DMComponentManager::injectDBConnection(libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr dbConnectionDataGetter)
		{
			m_ConnectionDataGetter = dbConnectionDataGetter;
		}

		libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr DMComponentManager::getDBConnectionDataGetter()
		{
			return m_ConnectionDataGetter;
		}

		void DMComponentManager::injectCSCOrderCreator(domMod::cscorder::ICSCOrderCreatorPtr ptr)
		{
			m_CSCOrderCreatorPtr = ptr;
		}

		libutil::infrastructure::db::dbTransaction::IDBTransactionPtr DMComponentManager::createDBTransaction()
		{
			METHODNAME_DEF(DMComponentManager, createDBTransaction)
				BLOG_TRACE_METHOD(m_Logger, fun);

			return boost::make_shared< libutil::infrastructure::db::dbTransaction::TransactionGuard >(getDBConnectionDataGetter()->getCurrentConnection());
		}

		basar::Int16 DMComponentManager::getBranchNo()
		{
			return m_Session->getUser()->getAreaID();
		}

		// ######################################################################################################################################
		// DM
		// ######################################################################################################################################

		domMod::batchOrder::IBatchOrderCollectionFinderPtr DMComponentManager::getBatchOrderCollectionFinder()
		{
			METHODNAME_DEF(DMComponentManager, getBatchOrderCollectionFinder)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::batchOrder::BatchOrderCollectionDMPtr dm = boost::make_shared< domMod::batchOrder::BatchOrderCollectionDM >();
			dm->injectBatchOrderCollectionAccessor(getBatchOrderCollectionAccessor());
			dm->injectBatchOrderConnectedFactory(getBatchOrderConnectedFactory());

			return dm;
		}

		domMod::batchOrder::IBatchOrderPtr DMComponentManager::getBatchOrder(basar::db::aspect::AccessorPropertyTable_YIterator yit)
		{
			METHODNAME_DEF(DMComponentManager, getBatchOrder)
				BLOG_TRACE_METHOD(getLogger(), fun);

			return getBatchOrderConnectedFactory()->create(yit);
		}

		domMod::stock::IStockCollectionDMPtr DMComponentManager::getStockCollectionDM()
		{
			METHODNAME_DEF(DMComponentManager, getStockCollectionDM)
				BLOG_TRACE_METHOD(getLogger(), fun);

			if (0 == m_StockCollectionDMPtr.get())
			{
				m_StockCollectionDMPtr = boost::make_shared< domMod::stock::StockCollectionDM >(getDBConnectionDataGetter()->getCurrentConnection());
				m_StockCollectionDMPtr->injectStockCalcParam(getParameterDM());

				m_StockCollectionDMPtr->init(domMod::stock::StockCollectionInitParams(getBranchNo()));
			}
			return m_StockCollectionDMPtr;
		}

		domMod::transferOrder::ITransferOrderHeadPtr DMComponentManager::getTransferOrderHead()
		{
			METHODNAME_DEF(DMComponentManager, getTransferOrderHead)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::transferOrder::TransferOrderHeadPtr dm = boost::make_shared< domMod::transferOrder::TransferOrderHead >(getDBConnectionDataGetter()->getCurrentConnection());
			dm->init(domMod::transferOrder::TransferOrderHeadInitParams(getBranchNo()));

			return dm;
		}

		domMod::transferOrder::ITransferOrderPosCollectionPtr DMComponentManager::getTransferOrderPosCollectionDM(domMod::transferOrder::IOrderTypeGetterPtr orderTypeGetter)
		{
			domMod::transferOrder::TransferOrderPosCollectionPtr dm = boost::make_shared< domMod::transferOrder::TransferOrderPosCollection >(getDBConnectionDataGetter()->getCurrentConnection());
			dm->injectStockCollection(getStockCollectionDM());
			dm->injectOrderTypeGetter(orderTypeGetter);
			dm->injectTransferOrderPositionConnectedFactory(getTransferOrderPositionConnectedFactory());
			dm->init(domMod::transferOrder::TransferOrderPosInitParams(getBranchNo()));

			return dm;
		}

		domMod::transferOrder::ITransferOrderCreationDMPtr DMComponentManager::getTransferOrderCreationDM()
		{
			METHODNAME_DEF(DMComponentManager, getTransferOrderCreationDM)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::transferOrder::TransferOrderCreationDMPtr dm = boost::make_shared< domMod::transferOrder::TransferOrderCreationDM >();
			dm->injectTransferOrder(getTransferOrderDMGetter()->getTransferOrderDM());
			dm->injectArticleConsultant(getFindArticleConsultant());
			dm->injectOrderCreationParameter(getParameterDM());

			return dm;
		}

		domMod::transferOrder::ITransferOrderSplitDMPtr DMComponentManager::getTransferOrderColorSplitDM()
		{
			METHODNAME_DEF(DMComponentManager, getTransferOrderColorSplitDM)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::transferOrder::TransferOrderSplitDMPtr dm = boost::make_shared< domMod::transferOrder::TransferOrderSplitDM >();
			dm->injectTransactionFactory(shared_from_this());
			dm->injectCreateSplittedHeadFactory(getConcreteTransferOrderFactory());
			dm->injectOrderProposal(getOrderProposalUpdater());
			dm->injectSplittingPolicy(boost::make_shared< domMod::transferOrder::splittingPolicy::ColorSplittingPolicy >());

			return dm;
		}

		domMod::transferOrder::ITransferOrderSplitDMPtr DMComponentManager::getTransferOrderNarcoticsRefrigeratedSplitDM()
		{
			METHODNAME_DEF(DMComponentManager, getTransferOrderNarcoticsRefrigeratedSplitDM)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::transferOrder::TransferOrderSplitDMPtr dm = boost::make_shared< domMod::transferOrder::TransferOrderSplitDM >();
			dm->injectTransactionFactory(shared_from_this());
			dm->injectCreateSplittedHeadFactory(getConcreteTransferOrderFactory());
			dm->injectOrderProposal(getOrderProposalUpdater());
			dm->injectSplittingPolicy(boost::make_shared< domMod::transferOrder::splittingPolicy::NarcoticsRefrigeratedSplittingPolicy >());

			return dm;
		}

		domMod::transferOrder::ITransferOrderInPreparationDMPtr DMComponentManager::getTransferOrderInPreparationDM()
		{
			METHODNAME_DEF(DMComponentManager, getTransferOrderInPreparationDM)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::transferOrder::TransferOrderInPreparationDMPtr dm = boost::make_shared< domMod::transferOrder::TransferOrderInPreparationDM >();
			dm->injectTransactionFactory(shared_from_this());
			dm->injectHead(getTransferOrderHeadInPreparation());
			dm->injectPos(getTransferOrderPosInPreparationCollection());

			return dm;
		}

		domMod::transferOrder::ITransferOrderHeadInPreparationPtr DMComponentManager::getTransferOrderHeadInPreparation()
		{
			METHODNAME_DEF(DMComponentManager, getTransferOrderHeadInPreparation)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::transferOrder::TransferOrderHeadInPreparationPtr dm = boost::make_shared< domMod::transferOrder::TransferOrderHeadInPreparation >(getDBConnectionDataGetter()->getCurrentConnection());
			dm->init(domMod::transferOrder::TransferOrderHeadInPreparationInitParams(getBranchNo(), m_Session->getUser()->getUserName()));
			dm->injectTransactionFactory(shared_from_this());

			return dm;
		}

		domMod::transferOrder::ITransferOrderPosInPreparationCollectionPtr DMComponentManager::getTransferOrderPosInPreparationCollection()
		{
			METHODNAME_DEF(DMComponentManager, getTransferOrderPosInPreparationCollection)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::transferOrder::TransferOrderPosInPreparationCollectionPtr dm = boost::make_shared< domMod::transferOrder::TransferOrderPosInPreparationCollection >(getDBConnectionDataGetter()->getCurrentConnection());
			dm->init(domMod::transferOrder::TransferOrderPosInPreparationCollectionInitParams(getBranchNo(), m_Session->getUser()->getUserName()));

			return dm;
		}

		domMod::orderProtocol::IOrderHeadProtocolPtr DMComponentManager::getOrderProtocolHead()
		{
			METHODNAME_DEF(DMComponentManager, getOrderProtocolHead)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::orderProtocol::OrderHeadProtocolDMPtr dm = boost::make_shared< domMod::orderProtocol::OrderHeadProtocolDM >();
			dm->injectAccessor(getOrderProtocolHeadAccessor());
			return dm;
		}

		domMod::orderProtocol::IOrderPosCollectionProtocolPtr DMComponentManager::getOrderProtocolPosCollection()
		{
			METHODNAME_DEF(DMComponentManager, getOrderProtocolPosCollection)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::orderProtocol::OrderPosCollectionProtocolDMPtr dm = boost::make_shared< domMod::orderProtocol::OrderPosCollectionProtocolDM >();
			dm->injectAccessor(getOrderProtocolPosCollectionAccessor());

			return dm;
		}

		domMod::cscorder::ICSCOrderReleaserPtr DMComponentManager::getCSCOrderDM(domMod::cscorder::IOrderGetterPtr orderGetter)
		{
			METHODNAME_DEF(DMComponentManager, getCSCOrderDM)
				BLOG_TRACE_METHOD(getLogger(), fun);

			// cscorder of libabbauw!! the other one is deprecated in wrapped in cscOrderCreator!
			domMod::cscorder::CSCOrderDMPtr dm = boost::make_shared< domMod::cscorder::CSCOrderDM >(getDBConnectionDataGetter()->getCurrentConnection());
			dm->injectCSCOrderCreator(m_CSCOrderCreatorPtr);
			dm->injectOrderGetter(orderGetter);

			dm->init(domMod::cscorder::CSCOrderInitParams(getBranchNo()));
			return dm;
		}

		domMod::purchaseordersupplierdm::IFindPurchaseOrderSupplierPtr DMComponentManager::getFindPurchaseSupplier()
		{
			METHODNAME_DEF(DMComponentManager, getFindPurchaseSupplier)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::purchaseordersupplierdm::PurchaseOrderSupplierDMPtr dm = boost::make_shared< domMod::purchaseordersupplierdm::PurchaseOrderSupplierDM >(getDBConnectionDataGetter()->getCurrentConnection());
			return dm;
		}

		domMod::parameter::IParameterDMPtr DMComponentManager::getParameterDM()
		{
			METHODNAME_DEF(DMComponentManager, getParameterDM)
				BLOG_TRACE_METHOD(getLogger(), fun);

			if (0 == m_ParameterDMPtr.get())
			{
				m_ParameterDMPtr = boost::make_shared< domMod::parameter::ParameterDM >(getDBConnectionDataGetter()->getCurrentConnection());
				m_ParameterDMPtr->injectTransactionFactory(shared_from_this());

				m_ParameterDMPtr->init(domMod::parameter::ParameterInitParams(getBranchNo(), m_ProgramName, "User Setting"));
			}

			return m_ParameterDMPtr;
		}

		domMod::orderProposal::IOrderProposalCollectionPtr DMComponentManager::getOrderProposalCollection()
		{
			METHODNAME_DEF(DMComponentManager, getOrderProposalCollection)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::orderProposal::OrderProposalCollectionDMPtr dm = boost::make_shared< domMod::orderProposal::OrderProposalCollectionDM >();
			dm->injectOrderProposalCollectionAccessor(getOrderProposalCollectionAccessor());
			dm->injectOrderProposalFactory(getOrderProposalFactory());

			return dm;
		}

		domMod::orderProposal::IOrderProposalCollectionFinderPtr DMComponentManager::getOrderProposalCollectionFinder()
		{
			METHODNAME_DEF(DMComponentManager, getOrderProposalCollectionFinder)
				BLOG_TRACE_METHOD(getLogger(), fun);

			return getOrderProposalCollection();
		}

		domMod::orderProposal::IOrderProposalCollectionProtectionProxyPtr DMComponentManager::getOrderProposalCollectionProtectionProxy()
		{
			METHODNAME_DEF(DMComponentManager, getOrderProposalCollectionProtectionProxy)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::orderProposal::OrderProposalCollectionProtectionProxyPtr proxy =
				boost::make_shared< domMod::orderProposal::OrderProposalCollectionProtectionProxy>();
			proxy->injectOrderProposalCollection(getOrderProposalCollection());

			return proxy;
		}

		domMod::orderProposal::IOrderProposalPtr DMComponentManager::getOrderProposal()
		{
			METHODNAME_DEF(DMComponentManager, getOrderProposal)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::orderProposal::OrderProposalDMPtr dm = boost::make_shared< domMod::orderProposal::OrderProposalDM >();
			dm->injectOrderProposalAccessor(getOrderProposalAccessor());

			return dm;
		}

		domMod::purchaseBatchOrder::IPurchaseBatchOrderPosUpdaterPtr DMComponentManager::getPurchaseBatchOrderPosUpdater()
		{
			METHODNAME_DEF(DMComponentManager, getPurchaseBatchOrderPosFinder)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::purchaseBatchOrder::PurchaseBatchOrderPosDMPtr dm = boost::make_shared< domMod::purchaseBatchOrder::PurchaseBatchOrderPosDM >();
			dm->injectPurchaseBatchOrderPosAccessor(getPurchaseBatchOrderPosAccessor());

			return dm;
		}

		domMod::orderProposal::IOrderProposalPtr DMComponentManager::getOrderProposalUpdater()
		{
			METHODNAME_DEF(DMComponentManager, getOrderProposalUpdater)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::orderProposal::OrderProposalUpdateDMPtr dm = boost::make_shared< domMod::orderProposal::OrderProposalUpdateDM >();
			dm->injectOrderProposal(getOrderProposal());
			dm->injectPurchaseBatchOrderPosUpdater(getPurchaseBatchOrderPosUpdater());
			dm->injectTransactionFactory(shared_from_this());

			return dm;
		}

		domMod::orderProposal::IOrderProposalCreatorPtr DMComponentManager::getOrderProposalCreator()
		{
			METHODNAME_DEF(DMComponentManager, getOrderProposalCreator)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::orderProposal::OrderProposalCreatorDMPtr dm = boost::make_shared< domMod::orderProposal::OrderProposalCreatorDM >();
			dm->injectOrderProposalFactory(getOrderProposalFactory());
			dm->injectCustomerFinder(getCustomerFinder());

			return dm;
		}

		domMod::customer::ICustomerFinderPtr DMComponentManager::getCustomerFinder()
		{
			METHODNAME_DEF(DMComponentManager, getCustomerFinder)
				BLOG_TRACE_METHOD(getLogger(), fun);

			typedef boost::shared_ptr< domMod::customer::CustomerDM > CustomerDMPtr;

			CustomerDMPtr dm = boost::make_shared< domMod::customer::CustomerDM >(getDBConnectionDataGetter()->getCurrentConnection());
			dm->init(domMod::customer::CustomerInitParams(getBranchNo()));

			return dm;
		}

		domMod::customer::ICustomerCollectionFinderPtr DMComponentManager::getCustomerCollectionFinder()
		{
			METHODNAME_DEF(DMComponentManager, getCustomerCollectionFinder)
				BLOG_TRACE_METHOD(getLogger(), fun);

			typedef boost::shared_ptr< domMod::customer::CustomerCollectionDM > CustomerCollectionDMPtr;

			CustomerCollectionDMPtr dm = boost::make_shared< domMod::customer::CustomerCollectionDM >(getDBConnectionDataGetter()->getCurrentConnection());
			dm->init(domMod::customer::CustomerCollectionInitParams(getBranchNo()));

			return dm;
		}

		domMod::manufacturerdm::IFindManufacturerPtr DMComponentManager::getFindManufacturerDM()
		{
			METHODNAME_DEF(DMComponentManager, getFindManufacturerDM)
				BLOG_TRACE_METHOD(getLogger(), fun);

			typedef boost::shared_ptr< domMod::manufacturerdm::ManufacturerDM > ManufacturerDMPtr;

			ManufacturerDMPtr dm = boost::make_shared< domMod::manufacturerdm::ManufacturerDM >(getDBConnectionDataGetter()->getCurrentConnection());
			dm->init(domMod::manufacturerdm::ManufacturerInitParams(getBranchNo()));

			return dm;
		}

		domMod::article::IFindArticlePtr DMComponentManager::getFindArticleCollectionDM()
		{
			METHODNAME_DEF(DMComponentManager, getFindArticleCollectionDM)
				BLOG_TRACE_METHOD(getLogger(), fun);

			if (0 == m_ArticleCollectionDMPtr.get())
			{
				m_ArticleCollectionDMPtr = boost::make_shared< domMod::article::ArticleCollectionDM >(getDBConnectionDataGetter()->getCurrentConnection());
				m_ArticleCollectionDMPtr->init(domMod::article::ArticleInitParams(getBranchNo()));
			}

			return m_ArticleCollectionDMPtr;
		}

		domMod::article::IFindArticlePricePtr DMComponentManager::getFindArticleConsultant()
		{
			METHODNAME_DEF(DMComponentManager, getFindArticleConsultant)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::article::ArticleConsultantDMPtr dm = boost::make_shared< domMod::article::ArticleConsultantDM >(getDBConnectionDataGetter()->getCurrentConnection());
			dm->init(domMod::article::ArticleConsultantInitParams(getBranchNo()));

			return dm;
		}

		ITransferOrderDMGetterPtr DMComponentManager::getTransferOrderDMGetter()
		{
			typedef boost::shared_ptr< TransferOrderDMGetter > TransferOrderDMGetterPtr;
			TransferOrderDMGetterPtr transferOrderDMGetter = boost::make_shared< TransferOrderDMGetter >();

			transferOrderDMGetter->injectConnection(getDBConnectionDataGetter()->getCurrentConnection());
			transferOrderDMGetter->injectTransactionFactory(shared_from_this());
			transferOrderDMGetter->injectCscOrderGetter(shared_from_this());
			transferOrderDMGetter->injectTransferOrderHeadGetter(shared_from_this());
			transferOrderDMGetter->injectTransferOrderPosCollectionGetter(shared_from_this());
			transferOrderDMGetter->injectOrderReleaseParameter(getParameterDM());

			return transferOrderDMGetter;
		}

		ITransferOrderDMGetterPtr DMComponentManager::getTransferOrderLockedDMGetter()
		{
			typedef boost::shared_ptr< TransferOrderLockedDMGetter > TransferOrderLockedDMGetterPtr;
			TransferOrderLockedDMGetterPtr transferOrderLockedDMGetter = boost::make_shared< TransferOrderLockedDMGetter >();

			transferOrderLockedDMGetter->injectConnection(getDBConnectionDataGetter()->getCurrentConnection());
			transferOrderLockedDMGetter->injectTransferOrderDMGetter(getTransferOrderDMGetter());
			transferOrderLockedDMGetter->injectTransferOrderLockFactory(getOrderLockFactory());
			transferOrderLockedDMGetter->injectLockManager(getLockManager());

			return transferOrderLockedDMGetter;
		}

		ITransferOrderHeadCollectionDMGetterPtr DMComponentManager::getTransferOrderHeadCollectionDMGetter()
		{
			typedef boost::shared_ptr< TransferOrderHeadCollectionDMGetter > TransferOrderHeadCollectionDMGetterPtr;
			TransferOrderHeadCollectionDMGetterPtr headCollectionGetter = boost::make_shared< TransferOrderHeadCollectionDMGetter >();

			headCollectionGetter->setBranchNo(getBranchNo());
			headCollectionGetter->injectConnection(getDBConnectionDataGetter()->getCurrentConnection());
			headCollectionGetter->injectOrderParameter(getParameterDM());
			headCollectionGetter->injectStockCollection(getStockCollectionDM());
			headCollectionGetter->injectTransferOrderFactory(getTransferOrderProtectionProxyFactory());
			headCollectionGetter->injectTransferOrderHeadFactory(getTransferOrderHeadFactory());
			headCollectionGetter->injectReserveQuantityFactorGetterFactory(shared_from_this());

			return headCollectionGetter;
		}

		ITransferOrderHeadCollectionDMGetterPtr DMComponentManager::getTransferOrderHeadCollectionLockedDMGetter()
		{
			typedef boost::shared_ptr< TransferOrderHeadCollectionLockedDMGetter > TransferOrderHeadCollectionLockedDMGetterPtr;
			TransferOrderHeadCollectionLockedDMGetterPtr headCollectionGetter = boost::make_shared< TransferOrderHeadCollectionLockedDMGetter >();

			headCollectionGetter->injectLockManager(getLockManager());
			headCollectionGetter->injectTransferOrderHeadCollectionDMGetter(getTransferOrderHeadCollectionDMGetter());
			headCollectionGetter->injectTransferOrderLockFactory(getOrderLockFactory());

			return headCollectionGetter;
		}

		domMod::orderProtocol::IOrderProtocolPtr DMComponentManager::getOrderProtocolDM()
		{
			domMod::orderProtocol::OrderProtocolDMPtr dm = boost::make_shared< domMod::orderProtocol::OrderProtocolDM >();
			dm->injectOrderHeadProtocol(getOrderProtocolHead());
			dm->injectOrderPosCollectionProtocol(getOrderProtocolPosCollection());
			dm->injectTransactionFactory(shared_from_this());

			return dm;
		}

		// ######################################################################################################################################
		// ACCESSORS
		// ######################################################################################################################################
		libutil::infrastructure::accessor::IAccessorPtr DMComponentManager::getBatchOrderCollectionAccessor()
		{
			infrastructure::accessor::batchOrder::SelectBatchOrderCollectionByPattern();

			using libutil::infrastructure::accessor::AccessorProxy;
			return boost::make_shared< AccessorProxy >(infrastructure::accessor::batchOrder::lit::ACC_BATCH_ORDER_COLLECTION,
				getDBConnectionDataGetter()->getCurrentConnection(), basar::db::aspect::FULL_CACHING);
		}

		libutil::infrastructure::accessor::IAccessorPtr DMComponentManager::getOrderProposalAccessor()
		{
			infrastructure::accessor::orderProposal::SelectOrderProposalByKey();

			using libutil::infrastructure::accessor::AccessorProxy;
			return boost::make_shared< AccessorProxy >(infrastructure::accessor::orderProposal::lit::ACC_ORDERPROPOSAL,
				getDBConnectionDataGetter()->getCurrentConnection(), basar::db::aspect::FULL_CACHING);
		}

		libutil::infrastructure::accessor::IAccessorPtr DMComponentManager::getOrderProposalCollectionAccessor()
		{
			infrastructure::accessor::orderProposal::SelectOrderProposalsByKey();

			using libutil::infrastructure::accessor::AccessorProxy;
			return boost::make_shared< AccessorProxy >(infrastructure::accessor::orderProposal::lit::ACC_ORDERPROPOSALCOLLECTION,
				getDBConnectionDataGetter()->getCurrentConnection(), basar::db::aspect::FULL_CACHING);
		}

		libutil::infrastructure::accessor::IAccessorPtr DMComponentManager::getPurchaseBatchOrderPosAccessor()
		{
			infrastructure::accessor::purchaseBatchOrder::SelectPurchaseBatchOrderPosByKey();

			using libutil::infrastructure::accessor::AccessorProxy;
			return boost::make_shared< AccessorProxy >(infrastructure::accessor::purchaseBatchOrder::lit::ACC_PURCHASE_BATCH_ORDER_POS,
				getDBConnectionDataGetter()->getCurrentConnection(), basar::db::aspect::FULL_CACHING);
		}

		libutil::infrastructure::accessor::IAccessorPtr	DMComponentManager::getOrderProtocolHeadAccessor()
		{
			infrastructure::accessor::orderprotocol::SelectOrderHeadProtocolByKey();

			using libutil::infrastructure::accessor::AccessorProxy;
			return boost::make_shared< AccessorProxy >(infrastructure::accessor::orderprotocol::lit::ORDER_HEAD_PROTOCOL_ACC,
				getDBConnectionDataGetter()->getCurrentConnection(), basar::db::aspect::FULL_CACHING);
		}

		libutil::infrastructure::accessor::IAccessorPtr	DMComponentManager::getOrderProtocolPosCollectionAccessor()
		{
			infrastructure::accessor::orderprotocol::SelectOrderPosProtocolByKey();

			using libutil::infrastructure::accessor::AccessorProxy;
			return boost::make_shared< AccessorProxy >(infrastructure::accessor::orderprotocol::lit::ORDER_POS_PROTOCOL_ACC,
				getDBConnectionDataGetter()->getCurrentConnection(), basar::db::aspect::FULL_CACHING);
		}


		// ######################################################################################################################################
		// FACTORIES
		// ######################################################################################################################################

		IOrderProposalFactoryPtr DMComponentManager::getOrderProposalFactory()
		{
			METHODNAME_DEF(DMComponentManager, getOrderProposalFactory)
				BLOG_TRACE_METHOD(getLogger(), fun);

			OrderProposalFactoryPtr orderProposalFactory = boost::make_shared< OrderProposalFactory >();
			orderProposalFactory->injectDBConnectionDataGetter(getDBConnectionDataGetter());

			return orderProposalFactory;
		}

		IOrderProposalCollectionFactoryPtr DMComponentManager::getOrderProposalCollectionFactory()
		{
			METHODNAME_DEF(DMComponentManager, getOrderProposalCollectionFactory)
				BLOG_TRACE_METHOD(getLogger(), fun);

			OrderProposalCollectionFactoryPtr factory = boost::make_shared< OrderProposalCollectionFactory >();
			factory->injectOrderProposalCollectionProtectionProxyGetter(shared_from_this());
			factory->injectTransferOrderFactory(getConcreteTransferOrderFactory());
			factory->injectCustomerFinder(getCustomerFinder());
			factory->injectTransferOrderLockFactory(getOrderLockFactory());

			return factory;
		}

		IBatchOrderConnectedFactoryPtr DMComponentManager::getBatchOrderConnectedFactory()
		{
			METHODNAME_DEF(DMComponentManager, getBatchOrderConnectedFactory)
				BLOG_TRACE_METHOD(getLogger(), fun);

			BatchOrderConnectedFactoryPtr batchOrderFactory = boost::make_shared< BatchOrderConnectedFactory >();
			batchOrderFactory->injectDBConnection(getDBConnectionDataGetter());
			batchOrderFactory->injectCSCOrderGetter(shared_from_this());
			batchOrderFactory->injectOrderLockFactory(getOrderLockFactory());

			return batchOrderFactory;
		}

		ITransferOrderFactoryPtr DMComponentManager::getTransferOrderProtectionProxyFactory()
		{
			METHODNAME_DEF(DMComponentManager, getTransferOrderProtectionProxyFactory)
				BLOG_TRACE_METHOD(getLogger(), fun);

			TransferOrderFactoryPtr transferOrderFactory = boost::make_shared< TransferOrderFactory >();
			transferOrderFactory->injectTransferOrderDMGetter(getTransferOrderLockedDMGetter());

			return transferOrderFactory;
		}

		ITransferOrderHeadFactoryPtr DMComponentManager::getTransferOrderHeadFactory()
		{
			METHODNAME_DEF(DMComponentManager, getTransferOrderHeadFactory)
				BLOG_TRACE_METHOD(getLogger(), fun);

			return getConcreteTransferOrderHeadFactory();
		}

		ITransferOrderHeadCollectionFactoryPtr DMComponentManager::getLockedTransferOrderHeadCollectionFactory()
		{
			METHODNAME_DEF(DMComponentManager, getLockedTransferOrderHeadCollectionFactory)
				BLOG_TRACE_METHOD(getLogger(), fun);

			TransferOrderHeadCollectionFactoryPtr transferOrderHeadCollectionFactory = boost::make_shared< TransferOrderHeadCollectionFactory >();
			transferOrderHeadCollectionFactory->injectTransferOrderHeadCollectionGetter(getTransferOrderHeadCollectionLockedDMGetter());

			return transferOrderHeadCollectionFactory;
		}

		libabbauw::domMod::transferOrder::IReserveQuantityFactorGetterPtr DMComponentManager::getReserveQuantityFactorGetter(domMod::transferOrder::ReserveQuantityFactorType reserveQtyFactorType)
		{
			METHODNAME_DEF(DMComponentManager, getReserveQuantityFactorGetter)
				BLOG_TRACE_METHOD(getLogger(), fun);

			domMod::transferOrder::ReserveQuantityFactorGetterPtr rqfg = boost::make_shared< domMod::transferOrder::ReserveQuantityFactorGetter >(reserveQtyFactorType);
			rqfg->injectReserveQtyFactorParameter(getParameterDM());

			return rqfg;
		}

		lock::IOrderLockFactoryPtr DMComponentManager::getOrderLockFactory()
		{
			lock::OrderLockFactoryPtr orderLockFactory = boost::make_shared< lock::OrderLockFactory >();
			orderLockFactory->setSession(m_Session);
			orderLockFactory->injectLockGuardFactory(getLockGuardFactory());

			return orderLockFactory;
		}

		TransferOrderFactoryPtr DMComponentManager::getConcreteTransferOrderFactory()
		{
			METHODNAME_DEF(DMComponentManager, getConcreteTransferOrderFactory)
				BLOG_TRACE_METHOD(getLogger(), fun);

			TransferOrderFactoryPtr transferOrderFactory = boost::make_shared< TransferOrderFactory >();
			transferOrderFactory->injectTransferOrderDMGetter(getTransferOrderDMGetter());

			return transferOrderFactory;
		}

		TransferOrderHeadFactoryPtr DMComponentManager::getConcreteTransferOrderHeadFactory()
		{
			METHODNAME_DEF(DMComponentManager, getConcreteTransferOrderHeadFactory)
				BLOG_TRACE_METHOD(getLogger(), fun);

			TransferOrderHeadFactoryPtr transferOrderHeadFactory = boost::make_shared< TransferOrderHeadFactory >();
			transferOrderHeadFactory->injectTransferOrderHeadGetter(shared_from_this());

			return transferOrderHeadFactory;
		}

		ITransferOrderPositionConnectedFactoryPtr DMComponentManager::getTransferOrderPositionConnectedFactory()
		{
			METHODNAME_DEF(DMComponentManager, getTransferOrderPositionConnectedFactory)
				BLOG_TRACE_METHOD(getLogger(), fun);

			TransferOrderPositionConnectedFactoryPtr factory = boost::make_shared< TransferOrderPositionConnectedFactory >();
			factory->injectDBConnection(getDBConnectionDataGetter());

			return factory;
		}

		// ######################################################################################################################################
		// OTHER
		// ######################################################################################################################################
		libLockManager::ILockManagerPtr DMComponentManager::getLockManager()
		{
			libLockManager::LockManagerPtr lockManager = boost::make_shared< libLockManager::LockManager >();

			libLockManager::accessor::LockAccessorPtr lockAccessor = boost::make_shared< libLockManager::accessor::LockAccessor >("batchorderlock");
			lockAccessor->injectConnection(getDBConnectionDataGetter()->getCurrentConnection().getSqlConnection());

			lockManager->injectAccessor(lockAccessor);
			return lockManager;
		}

		libLockManager::ILockGuardFactoryPtr DMComponentManager::getLockGuardFactory()
		{
			libLockManager::LockGuardFactoryPtr lockGuardFactory = boost::make_shared< libLockManager::LockGuardFactory >();
			lockGuardFactory->injectLockManager(getLockManager());

			return lockGuardFactory;
		}
	} // end namespace componentManager
} // end namespace libabbauw
