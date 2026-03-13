#include "transferorderdmgetter.h"

#include "transferorderdm.h"
#include "transferorderdmptr.h"
#include "transferorderposcollection.h"
#include "transferorderposcollectionptr.h"

#include "itransferorderheadgetter.h"
#include "itransferorderposcollectiongetter.h"
#include "iordergetter.h"
#include "icscordergetter.h"
#include "itransferorderhead.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include <libutil/misc/metaclassmacro_definitions.h>
#include <libbasarcmnutil_logging.h>

#include <boost/make_shared.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		TransferOrderDMGetter::TransferOrderDMGetter()
		{
		}

		TransferOrderDMGetter::~TransferOrderDMGetter()
		{
		}

		void TransferOrderDMGetter::injectConnection(basar::db::aspect::ConnectionRef connection)
		{
			m_Connection = connection;
		}

		void TransferOrderDMGetter::injectTransactionFactory(libutil::infrastructure::db::dbTransaction::IDBTransactionFactoryPtr transactionFactory)
		{
			m_TransactionFactory = transactionFactory;
		}

		void TransferOrderDMGetter::injectTransferOrderHeadGetter(ITransferOrderHeadGetterPtr headGetter)
		{
			m_TransferOrderHeadGetter = headGetter;
		}

		void TransferOrderDMGetter::injectTransferOrderPosCollectionGetter(ITransferOrderPosCollectionGetterPtr posCollectionGetter)
		{
			m_TransferOrderPosCollectionGetter = posCollectionGetter;
		}

		void TransferOrderDMGetter::injectCscOrderGetter(ICSCOrderGetterPtr cscOrderGetter)
		{
			m_CscOrderGetter = cscOrderGetter;
		}

		void TransferOrderDMGetter::injectOrderReleaseParameter(domMod::parameter::IOrderReleaseParameterPtr parameter)
		{
			m_OrderReleaseParameterPtr = parameter;
		}

		domMod::transferOrder::ITransferOrderPtr TransferOrderDMGetter::getTransferOrderDM()
		{
			METHODNAME_DEF(TransferOrderDMGetter, getTransferOrderDM);
			BLOG_TRACE_METHOD(LoggerPool::loggerLibAbbaUW, fun);

			using domMod::transferOrder::TransferOrderDMPtr;
			TransferOrderDMPtr transferOrder = boost::make_shared< domMod::transferOrder::TransferOrderDM >();
			domMod::transferOrder::ITransferOrderHeadPtr transferOrderHead = m_TransferOrderHeadGetter->getTransferOrderHead();

			transferOrder->injectTransactionFactory(m_TransactionFactory);
			transferOrder->injectHead(transferOrderHead);
			transferOrder->injectPosCollection(m_TransferOrderPosCollectionGetter->getTransferOrderPosCollectionDM(transferOrder));
			transferOrder->injectCscOrderReleaser(m_CscOrderGetter->getCSCOrderDM(transferOrderHead));
			transferOrder->injectOrderReleaseParameter(m_OrderReleaseParameterPtr);

			return transferOrder;
		}
	} // end namespace componentManager
} // end namespace libabbauw
