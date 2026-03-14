#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERHEADPROTOCOLPTR_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERHEADPROTOCOLPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace domMod
	{
		namespace orderProtocol
		{
			class IOrderHeadProtocol;
			typedef boost::shared_ptr<IOrderHeadProtocol> IOrderHeadProtocolPtr;

		} // end namespace orderProtocol
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERHEADPROTOCOLPTR_H
