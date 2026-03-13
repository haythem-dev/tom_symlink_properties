#ifndef GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_SPLITTINGPOLICY_COLORSPLITTINGPOLICY_H
#define GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_SPLITTINGPOLICY_COLORSPLITTINGPOLICY_H

#include "isplittingpolicy.h"

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{
namespace splittingPolicy
{

    class ColorSplittingPolicy : public ISplittingPolicy
    {
    public:
        ColorSplittingPolicy();
        ~ColorSplittingPolicy();

        // from ISplittingPolicy interface
        bool shouldPositionBeTransfered( basar::db::aspect::AccessorPropertyTable_YIterator );
        bool isSplittable( transferOrder::ITransferOrderPtr );
        
    private:
        ColorSplittingPolicy ( const ColorSplittingPolicy& );
	    ColorSplittingPolicy operator = ( const ColorSplittingPolicy& );
    };

} // end namespace splittingPolicy
} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw


#endif // GUARD_LIBABBAUW_DOMMOD_TRANSFERORDER_SPLITTINGPOLICY_COLORSPLITTINGPOLICY_H
