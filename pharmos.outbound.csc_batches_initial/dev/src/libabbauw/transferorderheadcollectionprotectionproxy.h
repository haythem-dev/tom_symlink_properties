#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERHEADCOLLECTIONPROTECTIONPROXY_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERHEADCOLLECTIONPROTECTIONPROXY_H

#include "itransferorderheadcollectiondm.h"
#include "itransferorderheadcollectiondmptr.h"

#include "iorderlockfactoryptr.h"
#include <ilockmanagerptr.h>
#include <ilockentryptr.h>

#include <list>

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{
			using basar::db::aspect::AccessorPropertyTableRef;
			using basar::db::aspect::AccessorPropertyTable_YIterator;

			class TransferOrderHeadCollectionProtectionProxy : public ITransferOrderHeadCollectionDM
			{
			public:
				TransferOrderHeadCollectionProtectionProxy();
				~TransferOrderHeadCollectionProtectionProxy();

				void                                                    injectTransferOrderHeadCollection(ITransferOrderHeadCollectionDMPtr);
				void                                                    injectLockManager(libLockManager::ILockManagerPtr);
				void                                                    injectTransferOrderLockFactory(lock::IOrderLockFactoryPtr);

				// defined in ITransferOrderHeadCollectionDM
				AccessorPropertyTableRef								get();
				AccessorPropertyTableRef								get() const;

				void                                                    resetSearchYit();
				AccessorPropertyTable_YIterator							getSearchYit();

				void                                                    getGreenOrders(YIterators& greenOrders);
				basar::Int32                                            getNumberOfGreenOrders() const;
				basar::Int32                                            getTotalNumberOfOrders();

				bool                                                    save(basar::VarString strDiscount);

				void													setBackupAllHeadPosCollection();
				std::vector<AccessorPropertyTable_YIterator>			getBackupAllHeadPosCollection();

				void                                                    findByPattern(const AccessorPropertyTable_YIterator, bool isOrderView);
				void                                                    findByKey(const AccessorPropertyTable_YIterator);
				bool                                                    isChanged() const;

				transferOrder::ITransferOrderPtr                        getOrder(const AccessorPropertyTable_YIterator);
				void                                                    setReserveQtyFactorType(ReserveQuantityFactorType);

				void                                                    addTransferOrderHead(AccessorPropertyTable_YIterator);

			private:
				void                                                    lock(basar::Int32 orderNo);
				void                                                    releaseLocks();

			private:
				typedef std::list< libLockManager::ILockEntryPtr >      LockEntryCollection;
				LockEntryCollection                                     m_LockEntryCollection;

			private:
				ITransferOrderHeadCollectionDMPtr                       m_TransferOrderHeadCollection;
				libLockManager::ILockManagerPtr                         m_LockManager;
				lock::IOrderLockFactoryPtr                              m_TransferOrderLockFactory;
			};

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_TRANSFERORDERHEADCOLLECTIONPROTECTIONPROXY_H
