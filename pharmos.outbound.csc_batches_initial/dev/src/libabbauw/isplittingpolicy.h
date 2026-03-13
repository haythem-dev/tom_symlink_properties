#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_SPLITTINGPOLICY_ISPLITTINGPOLICY_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_SPLITTINGPOLICY_ISPLITTINGPOLICY_H

#include "itransferorderptr.h"

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
namespace splittingPolicy
{
    class ISplittingPolicy
    {
    public:
        virtual ~ISplittingPolicy() {}

        virtual bool shouldPositionBeTransfered( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
        virtual bool isSplittable( transferOrder::ITransferOrderPtr ) = 0;
    };

} // end namespace splittingPolicy
} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw


#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_SPLITTINGPOLICY_ISPLITTINGPOLICY_H
