#ifndef GUARD_LIBABBAUW_DOMMOD_STOCK_STOCKCOLLECTIONDMPTR_H
#define GUARD_LIBABBAUW_DOMMOD_STOCK_STOCKCOLLECTIONDMPTR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------------------------------------------//
// forward declaration section
//--------------------------------------------------------------------------------------------------//

namespace libabbauw
{
namespace domMod
{
namespace stock
{
	class StockCollectionDM;
    typedef boost::shared_ptr< StockCollectionDM > StockCollectionDMPtr;
	
} // end namespace stock
} // end namespace domMod
} // end namespace libabbauw

#endif //GUARD_LIBABBAUW_DOMMOD_STOCK_STOCKCOLLECTIONDMPTR_H
