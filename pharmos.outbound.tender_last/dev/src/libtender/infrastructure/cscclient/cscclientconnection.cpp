#include "cscclientconnection.h"
#include "icscclientparameterproxy.h"
#include "loggerpool/tenderloggerpool.h"
#include <libutil/misc/metaclassmacro_definitions.h> // libutil
#include <libutil/misc/log_macro_definitions.h>      // libutil


#include <kscclient/articleinfostruct.h>
#include <kscclient/articleinfostructcode.h>
#include <kscclient/orderinfo.h>
//#include <callbackinquiry.h>
//#include <newcallbackcritpos.h>
//#include <newcallback.h>
//#include <callbackreasoninquiry.h>
//#include <callbackdivisioninquiry.h>
#include <kscclient/callksc.h>

namespace libtender {
namespace infrastructure {
namespace cscclient {

CscClientConnection::CscClientConnection(const log4cplus::Logger& logger) :
	m_Logger(logger)
{
}

CscClientConnection::~CscClientConnection()
{
}

void CscClientConnection::injectParameterProxy( ICscClientParameterProxyPtr paramProxy )
{
	m_ParamProxy = paramProxy;
}

const log4cplus::Logger& CscClientConnection::getLogger()
{
	return m_Logger;
}

basar::Int32 CscClientConnection::connect()
{
	METHODNAME_DEF( CSCClientConnection, connect )
	BLOG_TRACE_METHOD( getLogger(), fun );

	orderinfoS info;
	int ret = ::ConnectServerNoIni(
		const_cast<char*>(getCscDeviceName().c_str()),
		getCscUID(),
		getCscPort(),
		const_cast<char*>(getCscServer().c_str()),
		&info);

	std::stringstream ss;
	ss << "connecting to " << getCscServer() <<", port " << getCscPort() << ": retval=" << ret;
	ss << info.LACKREASON << " " << info.TEXT;
	BLOG_INFO( getLogger(), ss.str() );

	return ret;
}

basar::Int32 CscClientConnection::disconnect()
{
	METHODNAME_DEF( CSCClientConnection, disconnect )
	BLOG_TRACE_METHOD( getLogger(), fun );

	orderinfoS info;
	int ret = ::DisconnectServer(&info);

	return ret;
}

const basar::VarString& CscClientConnection::getCscServer()
{
	return m_ParamProxy->getCscServer();
}

basar::Int32 CscClientConnection::getCscPort()
{
	return m_ParamProxy->getCscPort();
}

const basar::VarString& CscClientConnection::getCscDeviceName()
{
	return m_ParamProxy->getCscDeviceName();
}

basar::Int32 CscClientConnection::getCscUID()
{
	return m_ParamProxy->getCscUID();
}

ICscClientParameterProxyPtr CscClientConnection::getParamProxy()
{
	METHODNAME_DEF(CSCClientConnection, getParamProxy)
	BLOG_TRACE_METHOD(getLogger(), fun);
	return m_ParamProxy;
}

}
}
}
