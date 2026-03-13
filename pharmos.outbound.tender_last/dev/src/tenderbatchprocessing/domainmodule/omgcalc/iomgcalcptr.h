#ifndef GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGCALCPTR_H
#define GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGCALCPTR_H

#include <boost/shared_ptr.hpp>

namespace tenderbatchprocessing
{
namespace domMod
{
namespace omgcalc
{
	class IOMGCalc;
	typedef boost::shared_ptr< IOMGCalc > IOMGCalcPtr;
}
}
}

#endif //end GUARD_DOMMOD_TENDERBATCHPROCESSING_IOMGCALCPTR_H
