#ifndef GUARD_LIBABBAUW_ITRANSFERORDERFACTORYPTR_H
#define GUARD_LIBABBAUW_ITRANSFERORDERFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		class ITransferOrderFactory;
		typedef boost::shared_ptr< ITransferOrderFactory > ITransferOrderFactoryPtr;

	} // end namespace componentManager
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_ITRANSFERORDERFACTORYPTR_H
