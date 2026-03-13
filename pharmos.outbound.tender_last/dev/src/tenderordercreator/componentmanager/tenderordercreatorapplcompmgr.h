//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief
 *  \author Bjoern Bischof
 *  \date   08.07.2013
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_TENDERORDERCREATOR_COMPONENTMANAGER_TENDERORDERCREATORAPPLCOMPMGR_H
#define GUARD_TENDERORDERCREATOR_COMPONENTMANAGER_TENDERORDERCREATORAPPLCOMPMGR_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libutil/irunnableptr.h>

// uc
#include "iusecasegetter.h"
#include "domainmodule/cscorder/icscorderptr.h"
// libtender
#include <infrastructure/libtenderapplcompmgrptr.h>

#include "infrastructure/cscclient/icscclientconnectionptr.h"
#include "infrastructure/mail/imailalertptr.h"
#include "infrastructure/purchasedbconn/ipurchasedbparameterproxyptr.h"
#include "icmdlineparametergetter.h"
#include "tenderordercreatorcmelinearguments.h"

// libutil
#include <libutil/accessor.h>
#include <libutil/applcomponent.h>
#include <boost/enable_shared_from_this.hpp>

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class ConnectionRef;
        }
    }
}

namespace log4cplus
{
    class Logger;
}

//-------------------------------------------------------------------------------------------------//
// class declaration setcion
//-------------------------------------------------------------------------------------------------//
namespace tenderordercreator {
namespace componentManager {

//-------------------------------------------------------------------------------------------------//

class ApplCompMgr : public libutil::componentManager::ApplCompMgrBase,
	public IUseCaseGetter,
	public infrastructure::ICmdLineParameterGetter,
	public boost::enable_shared_from_this< ApplCompMgr >
{
public:
	ApplCompMgr();
	virtual ~ApplCompMgr();

	void setCmdLineArguments( const infrastructure::cmdLine::AppArgs& appArgs );
	void injectCscClientConnection(	libtender::infrastructure::cscclient::ICscClientConnectionPtr );
	void injectMailAlert(infrastructure::mail::IMailAlertPtr );
	void injectPurchaseDBParameterProxy( libtender::infrastructure::db::IPurchaseDBParameterProxyPtr );
	void injectParameterGateway( libutil::infrastructure::parameter::IParameterGatewayPtr );

	/////////////////////////////////////////////////////////////////////
	// ICmdLineParameterGetter
	/////////////////////////////////////////////////////////////////////
	virtual basar::VarString	getCountry() const;
 	virtual basar::Date			getDateFrom() const;
	virtual basar::Date			getDateTo() const;
	virtual basar::Int16		getNextDays() const;

	/////////////////////////////////////////////////////////////////////
	// use case
	/////////////////////////////////////////////////////////////////////
	virtual libutil::useCase::IRunnablePtr	getMainUC();

	ApplCompMgr( const ApplCompMgr&);
	ApplCompMgr& operator=( const ApplCompMgr&);

	virtual void	doClear();

	/////////////////////////////////////////////////////////////////////
	// accessors
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	// dms
	/////////////////////////////////////////////////////////////////////
	libtender::domMod::cscorder::ICscOrderPtr			getCscOrderDM();

	/////////////////////////////////////////////////////////////////////
	// others
	/////////////////////////////////////////////////////////////////////
	infrastructure::mail::IMailAlertPtr					getMailAlert();
	/////////////////////////////////////////////////////////////////////
	// sub component manager(s)
	/////////////////////////////////////////////////////////////////////
	libtender::componentManager::ApplCompMgrPtr			getLIBtenderApplCompMgr();

	/////////////////////////////////////////////////////////////////////
	// db connection
	/////////////////////////////////////////////////////////////////////
	basar::db::aspect::ConnectionRef								getDBConnection();
	libtender::infrastructure::cscclient::ICscClientConnectionPtr	getCscClientConnection();

	///////////////////////////////////////////////////////////////////
	// member variables
	///////////////////////////////////////////////////////////////////
	const log4cplus::Logger&										m_Logger;
	libtender::componentManager::ApplCompMgrPtr						m_LIBtenderApplCompMgr;
	infrastructure::cmdLine::AppArgs								m_AppArgs;
	libtender::infrastructure::cscclient::ICscClientConnectionPtr	m_CscClientConnection;
	infrastructure::mail::IMailAlertPtr								m_MailAlert;

	libutil::infrastructure::parameter::IParameterGatewayPtr		m_ParameterGateway;
};

    } // end namnespace componentManager
} // end namnespace tenderordercreator

#endif
