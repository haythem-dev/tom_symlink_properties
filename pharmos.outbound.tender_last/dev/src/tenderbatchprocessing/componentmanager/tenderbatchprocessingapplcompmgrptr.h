#ifndef GUARD_TENDERBATCHPROCESSING_COMPONENTMANAGER_APPLCOMPMGRPTR_H
#define GUARD_TENDERBATCHPROCESSING_COMPONENTMANAGER_APPLCOMPMGRPTR_H

#include <boost/shared_ptr.hpp>

namespace tenderbatchprocessing
{
    namespace componentManager
    {
		class ApplCompMgr;
		typedef boost::shared_ptr< ApplCompMgr > ApplCompMgrPtr;
    }
}

#endif
