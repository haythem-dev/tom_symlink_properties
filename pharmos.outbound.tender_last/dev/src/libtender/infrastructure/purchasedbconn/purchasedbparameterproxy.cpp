#include "purchasedbparameterproxy.h"
#include "purchasedbparameterkeys.h"
#include <libutil/infrastructure/parameter/iparametergateway.h> // libutil
#include <libbasarlogin.h>

//-------------------------------------------------------------------------------------------------//

namespace libtender {
namespace infrastructure {
namespace db {

//-------------------------------------------------------------------------------------------------//

PurchaseDBParameterProxy::~PurchaseDBParameterProxy()
{
}

//-------------------------------------------------------------------------------------------------//

PurchaseDBParameterProxy::PurchaseDBParameterProxy() :
	m_PurchaseDBServer(),
	m_PurchaseDB()
{
}

//-------------------------------------------------------------------------------------------------//

void PurchaseDBParameterProxy::injectParameterGateway(libutil::infrastructure::parameter::IParameterGatewayPtr paramGateway)
{
	m_ParamGateway = paramGateway;
}

//-------------------------------------------------------------------------------------------------//

const basar::VarString& PurchaseDBParameterProxy::getPurchaseDBServer() const
{
	if (m_PurchaseDBServer.empty())
	{
		basar::I18nString areaID;
		areaID.itos(basar::login::Manager::getInstance().getAreaID());
		if (!m_ParamGateway->getParameterValue(areaID, constants::UADM_PURCHASE_DB_SERVER_KEY, m_PurchaseDBServer))
		{
			m_ParamGateway->getParameterValue(constants::UADM_PURCHASE_DB_SERVER_KEY, m_PurchaseDBServer);
		}
	}
	return m_PurchaseDBServer;
}

//-------------------------------------------------------------------------------------------------//

const basar::VarString& PurchaseDBParameterProxy::getPurchaseDB() const
{
	if (m_PurchaseDB.empty() )
	{
		basar::I18nString areaID;
		areaID.itos(basar::login::Manager::getInstance().getAreaID());
		if (!m_ParamGateway->getParameterValue(areaID, constants::UADM_PURCHASE_DB_KEY, m_PurchaseDB))
		{
			m_ParamGateway->getParameterValue(constants::UADM_PURCHASE_DB_KEY, m_PurchaseDB);
		}
	}
	return m_PurchaseDB;
}

//-------------------------------------------------------------------------------------------------//

} // namespace db
} // namespace infrastructure
} // namespace libtender

//-------------------------------------------------------------------------------------------------//
