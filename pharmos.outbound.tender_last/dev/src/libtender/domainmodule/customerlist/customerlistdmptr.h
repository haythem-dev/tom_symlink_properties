#ifndef GUARD_LIBTENDER_DOMMOD_CUSTOMER_CUSTOMMERLISTDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_CUSTOMER_CUSTOMMERLISTDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace customerlist
{
	class CustomerListDM;
	typedef boost::shared_ptr< CustomerListDM > CustomerListDMPtr;
} //end namespace customerlist
} //end namespace domMod
} //end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_CUSTOMER_CUSTOMMERLISTDMPTR_H