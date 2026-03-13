//#include "obstinfo/obst_info.h"
#include "tenderinfrastructuresetup.h"
#include "loggerpool/tenderloggerpool.h"
#include <infrastructure/ilanguagefileloadergetter.h>

//#include "infrastructure/setup/iobstlogingetter.h"
//#include "infrastructure/cmdline/obstcmdlineevaluator.h"
//#include "infrastructure/cmdline/obstcmdlineevaluatorptr.h"

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

namespace tender
{
namespace infrastructure
{
TenderInfrastructureSetup::TenderInfrastructureSetup()
: InfrastructureSetupBase( libtender::LoggerPool::getLoggerTender() )
{
	METHODNAME_DEF( TenderInfrastructureSetup, TenderInfrastructureSetup )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

TenderInfrastructureSetup::~TenderInfrastructureSetup()
{
	METHODNAME_DEF( TenderInfrastructureSetup, ~TenderInfrastructureSetup)
	BLOG_TRACE_METHOD( getLogger(), fun );
}

void TenderInfrastructureSetup::injectLanguageFileLoaderGetter( ILanguageFileLoaderGetterPtr getter )
{
	m_LanguageFileLoaderGetter = getter;
}

void TenderInfrastructureSetup::doInit()
{
	evaluateCmdline();
	initLogin();
	prepareTranslators();
	loadTranslators();
	establishDBConnection();
}

void TenderInfrastructureSetup::prepareTranslators()
{
	// DK: Workaround & Bad Hack: Create the language file loader again to use the language code from login.
	// Issue must be solved in libutil!
	injectLanguageFileLoader( m_LanguageFileLoaderGetter->createLanguageFileLoader() );
}

void TenderInfrastructureSetup::doShutdown()
{
	shutdownDefaultImpl();
}

} // end namespace infrastructure
} // end namespace tender
