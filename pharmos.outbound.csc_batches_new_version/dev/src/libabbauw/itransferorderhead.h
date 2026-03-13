#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEAD_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEAD_H

#include "orderstatus.h"
#include <libbasar_definitions.h>

#include "iordergetter.h"

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
			class ITransferOrderHead : public cscorder::IOrderGetter
			{
			public:
				virtual ~ITransferOrderHead() {}

				virtual basar::db::aspect::AccessorPropertyTable_YIterator get() = 0;
				virtual basar::db::aspect::AccessorPropertyTable_YIterator get() const = 0;

				virtual void                                               findById(basar::Int32 orderNo) = 0;
				virtual void                                               markForDelete() = 0;
				virtual bool                                               save() = 0;

				virtual bool                                               isChanged() const = 0;
				virtual bool                                               isEmpty() const = 0;

				virtual basar::Decimal                                     getDiscountPct() const = 0;
				virtual basar::VarString                                   getDiscountPctRange() const = 0;

				virtual basar::Int32                                       getOrderNo() const = 0;
				virtual OrderStatus                                        getOrderStatus() const = 0;

				virtual void                                               clear() = 0;
			};

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERHEAD_H
