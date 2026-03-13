#ifndef GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERPOSCOLLECTIONPROTOCOLPTR_H
#define GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERPOSCOLLECTIONPROTOCOLPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace domMod
	{
		namespace orderProtocol
		{
			class IOrderPosCollectionProtocol;
			typedef boost::shared_ptr<IOrderPosCollectionProtocol> IOrderPosCollectionProtocolPtr;

		} // end namespace orderProtocol
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_ORDERPROTOCOL_IORDERPOSCOLLECTIONPROTOCOLPTR_H
