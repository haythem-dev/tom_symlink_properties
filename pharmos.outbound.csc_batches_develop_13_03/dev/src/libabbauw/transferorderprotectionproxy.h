#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDERDM_TRANSFERORDERPROTECTIONPROXY_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDERDM_TRANSFERORDERPROTECTIONPROXY_H

#include "itransferorder.h"
#include "itransferorderptr.h"

#include "iorderlockfactoryptr.h"

#include <ilockmanagerptr.h>
#include <ilockentryptr.h>

namespace log4cplus
{
	class Logger;
}

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{

			class TransferOrderProtectionProxy : public ITransferOrder
			{
			public:
				TransferOrderProtectionProxy();
				~TransferOrderProtectionProxy();

				void                                                     injectTransferOrder(ITransferOrderPtr);
				void                                                     injectLockManager(libLockManager::ILockManagerPtr);
				void                                                     injectTransferOrderLockFactory(lock::IOrderLockFactoryPtr);

				// defined in ITransferOrderGetter interface
				ITransferOrderHeadPtr                                    getHead();
				ITransferOrderHeadPtr                                    getHead()          const;
				ITransferOrderPosCollectionPtr                           getPosCollection();
				ITransferOrderPosCollectionPtr                           getPosCollection() const;

				bool                                                     isEmpty()          const;

				basar::Int32                                             getOrderNo()       const;
				basar::Int32                                             getPositionCount() const;
				basar::Decimal                                           getOrderValue()    const;
				OrderStatus                                              getOrderStatus()   const;
				TrafficLightEnum                                         getColor()         const;

				// defined in ITransferOrderDM interface
				void                                                     findById(basar::Int32 orderNo);

				// used only for CPR0002688 to avoid double locking
				void                                                     findByIdWithoutLock(basar::Int32 orderNo);

				void                                                     save();
				void                                                     clear();
				bool                                                     isChanged() const;

				basar::db::aspect::AccessorPropertyTable_YIterator       addEmptyPosition();
				void                                                     removePosition(basar::db::aspect::AccessorPropertyTable_YIterator);

				void                                                     release(bool postpone);

				bool                                                     hasOrderTypeOM() const;

			private:
				// forbidden
				TransferOrderProtectionProxy(const TransferOrderProtectionProxy&);
				TransferOrderProtectionProxy operator = (const TransferOrderProtectionProxy&);

				const log4cplus::Logger& getLogger() const;
				void                                                     lock(basar::Int32 orderNo);
				void                                                     unlock();
				void                                                     checkForDataRace();

			private:
				const log4cplus::Logger& m_Logger;
				ITransferOrderPtr                                        m_TransferOrder;
				libLockManager::ILockManagerPtr                          m_LockManager;
				lock::IOrderLockFactoryPtr                               m_TransferOrderLockFactory;

				libLockManager::ILockEntryPtr                            m_LockEntry;
			};

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDERDM_TRANSFERORDERPROTECTIONPROXY_H
