#ifndef GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERFINDERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERFINDERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod
{
    namespace customer
    {
        class ICustomerFinder;
		typedef boost::shared_ptr< ICustomerFinder > ICustomerFinderPtr;
    }
}
}
#endif // GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERFINDERPTR_H
