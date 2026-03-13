#ifndef GUARD_TENDER_USECASE_ARTICLEPICKERUCPTR_H
#define GUARD_TENDER_USECASE_ARTICLEPICKERUCPTR_H

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
        class ArticlePickerUC;
        typedef boost::shared_ptr< ArticlePickerUC > ArticlePickerUCPtr;
    }
}

#endif //end GUARD_TENDER_USECASE_ARTICLEPICKERUCPTR_H
