#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERPOSCOLLECTIONGETTER_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERPOSCOLLECTIONGETTER_H

#include "itransferorderposcollectionptr.h"
#include "iordertypegetterptr.h"

namespace libabbauw
{
	namespace componentManager
	{
		class ITransferOrderPosCollectionGetter
		{
		public:
			virtual ~ITransferOrderPosCollectionGetter() {}

			virtual domMod::transferOrder::ITransferOrderPosCollectionPtr getTransferOrderPosCollectionDM(domMod::transferOrder::IOrderTypeGetterPtr) = 0;
		};

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERPOSCOLLECTIONGETTER_H
