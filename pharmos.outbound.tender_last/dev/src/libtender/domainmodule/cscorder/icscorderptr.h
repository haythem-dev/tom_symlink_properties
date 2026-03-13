#ifndef GUARD_DOMMOD_CSCORDER_ICSCORDERPTR_H
#define GUARD_DOMMOD_CSCORDER_ICSCORDERPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender {
namespace domMod {
namespace cscorder {

	class ICscOrder;
	typedef boost::shared_ptr< ICscOrder > ICscOrderPtr;

}
}
}

#endif // GUARD_DOMMOD_CSCORDER_ICSCORDERPTR_H
