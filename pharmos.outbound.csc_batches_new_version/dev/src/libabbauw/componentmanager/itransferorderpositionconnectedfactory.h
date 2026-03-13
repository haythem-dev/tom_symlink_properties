#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERPOSITIONCONNECTEDFACTORY_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERPOSITIONCONNECTEDFACTORY_H

#include "itransferorderpositionptr.h"

//------------------------------------------------------------------------------//
// Forward declarations
//------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
		}
	}
}

namespace libabbauw
{
	namespace componentManager
	{
		class ITransferOrderPositionConnectedFactory
		{
		public:
			virtual ~ITransferOrderPositionConnectedFactory() {}

			virtual domMod::transferOrder::ITransferOrderPositionPtr  create(basar::db::aspect::AccessorPropertyTable_YIterator) const = 0;
		};

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERPOSITIONCONNECTEDFACTORY_H
