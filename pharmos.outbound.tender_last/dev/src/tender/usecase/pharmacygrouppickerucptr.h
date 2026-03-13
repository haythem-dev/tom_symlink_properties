#ifndef GUARD_TENDER_USECASE_PHARMACYGROUPPICKERUCPTR_H
#define GUARD_TENDER_USECASE_PHARMACYGROUPPICKERUCPTR_H

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
        class PharmacyGroupPickerUC;
        typedef boost::shared_ptr< PharmacyGroupPickerUC > PharmacyGroupPickerUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_PHARMACYGROUPPICKERUCPTR_H