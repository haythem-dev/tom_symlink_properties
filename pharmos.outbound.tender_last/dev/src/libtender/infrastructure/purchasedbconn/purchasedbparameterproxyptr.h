#ifndef GUARD_PURCHASEDBPARAMETERPROXYPTR_H
#define GUARD_PURCHASEDBPARAMETERPROXYPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender {
namespace infrastructure {
namespace db {

	class PurchaseDBParameterProxy;

	typedef boost::shared_ptr<PurchaseDBParameterProxy> PurchaseDBParameterProxyPtr;

}
}
}

#endif // GUARD_PURCHASEDBPARAMETERPROXYPTR_H
