#ifndef GUARD_CSCBATCHES_INFRASTRUCTURE_SESSION_CSCSESSION_H
#define GUARD_CSCBATCHES_INFRASTRUCTURE_SESSION_CSCSESSION_H

#include "icscsession.h"

#include <libutil/parameter.h>
#include <libutil/session.h>

#include <libbasarcmnutil_i18nstring.h>
#include <libbasar_definitions.h>

namespace CSC_Batches
{
	namespace infrastructure
	{
		namespace session
		{
			class CSCSession : public ICSCSession
			{
			public:
				CSCSession();
				~CSCSession();

				void                        injectParameterGateway(libutil::infrastructure::parameter::IParameterGatewayPtr);
				void                        injectSession(libutil::infrastructure::session::ISessionPtr);

				// from ICSCSession interface
				const basar::I18nString& getDevice() const;
				const basar::I18nString& getServer();
				basar::Int32                getPort();

			private:
				// forbidden
				CSCSession(const CSCSession&);
				CSCSession& operator = (const CSCSession&);

			private:
				basar::I18nString                                        m_Device;
				basar::I18nString                                        m_Server;
				basar::Int32                                             m_Port;
				libutil::infrastructure::parameter::IParameterGatewayPtr m_Parameter;
				libutil::infrastructure::session::ISessionPtr            m_Session;
			};

		} // end namespace session
	} // end namespace infrastructure
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_INFRASTRUCTURE_SESSION_CSCSESSION_H
