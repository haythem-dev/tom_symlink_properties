#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_SPLITTINGPOLICY_NARCOTICSREFRIGERATEDSPLITTINGPOLICY_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_SPLITTINGPOLICY_NARCOTICSREFRIGERATEDSPLITTINGPOLICY_H

#include "isplittingpolicy.h"

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{
namespace splittingPolicy
{

    class NarcoticsRefrigeratedSplittingPolicy : public ISplittingPolicy
    {
    public:
        NarcoticsRefrigeratedSplittingPolicy();
        ~NarcoticsRefrigeratedSplittingPolicy();

        // from ISplittingPolicy interface
        bool shouldPositionBeTransfered( basar::db::aspect::AccessorPropertyTable_YIterator );
        bool isSplittable( transferOrder::ITransferOrderPtr );

    private:
        NarcoticsRefrigeratedSplittingPolicy ( const NarcoticsRefrigeratedSplittingPolicy& );
	    NarcoticsRefrigeratedSplittingPolicy operator = ( const NarcoticsRefrigeratedSplittingPolicy& );
    };

} // end namespace splittingPolicy
} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw


#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_SPLITTINGPOLICY_NARCOTICSREFRIGERATEDSPLITTINGPOLICY_H
