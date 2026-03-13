#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERFACTORY_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERFACTORY_H

#include "itransferorderptr.h"

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
		class ITransferOrderFactory
		{
		public:
			virtual ~ITransferOrderFactory() {}

			virtual domMod::transferOrder::ITransferOrderPtr create(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;

			// used only for CPR0002688 to avoid double locking
			virtual domMod::transferOrder::ITransferOrderPtr createWithoutLock(basar::db::aspect::AccessorPropertyTable_YIterator) = 0; 
		};

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERFACTORY_H
