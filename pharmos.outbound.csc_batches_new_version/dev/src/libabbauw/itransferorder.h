#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDER_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDER_H

#include "itransferordergetter.h"
#include "iordertypegetter.h"
#include "itransferorderheadptr.h"
#include "itransferorderposcollectionptr.h"
#include "orderstatus.h"

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
			class ITransferOrder : public ITransferOrderGetter, public IOrderTypeGetter
			{
			public:
				virtual ~ITransferOrder() {}

				virtual void                                                        findById(basar::Int32 orderNo) = 0;

				// used only for CPR0002688 to avoid double locking
				virtual void                                                        findByIdWithoutLock(basar::Int32 orderNo) = 0;

				virtual bool                                                        isChanged() const = 0;

				virtual void                                                        save() = 0;
				virtual void                                                        clear() = 0;

				virtual basar::db::aspect::AccessorPropertyTable_YIterator          addEmptyPosition() = 0;
				virtual void                                                        removePosition(basar::db::aspect::AccessorPropertyTable_YIterator) = 0;

				virtual void                                                        release(bool postpone) = 0;
			};

		} // end namespace transferOrder
	} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDER_H
