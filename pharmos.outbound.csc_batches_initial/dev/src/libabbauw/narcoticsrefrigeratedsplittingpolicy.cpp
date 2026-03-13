#include "narcoticsrefrigeratedsplittingpolicy.h"

#include "itransferorder.h"
#include "itransferorderhead.h"
#include "itransferorderposcollection.h"

#include "libabbauw_properties_definitions.h"
#include <libbasardbaspect_accessorpropertytable.h>
#include <libbasardbaspect_accessorpropertytable_yiterator.h>

namespace libabbauw
{
namespace domMod
{
namespace transferOrder
{
namespace splittingPolicy
{
    NarcoticsRefrigeratedSplittingPolicy::NarcoticsRefrigeratedSplittingPolicy()
    {
    }

    NarcoticsRefrigeratedSplittingPolicy::~NarcoticsRefrigeratedSplittingPolicy()
    {
    }

    bool NarcoticsRefrigeratedSplittingPolicy::shouldPositionBeTransfered( basar::db::aspect::AccessorPropertyTable_YIterator yit )
    {
        const basar::VarString notice = yit.getString( properties::NOTICE );
        const bool isNarcotics    = (notice == "N");
        const bool isRefrigerated = (notice == "K");

        return isNarcotics || isRefrigerated;
    }

    bool NarcoticsRefrigeratedSplittingPolicy::isSplittable( transferOrder::ITransferOrderPtr transferOrderDMPtr )
    {
        basar::db::aspect::AccessorPropertyTableRef propTab = transferOrderDMPtr->getPosCollection()->get();
        bool containsNarcoticsRefrigerated = false;
        bool containsNonNarcoticsRefrigerated = false;

        // if order contains positions with refrigerated/narcotics and "normal" positions then splitting it is possible
	    for( basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin(); yit != propTab.end(); ++yit )
	    {
            if( yit.getInt32( properties::ORDEREDQTY ) > 0 || yit.getInt32(properties::NONCHARGEDQTY) > 0 ) // only positions with QTY are taken into account!
            {
                if( yit.getString( libabbauw::properties::NOTICE ).empty() )
                {
                    containsNonNarcoticsRefrigerated = true;
                }
                else
                {
                    containsNarcoticsRefrigerated = true;
                }
            }
	    }

        const bool orderSplitPossible = ( transferOrderDMPtr->getOrderStatus().getStatus() == OrderStatus::ZU );
        return orderSplitPossible && containsNarcoticsRefrigerated && containsNonNarcoticsRefrigerated;
    }

} // end namespace splittingPolicy
} // end namespace transferOrder
} // end namespace domMod
} // end namespace libabbauw
