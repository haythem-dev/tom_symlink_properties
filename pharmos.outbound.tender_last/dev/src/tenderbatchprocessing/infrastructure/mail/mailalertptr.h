#ifndef GUARD_INFRASTRUCTURE_MAILALERTPTR_H
#define GUARD_INFRASTRUCTURE_MAILALERTPTR_H

#include <boost/shared_ptr.hpp>

namespace tenderbatchprocessing {
namespace infrastructure {
namespace mail {

	class MailAlert;

	typedef boost::shared_ptr<MailAlert> MailAlertPtr;

} // namespace mail
} // namespace infrastructure
} // namespace tenderbatchprocessing

#endif // GUARD_INFRASTRUCTURE_MAILALERTPTR_H
