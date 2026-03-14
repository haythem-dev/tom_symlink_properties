#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADFACTORY_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADFACTORY_H

#include "itransferorderheadptr.h"

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

namespace libabbauw
{
	namespace componentManager
	{
		class ITransferOrderHeadFactory
		{
		public:
			virtual ~ITransferOrderHeadFactory() {}

			virtual domMod::transferOrder::ITransferOrderHeadPtr create(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;
		};

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADFACTORY_H
