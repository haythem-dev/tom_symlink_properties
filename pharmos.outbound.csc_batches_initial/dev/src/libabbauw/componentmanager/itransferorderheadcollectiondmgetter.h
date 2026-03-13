#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADCOLLECTIONDMGETTER_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADCOLLECTIONDMGETTER_H

#include "itransferorderheadcollectiondmptr.h"

namespace libabbauw
{
	namespace componentManager
	{
		class ITransferOrderHeadCollectionDMGetter
		{
		public:
			virtual ~ITransferOrderHeadCollectionDMGetter() {}

			virtual domMod::transferOrder::ITransferOrderHeadCollectionDMPtr getTransferOrderHeadCollectionDM() = 0;
		};

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADCOLLECTIONDMGETTER_H
