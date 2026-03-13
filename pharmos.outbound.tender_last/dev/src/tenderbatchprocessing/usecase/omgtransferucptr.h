#ifndef GUARD_TENDERBATCHPROCESSING_USECASE_OMGTRANSFERUCPTR_H
#define GUARD_TENDERBATCHPROCESSING_USECASE_OMGTRANSFERUCPTR_H

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
        class OMGTransferUC;
        typedef boost::shared_ptr< OMGTransferUC > OMGTransferUCPtr;
    }
}

#endif //end GUARD_TENDERBATCHPROCESSING_USECASE_OMTRANSFERUCPTR_H
