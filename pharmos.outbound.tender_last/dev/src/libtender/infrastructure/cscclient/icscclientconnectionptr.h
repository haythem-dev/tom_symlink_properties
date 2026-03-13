#ifndef GUARD_INFRASTRUCTURE_ICSCCLIENTCONNECTIONPTR_H
#define GUARD_INFRASTRUCTURE_ICSCCLIENTCONNECTIONPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender {
namespace infrastructure {
namespace cscclient {

	class ICscClientConnection;

	typedef boost::shared_ptr<ICscClientConnection> ICscClientConnectionPtr;

} // namespace cscclient
} // namespace infrastructure
} // namespace libtender

#endif // GUARD_INFRASTRUCTURE_ICSCCLIENTCONNECTIONPTR_H
