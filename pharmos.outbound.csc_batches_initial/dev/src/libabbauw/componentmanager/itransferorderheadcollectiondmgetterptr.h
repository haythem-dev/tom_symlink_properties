#ifndef GUARD_LIBABBAUW_ITRANSFERORDERHEADCOLLECTIONDMGETTERPTR_H
#define GUARD_LIBABBAUW_ITRANSFERORDERHEADCOLLECTIONDMGETTERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace componentManager
	{
		class ITransferOrderHeadCollectionDMGetter;
		typedef boost::shared_ptr< ITransferOrderHeadCollectionDMGetter > ITransferOrderHeadCollectionDMGetterPtr;

	} // end namespace componentManager
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_ITRANSFERORDERHEADCOLLECTIONDMGETTERPTR_H
