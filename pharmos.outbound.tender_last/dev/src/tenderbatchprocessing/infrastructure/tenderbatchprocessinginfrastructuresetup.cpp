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
#include "tenderbatchprocessinginfrastructuresetup.h"
#include "itenderbatchprocessinglogingetter.h"
#include "tenderbatchprocessingcmdlineevaluator.h"
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
namespace tenderbatchprocessing {
namespace infrastructure {
namespace setup {

TenderBatchProcessingInfrastructureSetup::TenderBatchProcessingInfrastructureSetup() :
InfrastructureSetupBase( LoggerPool::getLoggerTenderbatchprocessing() )
{
	METHODNAME_DEF( TenderBatchProcessingInfrastructureSetup, TenderBatchProcessingInfrastructureSetup )
	BLOG_TRACE_METHOD( getLogger(), fun );
}

TenderBatchProcessingInfrastructureSetup::~TenderBatchProcessingInfrastructureSetup()
{
	METHODNAME_DEF( TenderBatchProcessingInfrastructureSetup, ~TenderBatchProcessingInfrastructureSetup)
	BLOG_TRACE_METHOD( getLogger(), fun );
}

void TenderBatchProcessingInfrastructureSetup::doInit()
{
	evaluateCmdline();
	login();
	loadTranslators();
	establishDBConnection();
}
    	
void TenderBatchProcessingInfrastructureSetup::login()
{
	prepareLogin();
	initLogin();
}

void TenderBatchProcessingInfrastructureSetup::prepareLogin()
{
	boost::shared_ptr<infrastructure::cmdLine::TenderbatchprocessingCmdLineEvaluator const> cmdLine
	( 
		boost::static_pointer_cast<infrastructure::cmdLine::TenderbatchprocessingCmdLineEvaluator>( getCmdLineEvaluator()) 
	);
	
	const infrastructure::cmdLine::AppArgs& appArgs = cmdLine->getAppArgs();
	libtender::infrastructure::CountryIDEnum countryEnum =
		libtender::infrastructure::CountryCodeMapper::getCountryID(appArgs.m_Country);
	basar::Int16 areaID = libtender::infrastructure::CountryCodeMapper::getRegionID(countryEnum);
	const libutil::infrastructure::login::UserLoginData userLoginData(
		tenderbatchprocessing::infrastructure::login::USER,
		tenderbatchprocessing::infrastructure::login::PASSWD,
		tenderbatchprocessing::infrastructure::login::APPLICATION,
		areaID,
		tenderbatchprocessing::infrastructure::login::LANGUAGE_ID);

	// b/c of private member libutil::infrastructure::login::ILoginPtr m_Login; in class InfrastructureSetupBase
	injectLogin( m_BatchLoginGetter->getNonInteractiveLogin(userLoginData) );
}

void TenderBatchProcessingInfrastructureSetup::doShutdown()
{
	shutdownDefaultImpl();
}

void TenderBatchProcessingInfrastructureSetup::injectLoginGetter( IBatchLoginGetterConstPtr loginGetter )
{
	m_BatchLoginGetter = loginGetter;
}

} // namespace setup
} // namespace infrastructure
} // namespace tenderbatchprocessing
