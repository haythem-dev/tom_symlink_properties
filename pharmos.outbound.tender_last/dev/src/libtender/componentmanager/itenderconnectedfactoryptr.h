#ifndef GUARD_LIBTENDER_COMPONENTMANAGER_ITENDERCONNECTEDFACTORYPTR_H
#define GUARD_LIBTENDER_COMPONENTMANAGER_ITENDERCONNECTEDFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
    namespace componentManager
    {
        class ITenderConnectedFactory;
        typedef boost::shared_ptr< ITenderConnectedFactory > ITenderConnectedFactoryPtr;
    }
}

#endif //end GUARD_LIBTENDER_COMPONENTMANAGER_ITENDERCONNECTEDFACTORYPTR_H
