#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_ORDERPROTOCOLDMPTR_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_ORDERPROTOCOLDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace domMod
	{
		namespace orderProtocol
		{
			class OrderProtocolDM;
			typedef boost::shared_ptr<OrderProtocolDM> OrderProtocolDMPtr;

		} // end namespace orderProtocol
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_ORDERPROTOCOLDMPTR_H
