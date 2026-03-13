#include "transferorderheadcollectionlockeddmgetter.h"

#include "transferorderheadcollectionprotectionproxy.h"
#include "transferorderheadcollectionprotectionproxyptr.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include <libutil/misc/metaclassmacro_definitions.h>
#include <libbasarcmnutil_logging.h>

#include <lockmanagerptr.h>

#include <boost/make_shared.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		TransferOrderHeadCollectionLockedDMGetter::TransferOrderHeadCollectionLockedDMGetter()
		{
		}

		TransferOrderHeadCollectionLockedDMGetter::~TransferOrderHeadCollectionLockedDMGetter()
		{
		}

		void TransferOrderHeadCollectionLockedDMGetter::injectLockManager(libLockManager::ILockManagerPtr lockManager)
		{
			m_LockManager = lockManager;
		}

		void TransferOrderHeadCollectionLockedDMGetter::injectTransferOrderHeadCollectionDMGetter(ITransferOrderHeadCollectionDMGetterPtr transferOrderHeadCollDMGetter)
		{
			m_TransferOrderHeadCollectionDMGetter = transferOrderHeadCollDMGetter;
		}

		void TransferOrderHeadCollectionLockedDMGetter::injectTransferOrderLockFactory(lock::IOrderLockFactoryPtr transferOrderLockFactory)
		{
			m_TransferOrderLockFactory = transferOrderLockFactory;
		}

		domMod::transferOrder::ITransferOrderHeadCollectionDMPtr TransferOrderHeadCollectionLockedDMGetter::getTransferOrderHeadCollectionDM()
		{
			METHODNAME_DEF(TransferOrderHeadCollectionLockedDMGetter, getTransferOrderHeadCollectionDM);
			BLOG_TRACE_METHOD(LoggerPool::loggerLibAbbaUW, fun);

			using namespace domMod::transferOrder;
			TransferOrderHeadCollectionProtectionProxyPtr proxy = boost::make_shared< TransferOrderHeadCollectionProtectionProxy >();
			proxy->injectTransferOrderHeadCollection(m_TransferOrderHeadCollectionDMGetter->getTransferOrderHeadCollectionDM());
			proxy->injectLockManager(m_LockManager);
			proxy->injectTransferOrderLockFactory(m_TransferOrderLockFactory);

			return proxy;
		}
	} // end namespace componentManager
} // end namespace libabbauw
