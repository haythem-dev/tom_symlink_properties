#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERLOCKEDDMGETTER_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERLOCKEDDMGETTER_H

#include "itransferorderdmgetter.h"
#include "itransferorderdmgetterptr.h"

#include <libbasardbaspect_connectionref.h>
#include "iorderlockfactoryptr.h"
#include <ilockmanagerptr.h>

namespace libabbauw
{
	namespace componentManager
	{
		class TransferOrderLockedDMGetter : public ITransferOrderDMGetter
		{
		public:
			TransferOrderLockedDMGetter();
			~TransferOrderLockedDMGetter();

			void                                                                 injectConnection(basar::db::aspect::ConnectionRef);
			void                                                                 injectTransferOrderDMGetter(ITransferOrderDMGetterPtr);
			void                                                                 injectLockManager(libLockManager::ILockManagerPtr);
			void                                                                 injectTransferOrderLockFactory(lock::IOrderLockFactoryPtr);

			// from ITransferOrderDMGetter interface
			domMod::transferOrder::ITransferOrderPtr                             getTransferOrderDM();

		private:
			// forbidden
			TransferOrderLockedDMGetter(const TransferOrderLockedDMGetter&);
			TransferOrderLockedDMGetter& operator = (const TransferOrderLockedDMGetter&);

		private:
			basar::db::aspect::ConnectionRef	                                 m_Connection;
			ITransferOrderDMGetterPtr                                            m_TransferOrderDMGetter;
			lock::IOrderLockFactoryPtr                                           m_TransferOrderLockFactory;
			libLockManager::ILockManagerPtr                                      m_LockManager;
		};
	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERLOCKEDDMGETTER_H
