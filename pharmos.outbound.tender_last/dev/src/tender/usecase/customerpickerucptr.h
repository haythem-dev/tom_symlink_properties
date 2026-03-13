#ifndef GUARD_TENDER_USECASE_CUSTOMERPICKERUCPTR_H
#define GUARD_TENDER_USECASE_CUSTOMERPICKERUCPTR_H

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
        class CustomerPickerUC;
        typedef boost::shared_ptr< CustomerPickerUC > CustomerPickerUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_CUSTOMERPICKERUCPTR_H