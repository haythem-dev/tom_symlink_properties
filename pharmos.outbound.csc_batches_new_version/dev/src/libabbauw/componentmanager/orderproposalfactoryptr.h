#ifndef GUARD_LIBABBAUW_COMPONENTMANAGER_ORDERPROPOSALFACTORYPTR_H
#define GUARD_LIBABBAUW_COMPONENTMANAGER_ORDERPROPOSALFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace componentManager 
{
    class OrderProposalFactory;
    typedef boost::shared_ptr< OrderProposalFactory > OrderProposalFactoryPtr;
	
} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_COMPONENTMANAGER_ORDERPROPOSALFACTORYPTR_H
