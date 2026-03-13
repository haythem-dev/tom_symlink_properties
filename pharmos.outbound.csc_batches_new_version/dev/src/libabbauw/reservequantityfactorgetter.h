#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_RESERVEQUANTITYFACTORGETTER_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_RESERVEQUANTITYFACTORGETTER_H

#include "ireservequantityfactorgetter.h"

#include "ireserveqtyfactorparameterptr.h"
#include "reservequantityfactortype.h"

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{

    class ReserveQuantityFactorGetter : public IReserveQuantityFactorGetter
    {
    public:
        ReserveQuantityFactorGetter( ReserveQuantityFactorType );
        ~ReserveQuantityFactorGetter();

        void injectReserveQtyFactorParameter( parameter::IReserveQtyFactorParameterPtr );

        basar::Decimal getReserveQtyFactor( const basar::VarString& orderType, bool isOrderView );

    private:
        parameter::IReserveQtyFactorParameterPtr getReserveQtyFactorParameter();
		bool									 isBookingType3(const basar::VarString& orderType);

    private:
        ReserveQuantityFactorType                m_ReserveQtyFactorType;
        basar::Decimal                           m_CurrentReserveQtyFactor;

        parameter::IReserveQtyFactorParameterPtr m_ReserveQtyFactorParameter;
    };

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_RESERVEQUANTITYFACTORGETTER_H
