//--------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief
 *  \author     Bischof Bjoern
 *  \date       09.07.2013
 *  \version    00.00.00.01 first version
 */
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_TENDER_COMPONENTMANAGER_TENDERAPPLCOMPMGRPTR_H
#define GUARD_TENDER_COMPONENTMANAGER_TENDERAPPLCOMPMGRPTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
    namespace componentManager
    {
        class ApplCompMgr;
        typedef boost::shared_ptr< ApplCompMgr > ApplCompMgrPtr;
    }
}

#endif
