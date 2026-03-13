#ifndef GUARD_TENDERBATCHPROCESSING_USECASE_OMGCUSTOMERCALCUCPTR_H
#define GUARD_TENDERBATCHPROCESSING_USECASE_OMGCUSTOMERCALCUCPTR_H

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
        class OMGCustomerCalcUC;
        typedef boost::shared_ptr< OMGCustomerCalcUC > OMGCustomerCalcUCPtr;
    }
}

#endif //end GUARD_TENDERBATCHPROCESSING_USECASE_OMGCUSTOMERCALCUCPTR_H
