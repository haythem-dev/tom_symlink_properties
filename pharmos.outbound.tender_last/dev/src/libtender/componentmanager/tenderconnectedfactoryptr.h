#ifndef GUARD_LIBTENDER_COMPONENTMANAGER_TENDERCONNECTEDFACTORYPTR_H
#define GUARD_LIBTENDER_COMPONENTMANAGER_TENDERCONNECTEDFACTORYPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
    namespace componentManager
    {
        class TenderConnectedFactory;
        typedef boost::shared_ptr< TenderConnectedFactory > TenderConnectedFactoryPtr;
    }
}

#endif //end GUARD_LIBTENDER_COMPONENTMANAGER_TENDERCONNECTEDFACTORYPTR_H
