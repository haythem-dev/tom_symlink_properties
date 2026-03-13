#include <infrastructure/libtenderapplcompmgr.h>
#include "tenderbatchprocessingapplcompmgr.h"
#include "loggerpool/loggerpool.h"

// uc
#include "usecase/mainucptr.h"
#include "usecase/mainuc.h"

#include "usecase/omgtransferuc.h"
#include "usecase/omgtransferucptr.h"

#include "usecase/omgcalcucptr.h"
#include "usecase/omgcalcuc.h"

#include "usecase/omgcustomercalcucptr.h"
#include "usecase/omgcustomercalcuc.h"

#include "usecase/omgsuppliercalcucptr.h"
#include "usecase/omgsuppliercalcuc.h"

//dommod
#include "domainmodule/cscorder/cscorderdmptr.h"
#include "domainmodule/cscorder/cscorderdm.h"

#include "domainmodule/omgtransfer/omgtransferdmptr.h"
#include "domainmodule/omgtransfer/omgtransferdm.h"

#include "domainmodule/omgcalc/omgcalcdmptr.h"
#include "domainmodule/omgcalc/omgcalcdm.h"

#include "domainmodule/omgcustomercalc/omgcustomercalcdmptr.h"
#include "domainmodule/omgcustomercalc/omgcustomercalcdm.h"

#include "domainmodule/omgsuppliercalc/omgsuppliercalcdmptr.h"
#include "domainmodule/omgsuppliercalc/omgsuppliercalcdm.h"

// libutil
#include <libutil/util.h>
#include <libutil/dbconnection.h>
#include <libutil/session.h>

#include <libutil/infrastructure/parameter/iparametergateway.h>

#include "infrastructure/omgcalc/omgcalcacc_definitions.h"
#include "infrastructure/omgcalc/omgcalcacc.h"

#include "infrastructure/omgcustomercalc/omgcustomercalcacc_definitions.h"
#include "infrastructure/omgcustomercalc/omgcustomercalcacc.h"

#include "infrastructure/omgsuppliercalc/omgsuppliercalcacc_definitions.h"
#include "infrastructure/omgsuppliercalc/omgsuppliercalcacc.h"

#include "infrastructure/omgtransfer/omgtransferacc_definitions.h"
#include "infrastructure/omgtransfer/omgtransferacc.h"

#include "infrastructure/remoteprocedure/remoteprocedureacc_definitions.h"
#include "infrastructure/remoteprocedure/remoteprocedureacc.h"

// basar
#include <libbasarcmnutil_logging.h>
#include <libbasardbsql_databaseinfo.h>
#include <libbasardbaspect_manager.h>

//----------------------------------------------------------------------------//
// class definition setcion
//----------------------------------------------------------------------------//
namespace tenderbatchprocessing {
namespace componentManager {

ApplCompMgr::ApplCompMgr() :
	m_Logger( tenderbatchprocessing::LoggerPool::getLoggerTenderbatchprocessing() )
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

	tenderbatchprocessing::useCase::MainUCPtr mainUC = tenderbatchprocessing::useCase::MainUCPtr( new tenderbatchprocessing::useCase::MainUC );
	mainUC->injectCmdLineParameterGetter( shared_from_this() );
	mainUC->injectMailAlert(getMailAlert());

    mainUC->injectTenderCollectionDM( getLIBtenderApplCompMgr()->getTenderCollectionDM() );
	mainUC->injectSuccessorTenderCollectionDM( getLIBtenderApplCompMgr()->getTenderCollectionDM() );
	
	basar::I18nString omgDuration;
	m_ParameterGateway->getParameterValue(getSession()->getUser()->getAreaID(), "OMG_DURATION", omgDuration);

