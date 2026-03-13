#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADCOLLECTIONFACTORYPTR_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADCOLLECTIONFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		class ITransferOrderHeadCollectionFactory;
		typedef boost::shared_ptr< ITransferOrderHeadCollectionFactory > ITransferOrderHeadCollectionFactoryPtr;

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERHEADCOLLECTIONFACTORYPTR_H
