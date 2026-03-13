#ifndef GUARD_CSCBATCHES_COMPONENTMANAGER_APPLCOMPONENTMANAGERPTR_H
#define GUARD_CSCBATCHES_COMPONENTMANAGER_APPLCOMPONENTMANAGERPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
	namespace componentManager
	{
		class ApplComponentManager;
		typedef boost::shared_ptr< ApplComponentManager > ApplComponentManagerPtr;

	} // end namespace componentManager
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_COMPONENTMANAGER_APPLCOMPONENTMANAGERPTR_H
