#ifndef GUARD_IPURCHASEDBPARAMETERPROXY_H
#define GUARD_IPURCHASEDBPARAMETERPROXY_H

//-------------------------------------------------------------------------------------------------//

#include <libbasar_definitions.h>
#include <libbasarcmnutil_bstring.h>

//-------------------------------------------------------------------------------------------------//

namespace libtender {
namespace infrastructure {
namespace db {

//-------------------------------------------------------------------------------------------------//

class IPurchaseDBParameterProxy
{
public:
	virtual ~IPurchaseDBParameterProxy() {};

	virtual const basar::VarString&			getPurchaseDBServer() const = 0;
	virtual const basar::VarString&			getPurchaseDB() const = 0;
};

//-------------------------------------------------------------------------------------------------//

} // namespace db
} // namespace infrastructure
} // namespace libtender

//-------------------------------------------------------------------------------------------------//

#endif // GUARD_IPURCHASEDBPARAMETERPROXY_H

//-------------------------------------------------------------------------------------------------//
