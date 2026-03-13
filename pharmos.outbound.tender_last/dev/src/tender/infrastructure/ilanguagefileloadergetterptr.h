#ifndef GUARD_TENDER_INFRASTRUCTURE_I_LANGUAGEFILELOADERGETTER_PTR_H
#define GUARD_TENDER_INFRASTRUCTURE_I_LANGUAGEFILELOADERGETTER_PTR_H

#include <boost/shared_ptr.hpp>

namespace tender
{
namespace infrastructure
{
	class ILanguageFileLoaderGetter;
	typedef boost::shared_ptr< ILanguageFileLoaderGetter const> ILanguageFileLoaderGetterPtr;

} // end namespace infrastructure
} // end namespace tender

#endif // GUARD_TENDER_INFRASTRUCTURE_I_LANGUAGEFILELOADERGETTER_PTR_H
