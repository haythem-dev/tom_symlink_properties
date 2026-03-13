#ifndef GUARD_TENDER_USECASE_CSCORDERMAINTAINUCPTR_H
#define GUARD_TENDER_USECASE_CSCORDERMAINTAINUCPTR_H

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
        class CSCOrderMaintainUC;
        typedef boost::shared_ptr< CSCOrderMaintainUC > CSCOrderMaintainUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_CSCORDERMAINTAINUCPTR_H
