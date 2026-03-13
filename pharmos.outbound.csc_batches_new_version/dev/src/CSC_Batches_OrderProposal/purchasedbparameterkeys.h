//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief      UserAdmin parameter-gateway key constants for purchase DB connection
 */
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_INFRASTRUCTURE_DB_PURCHASEDBPARAMETERKEYS_H
#define GUARD_ABBAUW_INFRASTRUCTURE_DB_PURCHASEDBPARAMETERKEYS_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasar_definitions.h>
#include <libbasarcmnutil_bstring.h>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace abbaUW {
namespace infrastructure {
namespace db {
namespace constants {

//-------------------------------------------------------------------------------------------------//

// Keys used to look up the purchase DB link parameters from the UserAdmin parameter gateway.
// The values must match the keys configured in the UserAdmin system.
static const basar::VarString UADM_PURCHASE_DB_KEY        ( "purchasedb"    );
static const basar::VarString UADM_PURCHASE_DB_SERVER_KEY ( "purchasedbsrv" );

//-------------------------------------------------------------------------------------------------//

} // namespace constants
} // namespace db
} // namespace infrastructure
} // namespace abbaUW

//-------------------------------------------------------------------------------------------------//

#endif // GUARD_ABBAUW_INFRASTRUCTURE_DB_PURCHASEDBPARAMETERKEYS_H
