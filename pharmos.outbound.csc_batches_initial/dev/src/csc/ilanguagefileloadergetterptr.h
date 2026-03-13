#ifndef GUARD_CSCBATCHES_INFRASTRUCTURE_ILANGUAGEFILELOADERGETTERPTR_H
#define GUARD_CSCBATCHES_INFRASTRUCTURE_ILANGUAGEFILELOADERGETTERPTR_H

#include <boost/shared_ptr.hpp>

namespace CSC_Batches {
namespace infrastructure {

	class ILanguageFileLoaderGetter;
	typedef boost::shared_ptr< ILanguageFileLoaderGetter const> ILanguageFileLoaderGetterPtr;

} 
} 

#endif // GUARD_CSCBATCHES_INFRASTRUCTURE_ILANGUAGEFILELOADERGETTERPTR_H
