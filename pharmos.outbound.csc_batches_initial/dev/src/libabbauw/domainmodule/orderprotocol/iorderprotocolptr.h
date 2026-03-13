#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERPROTOCOLPTR_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERPROTOCOLPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace domMod
	{
		namespace orderProtocol
		{
			class IOrderProtocol;
			typedef boost::shared_ptr< IOrderProtocol > IOrderProtocolPtr;

		} // end namespace orderProtocol
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERPROTOCOLPTR_H
