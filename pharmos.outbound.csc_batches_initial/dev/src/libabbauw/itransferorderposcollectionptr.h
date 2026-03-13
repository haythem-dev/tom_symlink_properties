#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERPOSCOLLECTIONPTR_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERPOSCOLLECTIONPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{
			class ITransferOrderPosCollection;
			typedef boost::shared_ptr< ITransferOrderPosCollection > ITransferOrderPosCollectionPtr;

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERPOSCOLLECTIONPTR_H
