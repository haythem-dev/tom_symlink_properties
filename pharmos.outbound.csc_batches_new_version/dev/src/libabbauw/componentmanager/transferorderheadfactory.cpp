#include "transferorderheadfactory.h"

#include "itransferorderheadgetter.h"

#include "transferorderheadptr.h"
#include "transferorderhead.h"

#include "yiteratormatcher.h"

#include <libbasardbaspect_accessorpropertytable_yiterator.h>
#include <libbasardbaspect_accessorpropertytable.h>

#include "loggerpool/libabbauw_loggerpool.h"
#include <libutil/misc/metaclassmacro_definitions.h>
#include <libutil/misc/log_macro_definitions.h>

namespace libabbauw
{
	namespace componentManager
	{
		TransferOrderHeadFactory::TransferOrderHeadFactory()
		{
		}

		void TransferOrderHeadFactory::injectTransferOrderHeadGetter(ITransferOrderHeadGetterPtr transferOrderHeadGetter)
		{
			m_TransferOrderHeadGetter = transferOrderHeadGetter;
		}

		domMod::transferOrder::ITransferOrderHeadPtr TransferOrderHeadFactory::create(basar::db::aspect::AccessorPropertyTable_YIterator yit)
		{
			METHODNAME_DEF(TransferOrderHeadFactory, create);

			// create new yit in state "FOR_CLEAN"
			domMod::transferOrder::ITransferOrderHeadPtr transferOrderHead = m_TransferOrderHeadGetter->getTransferOrderHead();
			basar::db::aspect::AccessorPropertyTableRef propTab = transferOrderHead->get().getPropertyTable();
			propTab.clear();
			basar::db::aspect::AccessorPropertyTable_YIterator yitHead = propTab.insert(basar::FOR_CLEAN);

			// match yit to yitHead with keeping original property states (yit.match() doesn't do that!)
			util::YIteratorMatcher(yit).matchTo(yitHead);

			return transferOrderHead;
		}
	} // end namespace componentManager
} // end namespace libabbauw
