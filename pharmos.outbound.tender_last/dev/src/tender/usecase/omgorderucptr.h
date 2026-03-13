#ifndef GUARD_TENDER_USECASE_OMGORDERUCPTR_H
#define GUARD_TENDER_USECASE_OMGORDERUCPTR_H

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
        class OMGOrderUC;
        typedef boost::shared_ptr< OMGOrderUC > OMGOrderUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_TENDEROMGORDERUCPTR_H
