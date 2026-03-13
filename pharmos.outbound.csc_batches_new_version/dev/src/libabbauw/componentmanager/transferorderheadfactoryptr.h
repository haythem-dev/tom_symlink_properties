#ifndef GUARD_LIBABBAUW_TRANSFERORDERHEADFACTORYPTR_H
#define GUARD_LIBABBAUW_TRANSFERORDERHEADFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		class TransferOrderHeadFactory;
		typedef boost::shared_ptr< TransferOrderHeadFactory > TransferOrderHeadFactoryPtr;

	} // end namespace componentManager
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_TRANSFERORDERHEADFACTORYPTR_H
