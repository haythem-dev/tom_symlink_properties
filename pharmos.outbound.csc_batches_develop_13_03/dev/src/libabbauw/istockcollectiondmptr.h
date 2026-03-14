#ifndef GUARD_LIBABBAUW_DOMMOD_STOCK_ISTOCKCOLLECTIONDMPTR_H
#define GUARD_LIBABBAUW_DOMMOD_STOCK_ISTOCKCOLLECTIONDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
	namespace domMod
	{
		namespace stock
		{
			class IStockCollectionDM;
			typedef boost::shared_ptr< IStockCollectionDM > IStockCollectionDMPtr;

		} // end namespace stock
	} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_DOMMOD_STOCK_ISTOCKCOLLECTIONDMPTR_H
