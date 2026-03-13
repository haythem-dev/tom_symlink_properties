#ifndef GUARD_TENDER_USECASE_TENDERINVOICEUCPTR_H
#define GUARD_TENDER_USECASE_TENDERINVOICEUCPTR_H

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
        class TenderInvoiceUC;
        typedef boost::shared_ptr< TenderInvoiceUC > TenderInvoiceUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_TENDERINVOICEUCPTR_H
