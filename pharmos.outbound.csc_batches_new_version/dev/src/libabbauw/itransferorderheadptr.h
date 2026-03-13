#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADPTR_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{
			class ITransferOrderHead;
			typedef boost::shared_ptr< ITransferOrderHead > ITransferOrderHeadPtr;

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif //GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADPTR_H
