#include "infrastructuresetup.h"
#include "ilanguagefileloadergetter.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include <libutil/applcomponent.h>
#include <libutil/util.h>
#include <libutil/log4cplus.h>
#include <libutil/dbconnection.h>
#include <libutil/login.h>
#include <libutil/exception.h>
#include <libutil/login.h>
#include <libutil/parameter.h>
#include <libutil/languagefileloader.h>

#include <libbasarcmnutil.h>

namespace CSC_Batches {
namespace infrastructure {

	InfrastructureSetup::InfrastructureSetup() : InfrastructureSetupBase( libabbauw::LoggerPool::loggerMain )
	{
		METHODNAME_DEF( InfrastructureSetup, InfrastructureSetup )
		BLOG_TRACE_METHOD( getLogger(), fun );
	}
	
	InfrastructureSetup::~InfrastructureSetup()
	{
		METHODNAME_DEF( InfrastructureSetup, ~InfrastructureSetup)
		BLOG_TRACE_METHOD( getLogger(), fun );
	}
	
	void InfrastructureSetup::injectLanguageFileLoaderGetter( CSC_Batches::infrastructure::ILanguageFileLoaderGetterPtr getter )
	{
		m_LanguageFileLoaderGetter = getter;
	}
	
	void InfrastructureSetup::doInit()
	{
		evaluateCmdline();
		initLogin();
		prepareTranslators();
		loadTranslators();
		establishDBConnection();
	}
	
	void InfrastructureSetup::prepareTranslators()
	{
		// TODO: Workaround & Bad Hack: Create the language file loader again to use the language code from login.
		// Issue must be solved in libutil!
		injectLanguageFileLoader( m_LanguageFileLoaderGetter->createLanguageFileLoader() );
	}
	
	void InfrastructureSetup::doShutdown()
	{
		shutdownDefaultImpl();
	}

}
}
