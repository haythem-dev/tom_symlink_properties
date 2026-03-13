#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_IRESERVEQUANTITYFACTORGETTERFACTORYPTR_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_IRESERVEQUANTITYFACTORGETTERFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace componentManager 
{
    class IReserveQuantityFactorGetterFactory;
    typedef boost::shared_ptr< IReserveQuantityFactorGetterFactory > IReserveQuantityFactorGetterFactoryPtr;
	
} // end namespace componentManager
} // end namespace libabbauw

#endif // end GUARD_LIBABBAUW_COMPONENTMANAGER_IRESERVEQUANTITYFACTORGETTERFACTORYPTR_H
