#ifndef GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERGETTERPTR_H
#define GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERGETTERPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace domMod
{
    namespace customer
    {
        class ICustomerGetter;
		typedef boost::shared_ptr< ICustomerGetter > ICustomerGetterPtr;
    }
}
}
#endif // GUARD_LIBABBAUW_DOMMOD_CUSTOMER_ICUSTOMERGETTERPTR_H
