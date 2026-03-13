#ifndef LIBABBAUW_COMPONENTMANAGER_ORDERPROPOSALCOLLECTIONFACTORY_H
#define LIBABBAUW_COMPONENTMANAGER_ORDERPROPOSALCOLLECTIONFACTORY_H

#include "iorderproposalcollectionfactory.h"
#include "itransferorderfactoryptr.h"
#include "icustomerfinderptr.h"

#include "iorderproposalcollectionprotectionproxygetterptr.h"
#include "iorderlockfactoryptr.h"

#include <ilockguardptr.h>
#include <libbasar_definitions.h>

namespace libabbauw
{
	namespace componentManager
	{
		class OrderProposalCollectionFactory : public IOrderProposalCollectionFactory
		{
		public:
			OrderProposalCollectionFactory();
			~OrderProposalCollectionFactory();

			void                                                                          injectOrderProposalCollectionProtectionProxyGetter(IOrderProposalCollectionProtectionProxyGetterPtr);
			void                                                                          injectTransferOrderFactory(ITransferOrderFactoryPtr);
			void                                                                          injectCustomerFinder(domMod::customer::ICustomerFinderPtr);

			void                                                                          injectTransferOrderLockFactory(lock::IOrderLockFactoryPtr);

			// from ITransferOrderHeadCollectionFactory interface
			domMod::orderProposal::IOrderProposalCollectionPtr                            create(const AccessorPropertyTableYIteratorCollection&);

		private:
			// don't allow assignment and copying of this class
			OrderProposalCollectionFactory(const OrderProposalCollectionFactory&);
			OrderProposalCollectionFactory& operator=(const OrderProposalCollectionFactory&);

			bool                                                                          isCustomerLocked(basar::Int32 pharmacyNo) const;
			libLockManager::ILockGuardPtr                                                 createLockGuard(basar::Int32 orderNo);

		private:
			IOrderProposalCollectionProtectionProxyGetterPtr                              m_OrderProposalCollectionProtectionProxyGetter;
			ITransferOrderFactoryPtr                                                      m_TransferOrderFactory;
			domMod::customer::ICustomerFinderPtr                                          m_CustomerFinder;

			lock::IOrderLockFactoryPtr                                                    m_TransferOrderLockFactory;
		};

	} // end namespace componentManager
} // end namespace libabbauw

#endif // LIBABBAUW_COMPONENTMANAGER_ORDERPROPOSALCOLLECTIONFACTORY_H

