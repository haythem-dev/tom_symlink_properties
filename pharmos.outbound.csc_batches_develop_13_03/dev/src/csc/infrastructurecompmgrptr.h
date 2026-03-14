#ifndef GUARD_CSCBATCHES_COMPONENTMANAGER_INFRASTRUCTURECOMPONENTMANAGERPTR_H
#define GUARD_CSCBATCHES_COMPONENTMANAGER_INFRASTRUCTURECOMPONENTMANAGERPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
namespace componentManager
{
	class InfrastructureCompMgr;
    typedef boost::shared_ptr< InfrastructureCompMgr > InfrastructureCompMgrPtr;

} // end namespace componentManager
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_COMPONENTMANAGER_INFRASTRUCTURECOMPONENTMANAGERPTR_H
