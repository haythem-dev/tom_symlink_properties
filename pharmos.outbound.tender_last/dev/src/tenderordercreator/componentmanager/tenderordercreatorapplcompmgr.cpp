//----------------------------------------------------------------------------//
/*! \file
 *  \brief
 *  \author Bjoern Bischof
 *  \date   08.07.2013
 */
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// include section
//----------------------------------------------------------------------------//
// global
#include <infrastructure/libtenderapplcompmgr.h>
#include "tenderordercreatorapplcompmgr.h"
#include "loggerpool/loggerpool.h"

// uc
#include "usecase/mainucptr.h"
#include "usecase/mainuc.h"

//dommod
#include "domainmodule/cscorder/cscorderdmptr.h"
#include "domainmodule/cscorder/cscorderdm.h"

// libutil
#include <libutil/util.h>
#include <libutil/dbconnection.h>
#include <libutil/session.h>

#include <libutil/infrastructure/parameter/iparametergateway.h>

// basar
#include <libbasarcmnutil_logging.h>
#include <libbasardbsql_databaseinfo.h>

//----------------------------------------------------------------------------//
// class definition setcion
//----------------------------------------------------------------------------//
namespace tenderordercreator {
namespace componentManager {

ApplCompMgr::ApplCompMgr() :
	m_Logger( tenderordercreator::LoggerPool::getLoggerTenderordercreator() )
{
	METHODNAME_DEF( ApplCompMgr, ApplCompMgr )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

ApplCompMgr::~ApplCompMgr()
{
	METHODNAME_DEF( ApplCompMgr, ~ApplCompMgr )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

void ApplCompMgr::setCmdLineArguments( const infrastructure::cmdLine::AppArgs& appArgs )
{
	METHODNAME_DEF( ApplCompMgr, setCmdLineArgurments )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_AppArgs = appArgs;
}

void ApplCompMgr::injectCscClientConnection( libtender::infrastructure::cscclient::ICscClientConnectionPtr conn )
{
	METHODNAME_DEF( ApplCompMgr, injectCscClientConnection )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_CscClientConnection = conn;
}

void ApplCompMgr::injectMailAlert( infrastructure::mail::IMailAlertPtr mailAlert )
{
	METHODNAME_DEF( ApplCompMgr, injectMailAlert )
	BLOG_TRACE_METHOD( m_Logger, fun );

	m_MailAlert = mailAlert;
}

void ApplCompMgr::doClear()
{
	METHODNAME_DEF( ApplCompMgr, doClear )
	BLOG_TRACE_METHOD( m_Logger, fun );
}

void ApplCompMgr::injectPurchaseDBParameterProxy( libtender::infrastructure::db::IPurchaseDBParameterProxyPtr proxy )
{
	getLIBtenderApplCompMgr()->injectPurchaseDBParameterProxy( proxy );
}

void ApplCompMgr::injectParameterGateway( libutil::infrastructure::parameter::IParameterGatewayPtr parameterGateway )
{
	m_ParameterGateway = parameterGateway;
}

libtender::componentManager::ApplCompMgrPtr ApplCompMgr::getLIBtenderApplCompMgr()
{
	METHODNAME_DEF( ApplCompMgr, getLIBtenderApplCompMgr )
	BLOG_TRACE_METHOD( m_Logger, fun );

	if( 0 == m_LIBtenderApplCompMgr.get() )
	{
		m_LIBtenderApplCompMgr = libtender::componentManager::ApplCompMgrPtr( new libtender::componentManager::ApplCompMgr() );
		m_LIBtenderApplCompMgr->injectDBConnection( getDBConnection() );
		m_LIBtenderApplCompMgr->injectSession(getSession());
	}

	return m_LIBtenderApplCompMgr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <UC getting>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
libutil::useCase::IRunnablePtr ApplCompMgr::getMainUC()
{
	METHODNAME_DEF( ApplCompMgr, getMainUC )
	BLOG_TRACE_METHOD( m_Logger, fun );

	tenderordercreator::useCase::MainUCPtr mainUC = tenderordercreator::useCase::MainUCPtr( new tenderordercreator::useCase::MainUC );
	mainUC->injectCmdLineParameterGetter( shared_from_this() );
	mainUC->injectMailAlert(getMailAlert());
	mainUC->injectScheduledDeliveryCollectionDM(getLIBtenderApplCompMgr()->getScheduledDeliveryCollectionDM());
	mainUC->injectCscOrderDM(getCscOrderDM());

	basar::I18nString orderType;
	m_ParameterGateway->getParameterValue( getSession()->getUser()->getAreaID(), "ORDER_TYPE", orderType );
	mainUC->injectOrderType( orderType );
	
	return mainUC;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// </UC getting>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

basar::VarString ApplCompMgr::getCountry() const
{
	return m_AppArgs.m_Country;
}

basar::Date ApplCompMgr::getDateFrom() const
{
	return m_AppArgs.m_DateFrom;
}

basar::Date ApplCompMgr::getDateTo() const
{
	return m_AppArgs.m_DateTo;
}

basar::Int16 ApplCompMgr::getNextDays() const
{
	return m_AppArgs.m_NextDays;
}

libtender::domMod::cscorder::ICscOrderPtr ApplCompMgr::getCscOrderDM()
{
	libtender::domMod::cscorder::CscOrderDMPtr p = libtender::domMod::cscorder::CscOrderDMPtr(new libtender::domMod::cscorder::CscOrderDM(LoggerPool::getLoggerTenderordercreator()));
	p->injectCscClientConnection(getCscClientConnection());
	return p;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <ACC getting>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// </ACC getting>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <OTHERS>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
basar::db::aspect::ConnectionRef ApplCompMgr::getDBConnection()
{
	METHODNAME_DEF( ApplCompMgr, getDBConnection )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return getDBConnectionData()->getCurrentConnection();
}

libtender::infrastructure::cscclient::ICscClientConnectionPtr ApplCompMgr::getCscClientConnection()
{
	METHODNAME_DEF( ApplCompMgr, getCscClientConnection )
	BLOG_TRACE_METHOD( m_Logger, fun );

	return m_CscClientConnection;
}

infrastructure::mail::IMailAlertPtr	ApplCompMgr::getMailAlert()
{
	return m_MailAlert;
}

} // end namnespace componentManager
} // end namnespace tenderordercreator
