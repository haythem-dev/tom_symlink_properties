//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief      Interface for accessing purchase DB / DB-server name parameters
 *              (used for remote dblink qualification in pubatchorderpos SQL)
 *  \author
 *  \date
 */
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_INFRASTRUCTURE_DB_IPURCHASEDBPARAMETERPROXY_H
#define GUARD_ABBAUW_INFRASTRUCTURE_DB_IPURCHASEDBPARAMETERPROXY_H

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

//-------------------------------------------------------------------------------------------------//
// interface declaration
//-------------------------------------------------------------------------------------------------//
class IPurchaseDBParameterProxy
{
public:
    virtual ~IPurchaseDBParameterProxy() {}

    virtual const basar::VarString& getPurchaseDB()       const = 0;
    virtual const basar::VarString& getPurchaseDBServer() const = 0;
};

//-------------------------------------------------------------------------------------------------//

} // namespace db
} // namespace infrastructure
} // namespace abbaUW

//-------------------------------------------------------------------------------------------------//

#endif // GUARD_ABBAUW_INFRASTRUCTURE_DB_IPURCHASEDBPARAMETERPROXY_H
