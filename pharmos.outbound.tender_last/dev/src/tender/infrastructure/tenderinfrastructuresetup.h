#ifndef GUARD_TENDER_INFRASTRUCTURE_SETUP_TENDERINFRASTRUCTURESETUP_H
#define GUARD_TENDER_INFRASTRUCTURE_SETUP_TENDERINFRASTRUCTURESETUP_H

#include <libutil/infrastructurecomponent.h>
#include <infrastructure/ilanguagefileloadergetterptr.h>

namespace tender
{
namespace infrastructure
{
class TenderInfrastructureSetup : public libutil::infrastructure::InfrastructureSetupBase
{
public:
	TenderInfrastructureSetup();
	virtual ~TenderInfrastructureSetup();

	void injectLanguageFileLoaderGetter( ILanguageFileLoaderGetterPtr );

protected:
	virtual void doInit();
	virtual void doShutdown();

private:
	TenderInfrastructureSetup( const TenderInfrastructureSetup & r );
	TenderInfrastructureSetup & operator=( const TenderInfrastructureSetup & r );

	void prepareTranslators();

	ILanguageFileLoaderGetterPtr m_LanguageFileLoaderGetter;
};

} // end namespace infrastructure
} // end namespace tender

#endif // GUARD_TENDER_INFRASTRUCTURE_SETUP_TENDERINFRASTRUCTURESETUP_H
