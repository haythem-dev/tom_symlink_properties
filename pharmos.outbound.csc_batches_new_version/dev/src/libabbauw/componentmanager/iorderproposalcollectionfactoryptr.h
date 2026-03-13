#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALCOLLECTIONFACTORYPTR_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALCOLLECTIONFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		class IOrderProposalCollectionFactory;
		typedef boost::shared_ptr< IOrderProposalCollectionFactory > IOrderProposalCollectionFactoryPtr;

	} // end namespace componentManager
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALCOLLECTIONFACTORYPTR_H
