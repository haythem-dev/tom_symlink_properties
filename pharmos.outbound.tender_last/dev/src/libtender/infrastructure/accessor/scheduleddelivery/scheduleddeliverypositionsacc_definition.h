#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SCHEDULEDDELIVERYPOSITIONSACC_DEFINITIONS_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SCHEDULEDDELIVERYPOSITIONSACC_DEFINITIONS_H

#include <libbasarcmnutil_parameterlist.h>

namespace libtender
{
namespace infrastructure
{
namespace accessor
{
namespace scheduledDelivery
{
namespace lit
{
const basar::VarString SCHEDULEDDELIVERY_POSITIONS_ACC          ( "AccSCHEDULEDDELIVERYPOSITIONS" );
const basar::VarString SELECT_SCHEDULEDDELIVERY_POSITIONS_BY_KEY( "SelectScheduledDeliveryPositionsByKey" );
const basar::VarString SELECT_ALREADY_RESERVED_QUANTITY         ( "SelectAlreadyReservedQuantity" );
const basar::VarString SAVE_SCHEDULEDDELIVERY_POSITION          ( "SaveScheduledDeliveryPosition" );

} // end namespace lit
} // end namespace scheduledDelivery
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libtender

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_SCHEDULEDDELIVERYPOSITIONSACC_DEFINITIONS_H
