#ifndef GUARD_TENDERBATCHPROCESSING_USECASE_OMGCALCUCPTR_H
#define GUARD_TENDERBATCHPROCESSING_USECASE_OMGCALCUCPTR_H

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
        class OMGCalcUC;
        typedef boost::shared_ptr< OMGCalcUC > OMGCalcUCPtr;
    }
}

#endif //end GUARD_TENDERBATCHPROCESSING_USECASE_OMGCALCUCPTR_H
