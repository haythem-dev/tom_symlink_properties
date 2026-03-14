#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONCHANGERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONCHANGERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{
			class ITransferOrderHeadCollectionChanger;
			typedef boost::shared_ptr< ITransferOrderHeadCollectionChanger > ITransferOrderHeadCollectionChangerPtr;

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEADCOLLECTIONCHANGERPTR_H
