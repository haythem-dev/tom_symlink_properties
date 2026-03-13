#ifndef GUARD_LIBTENDER_COMPONENTMANAGER_TENDERFACTORYBASEPTR_H
#define GUARD_LIBTENDER_COMPONENTMANAGER_TENDERFACTORYBASEPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
    namespace componentManager
    {
        class TenderFactoryBase;
        typedef boost::shared_ptr< TenderFactoryBase > TenderFactoryBasePtr;
    }
}

#endif //end GUARD_LIBTENDER_COMPONENTMANAGER_TENDERFACTORYBASEPTR_H
