//--------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief
 *  \author     Bischof Bjoern
 *  \date       09.02.2014
 *  \version    00.00.00.01 first version
 */
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBTENDER_COMPONENTMANAGER_LIBTENDERAPPLCOMPMGRPTR_H
#define GUARD_LIBTENDER_COMPONENTMANAGER_LIBTENDERAPPLCOMPMGRPTR_H

#include <boost/shared_ptr.hpp>

namespace libtender
{
    namespace componentManager
    {
        class ApplCompMgr;
        typedef boost::shared_ptr< ApplCompMgr > ApplCompMgrPtr;
    }
}

#endif
