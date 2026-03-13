#ifndef GUARD_TENDER_USECASE_SUPPLIERPICKERUCPTR_H
#define GUARD_TENDER_USECASE_SUPPLIERPICKERUCPTR_H

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
        class SupplierPickerUC;
        typedef boost::shared_ptr< SupplierPickerUC > SupplierPickerUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_SUPPLIERPICKERUCPTR_H