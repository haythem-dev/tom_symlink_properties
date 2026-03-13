#ifndef GUARD_PURCHASEDBPARAMETERPROXY_H
#define GUARD_PURCHASEDBPARAMETERPROXY_H

//-------------------------------------------------------------------------------------------------//

#include "infrastructure/purchasedbconn/ipurchasedbparameterproxy.h"
#include <libutil/infrastructure/parameter/iparametergatewayptr.h>
#include <libbasar_definitions.h>
#include <libbasarcmnutil_i18nstring.h>

//-------------------------------------------------------------------------------------------------//

namespace libtender {
namespace infrastructure {
namespace db {

//-------------------------------------------------------------------------------------------------//

class PurchaseDBParameterProxy : public IPurchaseDBParameterProxy
{
public:
	PurchaseDBParameterProxy();
	virtual ~PurchaseDBParameterProxy();


	void injectParameterGateway(libutil::infrastructure::parameter::IParameterGatewayPtr paramGateway);

	virtual const basar::VarString&			getPurchaseDBServer() const;
	virtual const basar::VarString&			getPurchaseDB() const;

private:
	PurchaseDBParameterProxy(const PurchaseDBParameterProxy&);
	PurchaseDBParameterProxy& operator=(const PurchaseDBParameterProxy&);

	libutil::infrastructure::parameter::IParameterGatewayPtr	m_ParamGateway;

	mutable basar::I18nString									m_PurchaseDBServer;
	mutable basar::I18nString									m_PurchaseDB;
};

//-------------------------------------------------------------------------------------------------//

} // namespace db
} // namespace infrastructure
} // namespace libtender

//-------------------------------------------------------------------------------------------------//

#endif // GUARD_PURCHASEDBPARAMETERPROXY_H

//-------------------------------------------------------------------------------------------------//
