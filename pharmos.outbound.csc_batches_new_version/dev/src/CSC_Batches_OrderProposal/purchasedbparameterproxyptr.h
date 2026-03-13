//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief      Shared-pointer typedef for PurchaseDBParameterProxy (concrete type)
 */
//-------------------------------------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_INFRASTRUCTURE_DB_PURCHASEDBPARAMETERPROXYPTR_H
#define GUARD_ABBAUW_INFRASTRUCTURE_DB_PURCHASEDBPARAMETERPROXYPTR_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "purchasedbparameterproxy.h"
#include <boost/shared_ptr.hpp>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace abbaUW {
namespace infrastructure {
namespace db {

//-------------------------------------------------------------------------------------------------//

typedef boost::shared_ptr<PurchaseDBParameterProxy> PurchaseDBParameterProxyPtr;

//-------------------------------------------------------------------------------------------------//

} // namespace db
} // namespace infrastructure
} // namespace abbaUW

//-------------------------------------------------------------------------------------------------//

#endif // GUARD_ABBAUW_INFRASTRUCTURE_DB_PURCHASEDBPARAMETERPROXYPTR_H
