#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERSPLITDM_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERSPLITDM_H

#include "itransferorderptr.h"

namespace libabbauw
{
namespace domMod
{

namespace transferOrder
{

    class ITransferOrderSplitDM
    {
    public:
        virtual ~ITransferOrderSplitDM() {}

        virtual transferOrder::ITransferOrderPtr               split( transferOrder::ITransferOrderPtr )        = 0;
        virtual bool                                           isSplittable( transferOrder::ITransferOrderPtr ) = 0;
    };

} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_ITRANSFERORDERSPLITDM_H
