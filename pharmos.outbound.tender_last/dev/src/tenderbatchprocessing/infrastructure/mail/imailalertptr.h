#ifndef GUARD_INFRASTRUCTURE_IMAILALERTPTR_H
#define GUARD_INFRASTRUCTURE_IMAILALERTPTR_H

#include <boost/shared_ptr.hpp>

namespace tenderbatchprocessing {
namespace infrastructure {
namespace mail {

	class IMailAlert;

	typedef boost::shared_ptr<IMailAlert> IMailAlertPtr;

} // namespace mail
} // namespace infrastructure
} // namespace tenderbatchprocessing

#endif // GUARD_INFRASTRUCTURE_IMAILALERTPTR_H
