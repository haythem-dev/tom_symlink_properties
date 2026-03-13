#ifndef GUARD_INFRASTRUCTURE_CSCCLIENTCONNECTIONPTR_H
#define GUARD_INFRASTRUCTURE_CSCCLIENTCONNECTIONPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender {
namespace infrastructure {
namespace cscclient {

	class CscClientConnection;

	typedef boost::shared_ptr<CscClientConnection> CscClientConnectionPtr;

} // namespace cscclient
} // namespace infrastructure
} // namespace libtender

#endif // GUARD_INFRASTRUCTURE_CSCCLIENTCONNECTIONPTR_H
