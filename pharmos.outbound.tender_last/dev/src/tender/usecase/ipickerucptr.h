#ifndef GUARD_TENDER_USECASE_IPICKERUCPTR_H
#define GUARD_TENDER_USECASE_IPICKERUCPTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
namespace useCase
{
	class IPickerUC;
    typedef boost::shared_ptr< IPickerUC > IPickerUCPtr;

} //end namespace useCase
} //end namespace tender

#endif // end GUARD_TENDER_USECASE_IPICKERUCPTR_H
