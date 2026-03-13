#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERGETTER_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERGETTER_H

#include "itransferorderptr.h"

namespace libabbauw
{
	namespace componentManager
	{
		class ITransferOrderDMGetter
		{
		public:
			virtual ~ITransferOrderDMGetter() {}

			virtual domMod::transferOrder::ITransferOrderPtr                          getTransferOrderDM() = 0;
		};

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERGETTER_H
