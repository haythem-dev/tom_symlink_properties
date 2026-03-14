#ifndef GUARD_LIBABBAUW_ITRANSFERORDERHEADFACTORYPTR_H
#define GUARD_LIBABBAUW_ITRANSFERORDERHEADFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		class ITransferOrderHeadFactory;
		typedef boost::shared_ptr< ITransferOrderHeadFactory > ITransferOrderHeadFactoryPtr;

	} // end namespace componentManager
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_ITRANSFERORDERHEADFACTORYPTR_H
