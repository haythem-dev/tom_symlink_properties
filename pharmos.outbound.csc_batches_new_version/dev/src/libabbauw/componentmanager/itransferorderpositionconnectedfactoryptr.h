#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERPOSITIONCONNECTEDFACTORYPTR_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERPOSITIONCONNECTEDFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		class ITransferOrderPositionConnectedFactory;
		typedef boost::shared_ptr< ITransferOrderPositionConnectedFactory > ITransferOrderPositionConnectedFactoryPtr;

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_ITRANSFERORDERPOSITIONCONNECTEDFACTORYPTR_H
