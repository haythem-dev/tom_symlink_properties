#ifndef GUARD_TENDER_USECASE_ITENDERSTATEPTR_H
#define GUARD_TENDER_USECASE_ITENDERSTATEPTR_H

#include <boost/shared_ptr.hpp>
namespace tender
{
    namespace useCase
    {
        class ITenderStateUC;
        typedef boost::shared_ptr< ITenderStateUC > ITenderStateUCPtr;
    }
}

#endif // GUARD_TENDER_USECASE_ITENDERSTATEPTR_H
