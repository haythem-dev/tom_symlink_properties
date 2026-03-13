#ifndef GUARD_LIBTENDER_DOMMOD_CUSTOMER_ICUSTOMERCOLLECTIONDMPTR_H
#define GUARD_LIBTENDER_DOMMOD_CUSTOMER_ICUSTOMERCOLLECTIONDMPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
namespace domMod
{
namespace customer
{
	class ICustomerCollectionDM;
	typedef boost::shared_ptr< ICustomerCollectionDM > ICustomerCollectionDMPtr;
} //end namespace customer
} //end namespace domMod
} //end namespace libtender

#endif //end GUARD_LIBTENDER_DOMMOD_CUSTOMER_ICUSTOMERCOLLECTIONDMPTR_H