#ifndef GUARD_TENDER_USECASE_PARTNERPICKERUCPTR_H
#define GUARD_TENDER_USECASE_PARTNERPICKERUCPTR_H

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
        class PartnerPickerUC;
        typedef boost::shared_ptr< PartnerPickerUC > PartnerPickerUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_PARTNERPICKERUCPTR_H