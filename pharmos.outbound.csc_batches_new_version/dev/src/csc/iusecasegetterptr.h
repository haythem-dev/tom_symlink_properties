#ifndef GUARD_CSCBATCHES_COMPONENTMANAGER_IUSECASEGETTERPTR_H
#define GUARD_CSCBATCHES_COMPONENTMANAGER_IUSECASEGETTERPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
namespace componentManager
{
	class IUseCaseGetter;
    typedef boost::shared_ptr< IUseCaseGetter > IUseCaseGetterPtr;

} // end namespace componentManager
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_COMPONENTMANAGER_IUSECASEGETTERPTR_H
