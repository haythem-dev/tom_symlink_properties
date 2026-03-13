#include "cscclientparameterproxy.h"
#include "cscclientparameterkeys.h"
#include <libbasarlogin.h>

namespace libtender {
namespace infrastructure {
namespace cscclient {


CscClientParameterProxy::CscClientParameterProxy() :
	m_CscServer(),
	m_CscPort(0),
	m_CscDeviceName(),
	m_CscUID(0)
{
}

CscClientParameterProxy::~CscClientParameterProxy()
{
}

void CscClientParameterProxy::injectParameterGateway( libutil::infrastructure::parameter::IParameterGatewayPtr paramGateway )
{
	m_ParamGateway = paramGateway;
}

const basar::VarString& CscClientParameterProxy::getCscServer() const
{
	if (m_CscServer.empty() )
	{
		basar::I18nString areaID;
		areaID.itos(basar::login::Manager::getInstance().getAreaID());
		if (!m_ParamGateway->getParameterValue(areaID, constants::UADM_CSC_SERVER_KEY, m_CscServer))
		{
			m_ParamGateway->getParameterValue(constants::UADM_CSC_SERVER_KEY, m_CscServer);
		}
	}
	return m_CscServer;
}

basar::Int32 CscClientParameterProxy::getCscPort() const
{
	if (0 == m_CscPort)
	{
		basar::I18nString val;
		basar::I18nString areaID;
		areaID.itos(basar::login::Manager::getInstance().getAreaID());
		if (!m_ParamGateway->getParameterValue(areaID, constants::UADM_CSC_PORT_KEY, val))
		{
			m_ParamGateway->getParameterValue(constants::UADM_CSC_PORT_KEY, val);
		}
		m_CscPort = val.stoi();
	}
	return m_CscPort;
}

const basar::VarString& CscClientParameterProxy::getCscDeviceName() const
{
	if (m_CscDeviceName.empty() )
	{
		basar::I18nString areaID;
		areaID.itos(basar::login::Manager::getInstance().getAreaID());
		if (!m_ParamGateway->getParameterValue(areaID, constants::UADM_CSC_DEVICENAME_KEY, m_CscDeviceName))
		{
			m_ParamGateway->getParameterValue(constants::UADM_CSC_DEVICENAME_KEY, m_CscDeviceName);
		}
	}
	return m_CscDeviceName;
}

basar::Int32 CscClientParameterProxy::getCscUID() const
{
	if (0 == m_CscUID)
	{
		basar::I18nString val;
		basar::I18nString areaID;
		areaID.itos(basar::login::Manager::getInstance().getAreaID());
		if (!m_ParamGateway->getParameterValue(areaID, constants::UADM_CSC_UID_KEY, val))
		{
			m_ParamGateway->getParameterValue(constants::UADM_CSC_UID_KEY, val);
		}
		m_CscUID = val.stoi();
	}
	return m_CscUID;
}

const basar::VarString& CscClientParameterProxy::getCscOrderType() const
{
    if( m_CscOrderType.empty() )
    {
        basar::I18nString areaID;
        areaID.itos( basar::login::Manager::getInstance().getAreaID() );
        if( false == m_ParamGateway->getParameterValue( areaID, constants::UADM_CSC_ORDER_TYPE, m_CscOrderType ) )
        {
            m_ParamGateway->getParameterValue( constants::UADM_CSC_ORDER_TYPE, m_CscOrderType );
        }
    }
    return m_CscOrderType;
}

const basar::VarString& CscClientParameterProxy::getCscOrderPickingType() const
{
    if( m_CscOrderPickingType.empty() )
    {
        basar::I18nString areaID;
        areaID.itos( basar::login::Manager::getInstance().getAreaID() );
        if( false == m_ParamGateway->getParameterValue( areaID, constants::UADM_CSC_ORDER_PICKING_TYPE, m_CscOrderPickingType ) )
        {
            m_ParamGateway->getParameterValue( constants::UADM_CSC_ORDER_PICKING_TYPE, m_CscOrderPickingType );
        }
    }
    return m_CscOrderPickingType;
}

const basar::VarString& CscClientParameterProxy::getCscOrderBookingType() const
{
    if( m_CscOrderBookingType.empty() )
    {
        basar::I18nString areaID;
        areaID.itos( basar::login::Manager::getInstance().getAreaID() );
        if( false == m_ParamGateway->getParameterValue( areaID, constants::UADM_CSC_ORDER_BOOKING_TYPE, m_CscOrderBookingType ) )
        {
            m_ParamGateway->getParameterValue(constants::UADM_CSC_ORDER_BOOKING_TYPE, m_CscOrderBookingType );
        }
    }
    return m_CscOrderBookingType;
}

} // namespace cscclient
} // namespace infrastructure
} // namespace libtender
