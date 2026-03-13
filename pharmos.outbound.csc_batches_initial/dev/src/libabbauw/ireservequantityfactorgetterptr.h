#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_IRESERVEQUANTITYFACTORGETTERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_IRESERVEQUANTITYFACTORGETTERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod 
{
namespace transferOrder
{
    class IReserveQuantityFactorGetter;
    typedef boost::shared_ptr< IReserveQuantityFactorGetter > IReserveQuantityFactorGetterPtr;
	
} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_IRESERVEQUANTITYFACTORGETTERPTR_H
