#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ORDERPROPOSALCOLLECTIONFACTORYPTR_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ORDERPROPOSALCOLLECTIONFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		class OrderProposalCollectionFactory;
		typedef boost::shared_ptr< OrderProposalCollectionFactory > OrderProposalCollectionFactoryPtr;

	} // end namespace componentManager
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_COMPONENTMANAGER_ORDERPROPOSALCOLLECTIONFACTORYPTR_H
