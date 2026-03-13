#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERFACTORYPTR_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		class TransferOrderFactory;
		typedef boost::shared_ptr< TransferOrderFactory > TransferOrderFactoryPtr;

	} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_TRANSFERORDERFACTORYPTR_H
