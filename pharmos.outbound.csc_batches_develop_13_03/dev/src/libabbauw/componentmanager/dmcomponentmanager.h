#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_DMCOMPONENTMANAGER_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_DMCOMPONENTMANAGER_H

// ComponentManager interfaces
#include "itransferorderheadgetter.h"
#include "itransferorderposcollectiongetter.h"
#include "icscordergetter.h"
#include "ireservequantityfactorgetterfactory.h"
#include "iorderproposalcollectionprotectionproxygetter.h"
#include "iorderproposalfactoryptr.h"
#include "iorderprotocolgetter.h"

// DMs
#include "domainmodule/batchorder/ibatchordercollectionfinderptr.h"
#include "domainmodule/batchorder/ibatchorderptr.h"

#include "iordertypegetterptr.h"
#include "itransferorderptr.h"
#include "itransferorderheadptr.h"
#include "itransferorderposcollectionptr.h"

#include "stockcollectiondmptr.h"
#include "istockcollectiondmptr.h"

#include "cscorderdmptr.h"
#include "icscordercreatorptr.h"

#include "domainmodule/orderproposal/iorderproposalcollectionptr.h"
#include "domainmodule/orderproposal/iorderproposalcollectionfinderptr.h"

#include "domainmodule/orderproposal/iorderproposalptr.h"
#include "domainmodule/orderproposal/iorderproposalcreatorptr.h"

#include "domainmodule/orderprotocol/iorderprotocolptr.h"
#include "domainmodule/orderprotocol/iorderheadprotocolptr.h"
#include "domainmodule/orderprotocol/iorderposcollectionprotocolptr.h"

#include "icustomerfinderptr.h"
#include "icustomercollectionfinderptr.h"
#include "ifindmanufacturerptr.h"

#include "articlecollectiondmptr.h"
#include "ifindarticleptr.h"

#include "articleconsultantdmptr.h"
#include "ifindarticlepriceptr.h"

#include "ipurchasebatchorderposupdaterptr.h"

#include "iparameterdmptr.h"
#include "parameterdmptr.h"

#include "ifindpurchaseordersupplierptr.h"

#include "itransferorderinpreparationdmptr.h"
#include "transferorderinpreparationdmptr.h"
#include "itransferorderheadinpreparationptr.h"
#include "itransferorderposinpreparationcollectionptr.h"

#include "itransferordercreationdmptr.h"
#include "itransferordersplitdmptr.h"

#include "ibatchorderconnectedfactoryptr.h"
#include "iorderproposalcollectionfactoryptr.h"

#include "itransferorderdmgetterptr.h"
#include "transferorderfactoryptr.h"

#include "transferorderheadfactoryptr.h"
#include "itransferorderfactoryptr.h"
#include "itransferorderheadfactoryptr.h"

#include "itransferorderheadcollectiondmgetterptr.h"
#include "itransferorderheadcollectionfactoryptr.h"

#include "itransferorderpositionconnectedfactoryptr.h"

#include "iorderlockfactoryptr.h"
#include <ilockmanagerptr.h>
#include <ilockguardfactoryptr.h>

#include <libutil/dbtransaction.h>
#include <libutil/dbconnection.h>
#include <libutil/accessor.h>
#include <libutil/infrastructure/session/isessionptr.h>

#include <boost/enable_shared_from_this.hpp>

//----------------------------------------------------------------------------//
// forward declaration section
//----------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTableRef;
			class AccessorPropertyTable_YIterator;
		}
	}
}

