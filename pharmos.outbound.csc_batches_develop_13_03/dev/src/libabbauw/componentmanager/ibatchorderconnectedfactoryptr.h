#ifndef GUARD_LIBABBAUW_IBATCHORDERCONNECTEDFACTORYPTR_H
#define GUARD_LIBABBAUW_IBATCHORDERCONNECTEDFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libabbauw
{
namespace componentManager 
{
    class IBatchOrderConnectedFactory;
    typedef boost::shared_ptr< IBatchOrderConnectedFactory > IBatchOrderConnectedFactoryPtr;
	
} // end namespace componentManager
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_IBATCHORDERCONNECTEDFACTORYPTR_H
