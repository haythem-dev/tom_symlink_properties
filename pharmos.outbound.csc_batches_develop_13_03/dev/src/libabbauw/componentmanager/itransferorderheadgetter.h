#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADGETTER_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADGETTER_H

#include "itransferorderheadptr.h"

namespace libabbauw
{
	namespace componentManager
	{
		class ITransferOrderHeadGetter
		{
		public:
			virtual ~ITransferOrderHeadGetter() {}

			virtual domMod::transferOrder::ITransferOrderHeadPtr                       getTransferOrderHead() = 0;
		};

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADGETTER_H
