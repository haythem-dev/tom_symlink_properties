#ifndef GUARD_INFRASTRUCTURE_ICSCCLIENTCONNECTION_H
#define GUARD_INFRASTRUCTURE_ICSCCLIENTCONNECTION_H

#include <libbasar_definitions.h>
#include "icscclientparameterproxyptr.h"

namespace libtender {
namespace infrastructure {
namespace cscclient {

class ICscClientConnection
{
public:
	virtual ~ICscClientConnection() {};

	virtual basar::Int32 connect() = 0;
	virtual basar::Int32 disconnect() = 0;
	virtual ICscClientParameterProxyPtr getParamProxy() = 0;
};

} // namespace libtender
} // namespace infrastructure
} // namespace tenderordercreator

#endif // GUARD_INFRASTRUCTURE_ICSCCLIENTCONNECTION_H
