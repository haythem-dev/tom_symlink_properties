#ifndef GUARD_IPARAMETERPROXYPTR_H
#define GUARD_IPARAMETERPROXYPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender {
namespace infrastructure {
namespace cscclient {

	class ICscClientParameterProxy;

	typedef boost::shared_ptr<ICscClientParameterProxy> ICscClientParameterProxyPtr;
}
}
}

#endif
