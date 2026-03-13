#ifndef GUARD_TENDER_USECASE_CSCORDERUCPTR_H
#define GUARD_TENDER_USECASE_CSCORDERUCPTR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------------------------------------------//
// forward declaration section
//--------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace useCase
    {
        class CSCOrderUC;
        typedef boost::shared_ptr< CSCOrderUC > CSCOrderUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_CSCORDERUCPTR_H
