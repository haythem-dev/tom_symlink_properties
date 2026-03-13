#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERGETTER_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERGETTER_H

#include "itransferorderheadptr.h"
#include "itransferorderposcollectionptr.h"
#include "orderstatus.h"

#include "trafficlightcolorsenum.h"
#include <libbasar_definitions.h>

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

namespace libabbauw
{
	namespace domMod
	{
		namespace transferOrder
		{
			class ITransferOrderGetter
			{
			public:
				virtual ~ITransferOrderGetter() {}

				virtual bool                                                        isEmpty()          const = 0;

				virtual ITransferOrderHeadPtr                                       getHead() = 0;
				virtual ITransferOrderHeadPtr                                       getHead()          const = 0;
				virtual ITransferOrderPosCollectionPtr                              getPosCollection() = 0;
				virtual ITransferOrderPosCollectionPtr                              getPosCollection() const = 0;

				virtual basar::Decimal                                              getOrderValue()    const = 0;
				virtual basar::Int32                                                getPositionCount() const = 0;

				virtual OrderStatus                                                 getOrderStatus()   const = 0;
				virtual basar::Int32                                                getOrderNo()       const = 0;
				virtual TrafficLightEnum                                            getColor()         const = 0;
			};

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERGETTER_H
