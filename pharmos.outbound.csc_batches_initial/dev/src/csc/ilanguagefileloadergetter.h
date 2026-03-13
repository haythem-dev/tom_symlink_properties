#ifndef GUARD_CSCBATCHES_INFRASTRUCTURE_ILANGUAGEFILELOADERGETTER_H
#define GUARD_CSCBATCHES_INFRASTRUCTURE_ILANGUAGEFILELOADERGETTER_H

#include <libutil/languagefileloader.h>

namespace CSC_Batches {
namespace infrastructure {

class ILanguageFileLoaderGetter
{
public:
	virtual ~ILanguageFileLoaderGetter() {};

	virtual libutil::infrastructure::i18n::ILanguageFileLoaderPtr createLanguageFileLoader() const = 0;
};

}
} 

#endif // GUARD_CSCBATCHES_INFRASTRUCTURE_ILANGUAGEFILELOADERGETTER_H
