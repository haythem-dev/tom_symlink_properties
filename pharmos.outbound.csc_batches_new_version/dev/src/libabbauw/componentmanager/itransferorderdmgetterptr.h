#ifndef GUARD_LIBABBAUW_ITRANSFERORDERGETTERPTR_H
#define GUARD_LIBABBAUW_ITRANSFERORDERGETTERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		class ITransferOrderDMGetter;
		typedef boost::shared_ptr< ITransferOrderDMGetter > ITransferOrderDMGetterPtr;

	} // end namespace componentManager
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_ITRANSFERORDERGETTERPTR_H
