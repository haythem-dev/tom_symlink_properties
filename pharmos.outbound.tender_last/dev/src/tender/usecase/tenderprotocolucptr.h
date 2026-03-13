#ifndef GUARD_TENDER_USECASE_TENDERPROTOCOLUCPTR_H
#define GUARD_TENDER_USECASE_TENDERPROTOCOLUCPTR_H

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
        class TenderProtocolUC;
        typedef boost::shared_ptr< TenderProtocolUC > TenderProtocolUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_TENDERPROTOCOLUCPTR_H
