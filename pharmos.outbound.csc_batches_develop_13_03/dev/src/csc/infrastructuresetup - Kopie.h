#ifndef GUARD_CSCBATCHES_INFRASTRUCTURE_SETUP_INFRASTRUCTURESETUPPTR_H
#define GUARD_CSCBATCHES_INFRASTRUCTURE_SETUP_INFRASTRUCTURESETUPPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches {
namespace infrastructure {

	class InfrastructureSetup;
	typedef boost::shared_ptr< InfrastructureSetup > InfrastructureSetupPtr;

} 
} 

#endif // GUARD_CSCBATCHES_INFRASTRUCTURE_SETUP_INFRASTRUCTURESETUPPTR_H
