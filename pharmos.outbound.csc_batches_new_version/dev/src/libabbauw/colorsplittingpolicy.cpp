#include "colorsplittingpolicy.h"

#include "itransferorder.h"
#include "itransferorderhead.h"
#include "itransferorderposcollection.h"

#include "libabbauw_properties_definitions.h"
#include "loggerpool/libabbauw_loggerpool.h"
#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>

#include <libutil/misc/log_macro_definitions.h>

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{
namespace splittingPolicy
{
    ColorSplittingPolicy::ColorSplittingPolicy()
    {
    }

    ColorSplittingPolicy::~ColorSplittingPolicy()
    {
    }

    bool ColorSplittingPolicy::shouldPositionBeTransfered( basar::db::aspect::AccessorPropertyTable_YIterator yit )
	{
        const basar::Int16 color = yit.getInt16( properties::COLOR );

        // a position can only have one color -> no bitmask operation are needed here
        return color == RED || color == YELLOW;
    }

    bool ColorSplittingPolicy::isSplittable( transferOrder::ITransferOrderPtr transferOrderDMPtr )
    {
        bool hasAtLeastOnePositionWithColorGreenAMGE =       false;
        bool hasAtLeastOnePositionWithColorRedOrYellowAMGE = false;

        basar::db::aspect::AccessorPropertyTableRef propTab = transferOrderDMPtr->getPosCollection()->get();

        for( basar::db::aspect::AccessorPropertyTable_YIterator yitPos = propTab.begin(); yitPos != propTab.end(); ++yitPos )
	    {
            if( (yitPos.getInt16( libabbauw::properties::COLOR ) & GREEN) == GREEN && 
				(0 < yitPos.getInt32(properties::ORDEREDQTY) || 0 < yitPos.getInt32(properties::NONCHARGEDQTY))	)
            {
                hasAtLeastOnePositionWithColorGreenAMGE = true;
            }
            else 
            if( ((yitPos.getInt16( libabbauw::properties::COLOR ) & RED) == RED ||
                (yitPos.getInt16( libabbauw::properties::COLOR ) & YELLOW) == YELLOW) && 
                (0 < yitPos.getInt32(properties::ORDEREDQTY) || 0 < yitPos.getInt32(properties::NONCHARGEDQTY)) )
            {
                hasAtLeastOnePositionWithColorRedOrYellowAMGE = true;
            }
	    }

	    // if order contains both, green and yellow/red color then splitting this order into two orders is possible.
        bool orderSplitPossible = hasAtLeastOnePositionWithColorGreenAMGE && hasAtLeastOnePositionWithColorRedOrYellowAMGE;
        orderSplitPossible &= ( transferOrderDMPtr->getOrderStatus().getStatus() == OrderStatus::ZU );

	    return orderSplitPossible;
    }

} // end namespace splittingPolicy
} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw
