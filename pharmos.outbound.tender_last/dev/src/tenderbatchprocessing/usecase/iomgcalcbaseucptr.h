#ifndef GUARD_TENDERBATCHPROCESSING_USECASE_IOMGCALCBASEUCPTR_H
#define GUARD_TENDERBATCHPROCESSING_USECASE_IOMGCALCBASEUCPTR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------------------------------------------//
// forward declaration section
//--------------------------------------------------------------------------------------------------//
namespace tenderbatchprocessing
{
    namespace useCase
    {
        class IOMGCalcBaseUC;
        typedef boost::shared_ptr< IOMGCalcBaseUC > IOMGCalcBaseUCPtr;
    }
}

#endif //end GUARD_TENDERBATCHPROCESSING_USECASE_IOMGCALCBASEUCPTR_H
