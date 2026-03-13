#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_IRESERVEQUANTITYFACTORGETTER_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_IRESERVEQUANTITYFACTORGETTER_H

#include <libbasarcmnutil_decimal.h>
#include <libbasarcmnutil_bstring.h>

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

    class IReserveQuantityFactorGetter
    {
    public:
        virtual ~IReserveQuantityFactorGetter() {}

        virtual basar::Decimal getReserveQtyFactor( const basar::VarString& orderType, bool isOrderView ) = 0;
    };

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_IRESERVEQUANTITYFACTORGETTER_H
