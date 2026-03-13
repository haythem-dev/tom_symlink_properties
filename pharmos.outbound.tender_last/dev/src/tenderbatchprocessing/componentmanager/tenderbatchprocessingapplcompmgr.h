#ifndef GUARD_TENDERBATCHPROCESSING_COMPONENTMANAGER_TENDERBATCHPROCESSINGAPPLCOMPMGR_H
#define GUARD_TENDERBATCHPROCESSING_COMPONENTMANAGER_TENDERBATCHPROCESSINGAPPLCOMPMGR_H

#include <libutil/irunnableptr.h>

// uc
#include "iusecasegetter.h"
#include "domainmodule/cscorder/icscorderptr.h"

//dm
#include "domainmodule/omgtransfer/iomgtransferptr.h"
#include "domainmodule/omgcalc/iomgcalcptr.h"
#include "domainmodule/omgcustomercalc/iomgcustomercalcptr.h"
#include "domainmodule/omgsuppliercalc/iomgsuppliercalcptr.h"

// libtender
#include <infrastructure/libtenderapplcompmgrptr.h>

#include "infrastructure/cscclient/icscclientconnectionptr.h"
#include "infrastructure/mail/imailalertptr.h"
#include "infrastructure/purchasedbconn/ipurchasedbparameterproxyptr.h"
#include "icmdlineparametergetter.h"
#include "tenderbatchprocessingcmelinearguments.h"

// libutil
#include <libutil/accessor.h>
#include <libutil/applcomponent.h>
#include <boost/enable_shared_from_this.hpp>

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

namespace tenderbatchprocessing {
namespace componentManager {

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

	virtual basar::VarString	getCountry() const;
 	virtual basar::Date			getDateFrom() const;
	virtual basar::Date			getDateTo() const;
	virtual basar::Int16		getNextDays() const;
	
	virtual libutil::useCase::IRunnablePtr getMainUC();
    virtual libutil::useCase::IRunnablePtr getOMGCalcUC();
    virtual libutil::useCase::IRunnablePtr getOMGTransferUC();
    virtual tenderbatchprocessing::useCase::IOMGCalcBaseUCPtr getOMGCustomerCalcUC();
    virtual tenderbatchprocessing::useCase::IOMGCalcBaseUCPtr getOMGSupplierCalcUC();

	ApplCompMgr( const ApplCompMgr&);
	ApplCompMgr& operator=( const ApplCompMgr&);

	virtual void	doClear();

	libtender::domMod::cscorder::ICscOrderPtr							getCscOrderDM();
    tenderbatchprocessing::domMod::omgtransfer::IOMGTransferPtr			getOMGTransferDM();
    tenderbatchprocessing::domMod::omgcalc::IOMGCalcPtr					getOMGCalcDM();
    tenderbatchprocessing::domMod::omgcustomercalc::IOMGCustomerCalcPtr	getOMGCustomerCalcDM();
    tenderbatchprocessing::domMod::omgsuppliercalc::IOMGSupplierCalcPtr	getOMGSupplierCalcDM();

	infrastructure::mail::IMailAlertPtr				getMailAlert();

	libtender::componentManager::ApplCompMgrPtr		getLIBtenderApplCompMgr();

	basar::db::aspect::ConnectionRef								getDBConnection();
	basar::db::aspect::ConnectionRef								getCICSConnection();
	libtender::infrastructure::cscclient::ICscClientConnectionPtr	getCscClientConnection();

    libutil::infrastructure::accessor::IAccessorPtr getOMGCalcAccessor();
    libutil::infrastructure::accessor::IAccessorPtr getOMGCustomerCalcAccessor();
    libutil::infrastructure::accessor::IAccessorPtr getOMGSupplierCalcAccessor();
    libutil::infrastructure::accessor::IAccessorPtr getOMGTransferAccessor();
	libutil::infrastructure::accessor::IAccessorPtr getRemoteProcedureAccessor();

	log4cplus::Logger												m_Logger;
	libtender::componentManager::ApplCompMgrPtr						m_LIBtenderApplCompMgr;
	infrastructure::cmdLine::AppArgs								m_AppArgs;
	libtender::infrastructure::cscclient::ICscClientConnectionPtr	m_CscClientConnection;
	basar::db::aspect::ConnectionRef								m_CICSConnection;
	infrastructure::mail::IMailAlertPtr								m_MailAlert;

	libutil::infrastructure::parameter::IParameterGatewayPtr		m_ParameterGateway;
};

} // end namnespace componentManager
} // end namnespace tenderbatchprocessing

#endif
