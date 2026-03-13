#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALCOLLECTIONPROTECTIONPROXYGETTER_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALCOLLECTIONPROTECTIONPROXYGETTER_H

#include "domainmodule/orderproposal/iorderproposalcollectionprotectionproxyptr.h"

namespace libabbauw
{
	namespace componentManager
	{
		class IOrderProposalCollectionProtectionProxyGetter
		{
		public:
			virtual ~IOrderProposalCollectionProtectionProxyGetter() {}

			virtual domMod::orderProposal::IOrderProposalCollectionProtectionProxyPtr getOrderProposalCollectionProtectionProxy() = 0;
		};

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALCOLLECTIONPROTECTIONPROXYGETTER_H
