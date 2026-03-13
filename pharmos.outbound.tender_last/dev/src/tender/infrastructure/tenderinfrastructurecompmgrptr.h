//--------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief
 *  \author     Bischof Bjoern
 *  \date       28.07.2013
 *  \version    00.00.00.01 first version
 */
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_TENDER_COMPONENTMANAGER_TENDERINFRASTRUCTURECOMPMGRPTR_H
#define GUARD_TENDER_COMPONENTMANAGER_TENDERINFRASTRUCTURECOMPMGRPTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
    namespace componentManager
    {
        class InfrastructureCompMgr;
        typedef boost::shared_ptr< InfrastructureCompMgr > InfrastructureCompMgrPtr;
    }
}

#endif
