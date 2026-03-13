//--------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief
 *  \author     Bischof Bjoern
 *  \date       28.07.2013
 *  \version    00.00.00.01 first version
 */
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_TENDERORDERCREATOR_COMPONENTMANAGER_INFRASTRUCTURECOMPMGRPTR_H
#define GUARD_TENDERORDERCREATOR_COMPONENTMANAGER_INFRASTRUCTURECOMPMGRPTR_H

#include <boost/shared_ptr.hpp>

namespace tenderordercreator
{
    namespace componentManager
    {
        class InfrastructureCompMgr;
        typedef boost::shared_ptr< InfrastructureCompMgr > InfrastructureCompMgrPtr;
    }
}

#endif 
