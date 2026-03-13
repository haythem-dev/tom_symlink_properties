#ifndef GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGCUSTOMERCALCPTR_H
#define GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGCUSTOMERCALCPTR_H

#include <boost/shared_ptr.hpp>

namespace tenderbatchprocessing
{
namespace domMod
{
namespace omgcustomercalc
{
	class IOMGCustomerCalc;
	typedef boost::shared_ptr< IOMGCustomerCalc > IOMGCustomerCalcPtr;
}
}
}

#endif //end GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGCUSTOMERCALCPTR_H
