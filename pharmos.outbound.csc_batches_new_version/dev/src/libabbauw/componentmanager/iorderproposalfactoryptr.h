#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALFACTORYPTR_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace componentManager 
{
    class IOrderProposalFactory;
    typedef boost::shared_ptr< IOrderProposalFactory > IOrderProposalFactoryPtr;
	
} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_IORDERPROPOSALFACTORYPTR_H