namespace libabbauw
{
	namespace componentManager
	{
		class DMComponentManager : public boost::enable_shared_from_this < DMComponentManager >,
			public libutil::infrastructure::db::dbTransaction::IDBTransactionFactory,
			public ITransferOrderHeadGetter,
			public ITransferOrderPosCollectionGetter,
			public ICSCOrderGetter,
			public IOrderProposalCollectionProtectionProxyGetter,
			public IReserveQuantityFactorGetterFactory,
			public IOrderProtocolGetter
		{
		public:
			DMComponentManager();
			~DMComponentManager();

			void															                  setSession(libutil::infrastructure::session::ISessionPtr);
			void                                                                              setProgramName(const basar::VarString&);
			void                                                                              injectDBConnection(libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr);
			void                                                                              injectCSCOrderCreator(domMod::cscorder::ICSCOrderCreatorPtr);

			domMod::batchOrder::IBatchOrderCollectionFinderPtr                                getBatchOrderCollectionFinder();
			domMod::batchOrder::IBatchOrderPtr                                                getBatchOrder(basar::db::aspect::AccessorPropertyTable_YIterator);

			domMod::transferOrder::ITransferOrderHeadPtr                                      getTransferOrderHead();
			domMod::transferOrder::ITransferOrderPosCollectionPtr                             getTransferOrderPosCollectionDM(domMod::transferOrder::IOrderTypeGetterPtr);

			domMod::transferOrder::ITransferOrderInPreparationDMPtr                           getTransferOrderInPreparationDM();
			domMod::transferOrder::ITransferOrderCreationDMPtr                                getTransferOrderCreationDM();

			domMod::transferOrder::ITransferOrderSplitDMPtr                                   getTransferOrderColorSplitDM();
			domMod::transferOrder::ITransferOrderSplitDMPtr                                   getTransferOrderNarcoticsRefrigeratedSplitDM();

			domMod::stock::IStockCollectionDMPtr                                              getStockCollectionDM();
			domMod::cscorder::ICSCOrderReleaserPtr                                            getCSCOrderDM(domMod::cscorder::IOrderGetterPtr);

			domMod::orderProtocol::IOrderProtocolPtr										  getOrderProtocolDM();

			domMod::purchaseordersupplierdm::IFindPurchaseOrderSupplierPtr                    getFindPurchaseSupplier();
			domMod::parameter::IParameterDMPtr                                                getParameterDM();

			domMod::customer::ICustomerFinderPtr                                              getCustomerFinder();
			domMod::customer::ICustomerCollectionFinderPtr                                    getCustomerCollectionFinder();

			domMod::manufacturerdm::IFindManufacturerPtr									  getFindManufacturerDM();
			domMod::article::IFindArticlePtr									              getFindArticleCollectionDM();
			domMod::article::IFindArticlePricePtr                                             getFindArticleConsultant();

			domMod::orderProposal::IOrderProposalCollectionFinderPtr                          getOrderProposalCollectionFinder();
			domMod::orderProposal::IOrderProposalCollectionProtectionProxyPtr                 getOrderProposalCollectionProtectionProxy();

			domMod::orderProposal::IOrderProposalPtr                                          getOrderProposal();
			domMod::orderProposal::IOrderProposalPtr                                          getOrderProposalUpdater();
			domMod::orderProposal::IOrderProposalCreatorPtr			                          getOrderProposalCreator();

			ITransferOrderHeadCollectionDMGetterPtr                                           getTransferOrderHeadCollectionDMGetter();
			ITransferOrderHeadCollectionDMGetterPtr                                           getTransferOrderHeadCollectionLockedDMGetter();

			ITransferOrderFactoryPtr                                                          getTransferOrderProtectionProxyFactory();
			ITransferOrderHeadCollectionFactoryPtr                                            getLockedTransferOrderHeadCollectionFactory();

			IOrderProposalCollectionFactoryPtr                                                getOrderProposalCollectionFactory();

			libabbauw::domMod::transferOrder::IReserveQuantityFactorGetterPtr                 getReserveQuantityFactorGetter(domMod::transferOrder::ReserveQuantityFactorType);

		private:
			// don't allow assignment and copying of this class
			DMComponentManager(const DMComponentManager&);
			DMComponentManager& operator=(const DMComponentManager&);

		private:
			const log4cplus::Logger& getLogger() const;
			libutil::infrastructure::db::dbTransaction::IDBTransactionPtr                     createDBTransaction();

			libLockManager::ILockManagerPtr                                                   getLockManager();
			libLockManager::ILockGuardFactoryPtr                                              getLockGuardFactory();
			libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr             getDBConnectionDataGetter();
			basar::Int16                                                                      getBranchNo();

			domMod::transferOrder::ITransferOrderHeadInPreparationPtr                         getTransferOrderHeadInPreparation();
			domMod::transferOrder::ITransferOrderPosInPreparationCollectionPtr                getTransferOrderPosInPreparationCollection();

			domMod::orderProtocol::IOrderHeadProtocolPtr									  getOrderProtocolHead();
			domMod::orderProtocol::IOrderPosCollectionProtocolPtr							  getOrderProtocolPosCollection();

			domMod::purchaseBatchOrder::IPurchaseBatchOrderPosUpdaterPtr                      getPurchaseBatchOrderPosUpdater();

			domMod::orderProposal::IOrderProposalCollectionPtr                                getOrderProposalCollection();

			libutil::infrastructure::accessor::IAccessorPtr                                   getBatchOrderCollectionAccessor();

			// ACCESSORS
			libutil::infrastructure::accessor::IAccessorPtr                                   getOrderProposalAccessor();
			libutil::infrastructure::accessor::IAccessorPtr                                   getOrderProposalCollectionAccessor();
			libutil::infrastructure::accessor::IAccessorPtr                                   getPurchaseBatchOrderPosAccessor();

			libutil::infrastructure::accessor::IAccessorPtr									  getOrderProtocolHeadAccessor();
			libutil::infrastructure::accessor::IAccessorPtr									  getOrderProtocolPosCollectionAccessor();

			// FACTORIES
			IBatchOrderConnectedFactoryPtr                                                    getBatchOrderConnectedFactory();
			IOrderProposalFactoryPtr                                                          getOrderProposalFactory();

			ITransferOrderHeadFactoryPtr                                                      getTransferOrderHeadFactory();
			ITransferOrderPositionConnectedFactoryPtr                                         getTransferOrderPositionConnectedFactory();

			TransferOrderFactoryPtr                                                           getConcreteTransferOrderFactory();
			TransferOrderHeadFactoryPtr                                                       getConcreteTransferOrderHeadFactory();

			ITransferOrderDMGetterPtr                                                         getTransferOrderDMGetter();
			ITransferOrderDMGetterPtr                                                         getTransferOrderLockedDMGetter();

			lock::IOrderLockFactoryPtr                                                        getOrderLockFactory();

		private:
			const log4cplus::Logger& m_Logger;
			libutil::infrastructure::db::dbConnection::IDBConnectionDataGetterPtr             m_ConnectionDataGetter;
			libutil::infrastructure::session::ISessionPtr                                     m_Session;

			domMod::stock::StockCollectionDMPtr                                               m_StockCollectionDMPtr;
			domMod::article::ArticleCollectionDMPtr                                           m_ArticleCollectionDMPtr;
			domMod::parameter::ParameterDMPtr                                                 m_ParameterDMPtr;

			basar::VarString                                                                  m_ProgramName;
			domMod::cscorder::ICSCOrderCreatorPtr                                             m_CSCOrderCreatorPtr;
		};
	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_DMCOMPONENTMANAGER_H
