#ifndef GUARD_TENDER_USECASE_CUSTOMERSELECTIONUCPTR_H
#define GUARD_TENDER_USECASE_CUSTOMERSELECTIONUCPTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
    namespace useCase
    {
        class CustomerSelectionUC;
        typedef boost::shared_ptr<CustomerSelectionUC> CustomerSelectionUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_CUSTOMERSELECTIONUCPTR_H
