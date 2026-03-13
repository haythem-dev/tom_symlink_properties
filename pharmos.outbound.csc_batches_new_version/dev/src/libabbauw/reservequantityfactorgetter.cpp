#include "reservequantityfactorgetter.h"

#include "ireserveqtyfactorparameter.h"

#include "loggerpool/libabbauw_loggerpool.h"
#include <libutil/util.h>

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{
    ReserveQuantityFactorGetter::ReserveQuantityFactorGetter( ReserveQuantityFactorType factorType )
      :  m_ReserveQtyFactorType( factorType ), m_CurrentReserveQtyFactor( basar::Decimal( -1 ) )
    {
    }

    ReserveQuantityFactorGetter::~ReserveQuantityFactorGetter()
    {
    }

    void ReserveQuantityFactorGetter::injectReserveQtyFactorParameter( parameter::IReserveQtyFactorParameterPtr param )
    {
        m_ReserveQtyFactorParameter = param;
    }

    parameter::IReserveQtyFactorParameterPtr ReserveQuantityFactorGetter::getReserveQtyFactorParameter()
    {
        CHECK_INSTANCE_EXCEPTION( m_ReserveQtyFactorParameter.get() );
        return m_ReserveQtyFactorParameter;
    }

	bool ReserveQuantityFactorGetter::isBookingType3(const basar::VarString& orderType)
	{
		basar::VarString bookingType = orderType.substr(3,1);

		if("3" == bookingType)
		{
			return true;
		}

		return false;
	}

    basar::Decimal ReserveQuantityFactorGetter::getReserveQtyFactor( const basar::VarString& orderType, bool isOrderView )
    {
        basar::Decimal factor;

        if( isBookingType3(orderType) )
        {
			if(isOrderView && getReserveQtyFactorParameter()->isNotSalesEffectiveReserveQtyOrderFactorSet())
			{
				factor = getReserveQtyFactorParameter()->getNotSalesEffectiveReserveQtyOrderFactor();
			}
			else if(getReserveQtyFactorParameter()->isNotSalesEffectiveReserveQtyReleaseFactorSet())
			{
				factor = getReserveQtyFactorParameter()->getNotSalesEffectiveReserveQtyReleaseFactor();
			}
        }
        else
        {
            if( m_CurrentReserveQtyFactor == basar::Decimal( -1 ) ) // only reset if unset (getting parameter is rather expensive)
            {
                switch( m_ReserveQtyFactorType )
                {
                    case RELEASE_FACTOR:
                        factor = getReserveQtyFactorParameter()->getReserveQuantityReleaseFactor();
                        break;
                    case ORDER_FACTOR:
                        factor = getReserveQtyFactorParameter()->getReserveQuantityOrderFactor();
                        break;
                    // no default case to receive compiler warnings when enum is enhanced
                }

                basar::VarString msg = "Using factor '" + factor.toString() + "' for calculation on '" + orderType + "'";
                BLOG_DEBUG( LoggerPool::loggerTransferOrder, msg.c_str() );
                m_CurrentReserveQtyFactor = factor; // cache factor
            }
            else
            {
                factor = m_CurrentReserveQtyFactor;
            }
        }

        return factor;
    }

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw
