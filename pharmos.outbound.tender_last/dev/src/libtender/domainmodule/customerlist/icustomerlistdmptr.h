#ifndef GUARD_LIBTENDER_DOMMOD_CUSTOMER_ICUSTOMERLISTDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_CUSTOMER_ICUSTOMERLISTDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace customerlist
{
	class ICustomerListDM;
	typedef boost::shared_ptr< ICustomerListDM > ICustomerListDMPtr;
} //end namespace customerlist
} //end namespace domMod
} //end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_CUSTOMER_ICUSTOMERLISTDMPTR_H