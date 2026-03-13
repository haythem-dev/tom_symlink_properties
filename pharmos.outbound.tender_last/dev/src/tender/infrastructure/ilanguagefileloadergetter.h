#ifndef GUARD_TENDER_INFRASTRUCTURE_I_LANGUAGEFILELOADERGETTER_H
#define GUARD_TENDER_INFRASTRUCTURE_I_LANGUAGEFILELOADERGETTER_H

#include <libutil/languagefileloader.h>

namespace tender
{
namespace infrastructure
{
class ILanguageFileLoaderGetter
{
public:
	virtual ~ILanguageFileLoaderGetter() {};

	virtual libutil::infrastructure::i18n::ILanguageFileLoaderPtr createLanguageFileLoader() const = 0;
};

} // end namespace infrastructure
} // end namespace tender

#endif // GUARD_TENDER_INFRASTRUCTURE_I_LANGUAGEFILELOADERGETTER_H
