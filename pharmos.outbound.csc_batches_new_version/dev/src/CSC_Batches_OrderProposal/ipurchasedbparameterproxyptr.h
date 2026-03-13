//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief      Shared-pointer typedef for IPurchaseDBParameterProxy
 */
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_INFRASTRUCTURE_DB_IPURCHASEDBPARAMETERPROXYPTR_H
#define GUARD_ABBAUW_INFRASTRUCTURE_DB_IPURCHASEDBPARAMETERPROXYPTR_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "ipurchasedbparameterproxy.h"
#include <boost/shared_ptr.hpp>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace abbaUW {
namespace infrastructure {
namespace db {

//-------------------------------------------------------------------------------------------------//

typedef boost::shared_ptr<IPurchaseDBParameterProxy> IPurchaseDBParameterProxyPtr;

//-------------------------------------------------------------------------------------------------//

} // namespace db
} // namespace infrastructure
} // namespace abbaUW

//-------------------------------------------------------------------------------------------------//

#endif // GUARD_ABBAUW_INFRASTRUCTURE_DB_IPURCHASEDBPARAMETERPROXYPTR_H
