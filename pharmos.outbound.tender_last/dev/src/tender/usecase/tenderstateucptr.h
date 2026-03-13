#ifndef GUARD_TENDER_USECASE_TENDERSTATE_UC_PTR_H
#define GUARD_TENDER_USECASE_TENDERSTATE_UC_PTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
    namespace useCase
    {

        class TenderStateUC;
        typedef boost::shared_ptr< TenderStateUC > TenderStateUCPtr;
    }
}

#endif // end GUARD_TENDER_USECASE_TENDERSTATE_UC_PTR_H
