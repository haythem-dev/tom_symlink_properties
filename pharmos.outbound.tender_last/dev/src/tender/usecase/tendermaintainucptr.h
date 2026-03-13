#ifndef GUARD_TENDER_USECASE_TENDERMAINTAINUCPTR_H
#define GUARD_TENDER_USECASE_TENDERMAINTAINUCPTR_H

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
        class TenderMaintainUC;
        typedef boost::shared_ptr< TenderMaintainUC > TenderMaintainUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_TENDERMAINTAINUCPTR_H
