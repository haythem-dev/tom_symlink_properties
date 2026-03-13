#ifndef GUARD_CSCBATCHES_COMPONENTMANAGER_ICSVEXPORTGETTERPTR_H
#define GUARD_CSCBATCHES_COMPONENTMANAGER_ICSVEXPORTGETTERPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches
{
namespace componentManager
{
	class ICSVExportGetter;
    typedef boost::shared_ptr< ICSVExportGetter > ICSVExportGetterPtr;

} // end namespace componentManager
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_COMPONENTMANAGER_ICSVEXPORTGETTERPTR_H
