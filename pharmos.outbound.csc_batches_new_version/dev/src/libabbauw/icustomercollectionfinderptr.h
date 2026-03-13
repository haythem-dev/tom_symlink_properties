#ifndef GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERCOLLECTIONFINDERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERCOLLECTIONFINDERPTR_H

#include <boost/shared_ptr.hpp>
namespace libabbauw
{
namespace domMod
{
    namespace customer
    {
        class ICustomerCollectionFinder;
		typedef boost::shared_ptr< ICustomerCollectionFinder > ICustomerCollectionFinderPtr;
    }
}
}
#endif // GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERCOLLECTIONFINDERPTR_H
