#include "transferorderfactory.h"

#include "itransferorderdmgetter.h"

#include "itransferorder.h"
#include "itransferorderhead.h"

#include "exceptions/ordernotfoundexception.h"
#include "libabbauw_properties_definitions.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include <libutil/misc/metaclassmacro_definitions.h>
#include <libbasarcmnutil_logging.h>

#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasardbaspect_accessorpropertytable.h>

namespace libabbauw
{
	namespace componentManager
	{
		TransferOrderFactory::TransferOrderFactory()
		{
		}

		void TransferOrderFactory::injectTransferOrderDMGetter(ITransferOrderDMGetterPtr transferOrderGetter)
		{
			m_TransferOrderDMGetter = transferOrderGetter;
		}

		domMod::transferOrder::ITransferOrderPtr TransferOrderFactory::create(basar::db::aspect::AccessorPropertyTable_YIterator yit)
		{
			METHODNAME_DEF(TransferOrderFactory, create);

			domMod::transferOrder::ITransferOrderPtr transferOrder = m_TransferOrderDMGetter->getTransferOrderDM();
			const basar::Int32 orderNo = yit.getInt32(properties::ORDERNO);

			transferOrder->findById(orderNo);
			if (transferOrder->isEmpty())
			{
				// multi user capability: somebody has deleted this order
				basar::VarString msg = tr("Order with order number %1 not found or doesn't have positions!").arg(orderNo).toLocal8Bit().constData();
				throw exceptions::OrderNotFoundException(basar::ExceptInfo(fun, msg, __FILE__, __LINE__));
			}

			return transferOrder;
		}

		domMod::transferOrder::ITransferOrderPtr TransferOrderFactory::createWithoutLock(basar::db::aspect::AccessorPropertyTable_YIterator yit)
		{
			METHODNAME_DEF(TransferOrderFactory, create);

			// used only for CPR0002688 to avoid double locking
			domMod::transferOrder::ITransferOrderPtr transferOrder = m_TransferOrderDMGetter->getTransferOrderDM();
			const basar::Int32 orderNo = yit.getInt32(properties::ORDERNO);

			transferOrder->findByIdWithoutLock(orderNo);
			if (transferOrder->isEmpty())
			{
				// multi user capability: somebody has deleted this order
				basar::VarString msg = tr("Order with order number %1 not found or doesn't have positions!").arg(orderNo).toLocal8Bit().constData();
				throw exceptions::OrderNotFoundException(basar::ExceptInfo(fun, msg, __FILE__, __LINE__));
			}

			return transferOrder;
		}


		domMod::transferOrder::ITransferOrderPtr TransferOrderFactory::createSplittedHead(domMod::transferOrder::ITransferOrderPtr rhs)
		{
			domMod::transferOrder::ITransferOrderPtr transferOrder = m_TransferOrderDMGetter->getTransferOrderDM();
			basar::db::aspect::AccessorPropertyTable_YIterator orderYit = transferOrder->getHead()->get();
			orderYit.match(rhs->getHead()->get());

			// new orderhead has no positions and no orderno yet
			orderYit.setPropertyState(properties::ORDERNO, basar::SS_UNSET);
			return transferOrder;
		}
	} // end namespace componentManager
} // end namespace libabbauw
