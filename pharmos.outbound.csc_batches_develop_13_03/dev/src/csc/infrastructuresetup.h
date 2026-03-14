#ifndef GUARD_CSCBATCHES_INFRASTRUCTURE_SETUP_INFRASTRUCTURESETUP_H
#define GUARD_CSCBATCHES_INFRASTRUCTURE_SETUP_INFRASTRUCTURESETUP_H

#include <libutil/infrastructurecomponent.h>
#include "ilanguagefileloadergetterptr.h"

namespace CSC_Batches {
namespace infrastructure {

class InfrastructureSetup : public libutil::infrastructure::InfrastructureSetupBase
{
public:
	InfrastructureSetup();
	virtual ~InfrastructureSetup();

	void injectLanguageFileLoaderGetter( CSC_Batches::infrastructure::ILanguageFileLoaderGetterPtr );

protected:
	virtual void doInit();
	virtual void doShutdown();

private:
	InfrastructureSetup( const InfrastructureSetup & r );
	InfrastructureSetup & operator=( const InfrastructureSetup & r );

	void prepareTranslators();

	ILanguageFileLoaderGetterPtr m_LanguageFileLoaderGetter;
};

} 
} 

#endif // GUARD_CSCBATCHES_INFRASTRUCTURE_SETUP_INFRASTRUCTURESETUP_H
