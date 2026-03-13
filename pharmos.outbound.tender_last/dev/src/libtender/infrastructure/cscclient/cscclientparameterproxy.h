#ifndef GUARD_PARAMETERPROXY_H
#define GUARD_PARAMETERPROXY_H

#include "icscclientparameterproxy.h"
#include <libutil/parameter.h>
#include <libbasar_definitions.h>
#include <libbasarcmnutil_i18nstring.h>

namespace libtender {
namespace infrastructure {
namespace cscclient {

class CscClientParameterProxy : public ICscClientParameterProxy
{
public:
	CscClientParameterProxy();
	virtual ~CscClientParameterProxy();


	void injectParameterGateway(libutil::infrastructure::parameter::IParameterGatewayPtr paramGateway);

	virtual const basar::VarString&	getCscServer() const;
	virtual basar::Int32			getCscPort() const;
	virtual const basar::VarString&	getCscDeviceName() const;
	virtual basar::Int32			getCscUID() const;

    virtual const basar::VarString& getCscOrderType() const;
    virtual const basar::VarString& getCscOrderPickingType() const;
    virtual const basar::VarString& getCscOrderBookingType() const;
private:
	CscClientParameterProxy(const CscClientParameterProxy&);
	CscClientParameterProxy& operator=(const CscClientParameterProxy&);

	libutil::infrastructure::parameter::IParameterGatewayPtr m_ParamGateway;

	mutable basar::I18nString	m_CscServer;
	mutable basar::Int32		m_CscPort;
	mutable basar::I18nString	m_CscDeviceName;
	mutable basar::Int32		m_CscUID;
    mutable basar::I18nString	m_CscOrderType;
	mutable basar::I18nString   m_CscOrderPickingType;
	mutable basar::I18nString   m_CscOrderBookingType;
};

} // namespace cscclient
} // namespace infrastructure
} // namespace libtender

#endif // GUARD_PARAMETERPROXY_H