//--------------------------------------------------------------------------------------------------//
/*! \file       
 *  \brief      
 *  \author     Bischof Bjoern
 *  \date       12.06.2015
 *  \version    00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "tenderordercreatorinfrastructuresetup.h"
#include "itenderordercreatorlogingetter.h"
#include "tenderordercreatorcmdlineevaluator.h"
#include "login_definitions.h"
#include <loggerpool/loggerpool.h>

#include <infrastructure/countrycodemapper.h>
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

//#pragma warning (push)
//#pragma warning(disable: 4127 4244 4311 4312 4800 4251)
//#include <QApplication>

//#pragma warning (pop)

//--------------------------------------------------------------------------------------------------//
// namespace libBatch
//--------------------------------------------------------------------------------------------------//
namespace tenderordercreator {
namespace infrastructure {
namespace setup {

TenderOrderCreatorInfrastructureSetup::TenderOrderCreatorInfrastructureSetup() :
InfrastructureSetupBase( LoggerPool::getLoggerTenderordercreator() )
{
	METHODNAME_DEF( TenderOrderCreatorInfrastructureSetup, TenderOrderCreatorInfrastructureSetup )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

TenderOrderCreatorInfrastructureSetup::~TenderOrderCreatorInfrastructureSetup()
{
	METHODNAME_DEF( TenderOrderCreatorInfrastructureSetup, ~TenderOrderCreatorInfrastructureSetup)
	BLOG_TRACE_METHOD( getLogger(), fun );
}

void TenderOrderCreatorInfrastructureSetup::doInit()
{
	evaluateCmdline();
	login();
	loadTranslators();
	establishDBConnection();
}
    	
void TenderOrderCreatorInfrastructureSetup::login()
{
	prepareLogin();
	initLogin();
}

void TenderOrderCreatorInfrastructureSetup::prepareLogin()
{
	boost::shared_ptr<infrastructure::cmdLine::TenderordercreatorCmdLineEvaluator const> cmdLine
	( 
		boost::static_pointer_cast<infrastructure::cmdLine::TenderordercreatorCmdLineEvaluator>( getCmdLineEvaluator()) 
	);
	
	const infrastructure::cmdLine::AppArgs& appArgs = cmdLine->getAppArgs();
	libtender::infrastructure::CountryIDEnum countryEnum =
		libtender::infrastructure::CountryCodeMapper::getCountryID(appArgs.m_Country);
	basar::Int16 areaID = libtender::infrastructure::CountryCodeMapper::getRegionID(countryEnum);
	const libutil::infrastructure::login::UserLoginData userLoginData(
		tenderordercreator::infrastructure::login::USER,
		tenderordercreator::infrastructure::login::PASSWD,
		tenderordercreator::infrastructure::login::APPLICATION,
		areaID,
		tenderordercreator::infrastructure::login::LANGUAGE_ID);

	// b/c of private member libutil::infrastructure::login::ILoginPtr m_Login; in class InfrastructureSetupBase
	injectLogin( m_BatchLoginGetter->getNonInteractiveLogin(userLoginData) );
}

void TenderOrderCreatorInfrastructureSetup::doShutdown()
{
	shutdownDefaultImpl();
}

void TenderOrderCreatorInfrastructureSetup::injectLoginGetter( IBatchLoginGetterConstPtr loginGetter )
{
	m_BatchLoginGetter = loginGetter;
}

} // namespace setup
} // namespace infrastructure
} // namespace tenderordercreator
