#ifndef GUARD_TENDER_USECASE_CUSTOMERLISTMAINTAINUCPTR_H
#define GUARD_TENDER_USECASE_CUSTOMERLISTMAINTAINUCPTR_H

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
        class CustomerListMaintainUC;
        typedef boost::shared_ptr< CustomerListMaintainUC > CustomerListMaintainUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_CUSTOMERLISTMAINTAINUCPTR_H