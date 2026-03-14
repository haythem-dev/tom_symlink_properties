#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_IRESERVEQUANTITYFACTORGETTERFACTORY_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_IRESERVEQUANTITYFACTORGETTERFACTORY_H

#include "ireservequantityfactorgetterptr.h"
#include "reservequantityfactortype.h"

namespace libabbauw
{
namespace componentManager
{
class IReserveQuantityFactorGetterFactory
{
    public:
        virtual ~IReserveQuantityFactorGetterFactory() {}

        virtual libabbauw::domMod::transferOrder::IReserveQuantityFactorGetterPtr getReserveQuantityFactorGetter( domMod::transferOrder::ReserveQuantityFactorType ) = 0;
};

} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_IRESERVEQUANTITYFACTORGETTERFACTORY_H
