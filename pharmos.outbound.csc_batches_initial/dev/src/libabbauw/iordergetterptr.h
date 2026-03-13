#ifndef GUARD_LIBABBAUW_DOMMOD_CSCORDER_IORDERGETTERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_CSCORDER_IORDERGETTERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace domMod
	{
		namespace cscorder
		{
			class IOrderGetter;
			typedef boost::shared_ptr< IOrderGetter > IOrderGetterPtr;

		} // end namespace cscorder
	} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_DOMMOD_CSCORDER_IORDERGETTERPTR_H 
