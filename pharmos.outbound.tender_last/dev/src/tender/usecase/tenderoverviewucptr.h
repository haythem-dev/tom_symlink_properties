#ifndef GUARD_TENDER_USECASE_TENDEROVERVIEWUCPTR_H
#define GUARD_TENDER_USECASE_TENDEROVERVIEWUCPTR_H

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
        class TenderOverviewUC;
        typedef boost::shared_ptr< TenderOverviewUC > TenderOverviewUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_TENDEROVERVIEWUCPTR_H
