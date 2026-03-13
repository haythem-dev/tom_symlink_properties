#include "transferorderlockeddmgetter.h"

#include "transferorderprotectionproxy.h"
#include "transferorderprotectionproxyptr.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include <libutil/misc/metaclassmacro_definitions.h>
#include <libbasarcmnutil_logging.h>

#include <lockmanagerptr.h>

#include <boost/make_shared.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		TransferOrderLockedDMGetter::TransferOrderLockedDMGetter()
		{
		}

		TransferOrderLockedDMGetter::~TransferOrderLockedDMGetter()
		{
		}

		void TransferOrderLockedDMGetter::injectConnection(basar::db::aspect::ConnectionRef connection)
		{
			m_Connection = connection;
		}

		void TransferOrderLockedDMGetter::injectTransferOrderDMGetter(ITransferOrderDMGetterPtr transferOrderDMGetter)
		{
			m_TransferOrderDMGetter = transferOrderDMGetter;
		}

		void TransferOrderLockedDMGetter::injectTransferOrderLockFactory(lock::IOrderLockFactoryPtr transferOrderLockFactory)
		{
			m_TransferOrderLockFactory = transferOrderLockFactory;
		}

		void TransferOrderLockedDMGetter::injectLockManager(libLockManager::ILockManagerPtr lockManager)
		{
			m_LockManager = lockManager;
		}

		domMod::transferOrder::ITransferOrderPtr TransferOrderLockedDMGetter::getTransferOrderDM()
		{
			METHODNAME_DEF(TransferOrderLockedDMGetter, getTransferOrderDM);
			BLOG_TRACE_METHOD(LoggerPool::loggerLibAbbaUW, fun);

			domMod::transferOrder::TransferOrderProtectionProxyPtr proxy = boost::make_shared< domMod::transferOrder::TransferOrderProtectionProxy >();
			proxy->injectTransferOrder(m_TransferOrderDMGetter->getTransferOrderDM());
			proxy->injectLockManager(m_LockManager);
			proxy->injectTransferOrderLockFactory(m_TransferOrderLockFactory);

			return proxy;
		}
	} // end namespace componentManager
} // end namespace libabbauw
