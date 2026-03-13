#include "cscsession.h"

#include "definitions_uadm.h"

namespace CSC_Batches
{
	namespace infrastructure
	{
		namespace session
		{
			CSCSession::CSCSession() : m_Device("CSCBATCH"), m_Port(-1)
			{
			}

			CSCSession::~CSCSession()
			{
			}

			void CSCSession::injectParameterGateway(libutil::infrastructure::parameter::IParameterGatewayPtr parameter)
			{
				m_Parameter = parameter;
			}

			void CSCSession::injectSession(libutil::infrastructure::session::ISessionPtr session)
			{
				m_Session = session;
			}

			const basar::I18nString& CSCSession::getDevice() const
			{
				return m_Device;
			}

			const basar::I18nString& CSCSession::getServer()
			{
				if (m_Server.empty())
				{
					const basar::I18nString& userName = m_Session->getUser()->getUserName();

					basar::I18nString branchNo; 
					branchNo.itos(m_Session->getUser()->getAreaID());

					m_Parameter->getParameterValue(branchNo, userName, UADM_KSCSERVER, m_Server);
				}

				return m_Server;
			}

			basar::Int32 CSCSession::getPort()
			{
				if (m_Port < 0)
				{
					const basar::I18nString& userName = m_Session->getUser()->getUserName();
					
					basar::I18nString branchNo; 					
					branchNo.itos(m_Session->getUser()->getAreaID());
					
					basar::I18nString port;
					m_Parameter->getParameterValue(branchNo, userName, UADM_KSCPORT, port);

					m_Port = port.stoi();
				}

				return m_Port;
			}
		} // end namespace session
	} // end namespace infrastructure
} // end namespace CSC_Batches
