#ifndef GUARD_IPURCHASEDBPARAMETERPROXYPTR_H
#define GUARD_IPURCHASEDBPARAMETERPROXYPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender {
namespace infrastructure {
namespace db {

	class IPurchaseDBParameterProxy;

	typedef boost::shared_ptr<IPurchaseDBParameterProxy> IPurchaseDBParameterProxyPtr;

} // namespace db 
} // namespace infrastructure
} // namespace libtender

#endif // GUARD_IPURCHASEDBPARAMETERPROXYPTR_H
