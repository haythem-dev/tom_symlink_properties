#ifndef GUARD_TENDER_USECASE_PHARMACYCHAINPICKERUCPTR_H
#define GUARD_TENDER_USECASE_PHARMACYCHAINPICKERUCPTR_H

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
        class PharmacyChainPickerUC;
        typedef boost::shared_ptr< PharmacyChainPickerUC > PharmacyChainPickerUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_PHARMACYCHAINPICKERUCPTR_H