#ifndef GUARD_LIBABBAUW_PARAMETER_IRESERVEQTYFACTORPARAMETER_H
#define GUARD_LIBABBAUW_PARAMETER_IRESERVEQTYFACTORPARAMETER_H

#include <libbasarcmnutil_decimal.h>

namespace libabbauw
{
namespace domMod
{
namespace parameter
{

class IReserveQtyFactorParameter
{
public:	
    virtual ~IReserveQtyFactorParameter() {}

    virtual basar::Decimal                                      getReserveQuantityReleaseFactor()        = 0;
    virtual basar::Decimal                                      getReserveQuantityOrderFactor()          = 0;

    virtual basar::Decimal                                      getNotSalesEffectiveReserveQtyReleaseFactor()   = 0;
    virtual basar::Decimal                                      getNotSalesEffectiveReserveQtyOrderFactor()   = 0;

	virtual bool                                                isNotSalesEffectiveReserveCheckSet() = 0;
	virtual bool                                                isNotSalesEffectiveReserveQtyReleaseFactorSet() = 0;
	virtual bool                                                isNotSalesEffectiveReserveQtyOrderFactorSet() = 0;
};

} // end namespace parameter
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_PARAMETER_IRESERVEQTYFACTORPARAMETER_H