	mainUC->setOMGDuration(omgDuration.stoi());

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
	libtender::domMod::cscorder::CscOrderDMPtr p = libtender::domMod::cscorder::CscOrderDMPtr(new libtender::domMod::cscorder::CscOrderDM(LoggerPool::getLoggerTenderbatchprocessing()));
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

basar::db::aspect::ConnectionRef ApplCompMgr::getCICSConnection()
{
	METHODNAME_DEF( ApplCompMgr, getCICSConnection )
	BLOG_TRACE_METHOD( m_Logger, fun );

	basar::db::sql::DatabaseInfo cicsInfo;
	cicsInfo.setCICS( "BasarDefault_west", "10.242.5.212", "3106", "TCAS" );

								// name
								  //const VarString & ipAddr,			//!< CICS host ip address
								  //const VarString & port,			//!< CICS host port
								  //const VarString & startTransact,	//!< CICS start transaction
								  //const VarString & usr = "",		//!< CICS user 
								  //const VarString & pwd = ""		//!< CICS user's password 

	m_CICSConnection = basar::db::aspect::Manager::getInstance().createConnect( cicsInfo );
	return m_CICSConnection;
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


tenderbatchprocessing::domMod::omgcalc::IOMGCalcPtr ApplCompMgr::getOMGCalcDM()
{
    METHODNAME_DEF( ApplCompMgr, getOMGCalcDM() )
    BLOG_TRACE_METHOD( m_Logger, fun );

    tenderbatchprocessing::domMod::omgcalc::OMGCalcDMPtr omgCalc ( new tenderbatchprocessing::domMod::omgcalc::OMGCalcDM() );

    omgCalc->injectOMGCalcAccessor( getOMGCalcAccessor() );

    return omgCalc;
}

tenderbatchprocessing::domMod::omgcustomercalc::IOMGCustomerCalcPtr ApplCompMgr::getOMGCustomerCalcDM()
{
    METHODNAME_DEF( ApplCompMgr, getOMGCustomerCalcDM() )
    BLOG_TRACE_METHOD( m_Logger, fun );

    tenderbatchprocessing::domMod::omgcustomercalc::OMGCustomerCalcDMPtr omgCustomerCalc ( new tenderbatchprocessing::domMod::omgcustomercalc::OMGCustomerCalcDM() );

    omgCustomerCalc->injectOMGCalcAccessor( getOMGCustomerCalcAccessor() );

    return omgCustomerCalc;
}

tenderbatchprocessing::domMod::omgsuppliercalc::IOMGSupplierCalcPtr ApplCompMgr::getOMGSupplierCalcDM()
{
    METHODNAME_DEF( ApplCompMgr, getOMGSupplierCalcDM() )
    BLOG_TRACE_METHOD( m_Logger, fun );

    tenderbatchprocessing::domMod::omgsuppliercalc::OMGSupplierCalcDMPtr omgSupplierCalc ( new tenderbatchprocessing::domMod::omgsuppliercalc::OMGSupplierCalcDM() );

    omgSupplierCalc->injectOMGCalcAccessor( getOMGSupplierCalcAccessor() );

    return omgSupplierCalc;
}

libutil::useCase::IRunnablePtr ApplCompMgr::getOMGCalcUC()
{
    METHODNAME_DEF( ApplCompMgr, getOMGCalcUC )
    BLOG_TRACE_METHOD( m_Logger, fun );

    tenderbatchprocessing::useCase::OMGCalcUCPtr omgCal (  new tenderbatchprocessing::useCase::OMGCalcUC );
    omgCal->injectOMGCalcDM( getOMGCalcDM() );
    omgCal->injectTenderCollectionDM( getLIBtenderApplCompMgr()->getTenderCollectionDM() );
    omgCal->injectTenderProgressDM( getLIBtenderApplCompMgr()->getTenderProgressDM() );

    return omgCal;
}

tenderbatchprocessing::useCase::IOMGCalcBaseUCPtr ApplCompMgr::getOMGCustomerCalcUC()
{
    METHODNAME_DEF( ApplCompMgr, getOMGCustomerCalcUC )
    BLOG_TRACE_METHOD( m_Logger, fun );

    tenderbatchprocessing::useCase::OMGCustomerCalcUCPtr omgCustCalcUC (  new tenderbatchprocessing::useCase::OMGCustomerCalcUC );
    omgCustCalcUC->injectOMGCustomerCalcDM( getOMGCustomerCalcDM() );
    omgCustCalcUC->injectTenderCollectionDM( getLIBtenderApplCompMgr()->getTenderCollectionDM() );
    omgCustCalcUC->injectTenderProgressDM( getLIBtenderApplCompMgr()->getTenderProgressDM() );

    return omgCustCalcUC;
}

tenderbatchprocessing::useCase::IOMGCalcBaseUCPtr ApplCompMgr::getOMGSupplierCalcUC()
{
    METHODNAME_DEF( ApplCompMgr, getOMGSupplierCalcUC )
    BLOG_TRACE_METHOD( m_Logger, fun );

    tenderbatchprocessing::useCase::OMGSupplierCalcUCPtr omgSupplierCalcUC (  new tenderbatchprocessing::useCase::OMGSupplierCalcUC );
    omgSupplierCalcUC->injectOMGSupplierCalcDM( getOMGSupplierCalcDM() );
    omgSupplierCalcUC->injectTenderCollectionDM( getLIBtenderApplCompMgr()->getTenderCollectionDM() );
    omgSupplierCalcUC->injectTenderProgressDM( getLIBtenderApplCompMgr()->getTenderProgressDM() );

    return omgSupplierCalcUC;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getOMGCalcAccessor()
{
    METHODNAME_DEF( ApplCompMgr, getOMGCalcAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );

    tenderbatchprocessing::infrastructure::accessor::omgcalc::SelectOMGCalcById();
    libutil::infrastructure::accessor::IAccessorPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
        new libutil::infrastructure::accessor::AccessorProxy( tenderbatchprocessing::infrastructure::accessor::omgcalc::lit::OMGCALC_ACC, getDBConnection() ));
    return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getOMGCustomerCalcAccessor()
{
    METHODNAME_DEF( ApplCompMgr, getOMGCalcAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );

    tenderbatchprocessing::infrastructure::accessor::omgcustomercalc::SelectOMGCustomerCalcById();
    libutil::infrastructure::accessor::IAccessorPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
        new libutil::infrastructure::accessor::AccessorProxy( tenderbatchprocessing::infrastructure::accessor::omgcustomercalc::lit::OMGCUSTOMERCALC_ACC, getDBConnection() ));
    return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getOMGSupplierCalcAccessor()
{
    METHODNAME_DEF( ApplCompMgr, getOMGCalcAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );

    tenderbatchprocessing::infrastructure::accessor::omgsuppliercalc::SelectOMGCalcForTransfer();
    libutil::infrastructure::accessor::IAccessorPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
        new libutil::infrastructure::accessor::AccessorProxy( tenderbatchprocessing::infrastructure::accessor::omgsuppliercalc::lit::OMGSUPPLIERCALC_ACC, getDBConnection() ));
    return accessor;
}

libutil::useCase::IRunnablePtr ApplCompMgr::getOMGTransferUC()
{
    METHODNAME_DEF( ApplCompMgr, getOMGTransferUC )
    BLOG_TRACE_METHOD( m_Logger, fun );

    tenderbatchprocessing::useCase::OMGTransferUCPtr omgTrfUC ( new tenderbatchprocessing::useCase::OMGTransferUC );
    omgTrfUC->injectOMGTransferDM( getOMGTransferDM());
    omgTrfUC->injectOMGSupplierCalcDM( getOMGSupplierCalcDM() );
    return omgTrfUC;
}

tenderbatchprocessing::domMod::omgtransfer::IOMGTransferPtr ApplCompMgr::getOMGTransferDM()
{
    METHODNAME_DEF( ApplCompMgr, getOMGTransferDM() )
    BLOG_TRACE_METHOD( m_Logger, fun );

    tenderbatchprocessing::domMod::omgtransfer::OMGTransferDMPtr omgTransfer ( new tenderbatchprocessing::domMod::omgtransfer::OMGTransferDM() );
    omgTransfer->injectRemoteProcedureAccessor( getRemoteProcedureAccessor() );
    return omgTransfer;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getOMGTransferAccessor()
{
    METHODNAME_DEF( ApplCompMgr, getOMGTransferAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );

    tenderbatchprocessing::infrastructure::accessor::omgtransfer::SelectOMGTransferById();
    libutil::infrastructure::accessor::IAccessorPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
        new libutil::infrastructure::accessor::AccessorProxy( tenderbatchprocessing::infrastructure::accessor::omgtransfer::lit::OMGTRANSFER_ACC, getDBConnection() ));
    return accessor;
}

libutil::infrastructure::accessor::IAccessorPtr ApplCompMgr::getRemoteProcedureAccessor()
{
    METHODNAME_DEF( ApplCompMgr, getRemoteProcedureAccessor )
    BLOG_TRACE_METHOD( m_Logger, fun );

    tenderbatchprocessing::infrastructure::accessor::remoteProcedure::CallRemoteProcedure();
    libutil::infrastructure::accessor::IAccessorPtr accessor = libutil::infrastructure::accessor::AccessorProxyPtr(
        new libutil::infrastructure::accessor::AccessorProxy( tenderbatchprocessing::infrastructure::accessor::remoteProcedure::lit::REMOTE_PROCEDURE_ACC, getCICSConnection() ));
    return accessor;
}

} // end namnespace componentManager
} // end namnespace tenderbatchprocessing
