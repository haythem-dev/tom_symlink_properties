#ifndef GUARD_PARAMETERPROXYPTR_H
#define GUARD_PARAMETERPROXYPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender {
namespace infrastructure {
namespace cscclient {

	class CscClientParameterProxy;

	typedef boost::shared_ptr<CscClientParameterProxy> CscClientParameterProxyPtr;

}
}
}

#endif
