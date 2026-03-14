#ifndef GUARD_LIBABBAUW_ITRANSFERORDERHEADGETTERPTR_H
#define GUARD_LIBABBAUW_ITRANSFERORDERHEADGETTERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		class ITransferOrderHeadGetter;
		typedef boost::shared_ptr< ITransferOrderHeadGetter > ITransferOrderHeadGetterPtr;

	} // end namespace componentManager
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_ITRANSFERORDERHEADGETTERPTR_H
