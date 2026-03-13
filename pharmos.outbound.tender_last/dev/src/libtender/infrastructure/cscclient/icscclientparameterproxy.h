#ifndef GUARD_IPARAMETERPROXY_H
#define GUARD_IPARAMETERPROXY_H

#include <libbasar_definitions.h>

namespace libtender {
namespace infrastructure {
namespace cscclient {

class ICscClientParameterProxy
{
public:
	virtual ~ICscClientParameterProxy() {};

	virtual const basar::VarString&	getCscServer() const = 0;
	virtual basar::Int32			getCscPort() const = 0;
	virtual const basar::VarString&	getCscDeviceName() const = 0;
	virtual basar::Int32			getCscUID() const = 0;

	virtual const basar::VarString& getCscOrderType() const = 0;
	virtual const basar::VarString& getCscOrderPickingType() const = 0;
	virtual const basar::VarString& getCscOrderBookingType() const = 0;

};

} // namespace cscclient
} // namespace infrastructure
} // namespace libtender

#endif // GUARD_IPARAMETERPROXY_H
