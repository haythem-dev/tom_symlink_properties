#ifndef GUARD_INFRASTRUCTURE_CSCCLIENTCONNECTION_H
#define GUARD_INFRASTRUCTURE_CSCCLIENTCONNECTION_H

#include "icscclientconnection.h"
#include "icscclientparameterproxyptr.h"
#include <libbasar_definitions.h>

//-----------------------------------------------------------------------------------------

namespace log4cplus {
	class Logger;
}

//-----------------------------------------------------------------------------------------

namespace libtender {
namespace infrastructure {
namespace cscclient {

//-----------------------------------------------------------------------------------------

class CscClientConnection : public ICscClientConnection
{
public:
	CscClientConnection(const log4cplus::Logger& logger);
	virtual ~CscClientConnection();

	void injectParameterProxy(ICscClientParameterProxyPtr paramProxy);

	virtual basar::Int32 connect();
	virtual basar::Int32 disconnect();
	virtual ICscClientParameterProxyPtr getParamProxy();

private:
	CscClientConnection( const CscClientConnection& );
	CscClientConnection& operator=( const CscClientConnection& );

	const basar::VarString&		getCscServer();
	basar::Int32				getCscPort();
	const basar::VarString&		getCscDeviceName();
	basar::Int32				getCscUID();

	const log4cplus::Logger&	getLogger();

	const log4cplus::Logger&	m_Logger;
	ICscClientParameterProxyPtr	m_ParamProxy;
};

} // namespace cscclient
} // namespace infrastructure
} // namespace libtender

#endif // GUARD_INFRASTRUCTURE_CSCCLIENTCONNECTION_H