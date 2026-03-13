#include "mailalert.h"
#include "loggerpool.h"
#include "../../loggerpool.h"
#include <libbasarcmnutil_email.h>
#include <libutil/misc/metaclassmacro_definitions.h> // libutil
#include <libutil/misc/log_macro_definitions.h>      // libutil
#include <../libbatch/useradminmailconfiguration.h>
#include <../libbatch/useradminmailconfigurationptr.h>

//-----------------------------------------------------------------------------------------

namespace abbaUW {
	namespace infrastructure {
		namespace mail {

			//-----------------------------------------------------------------------------------------

			MailAlert::MailAlert(const log4cplus::Logger& logger) :
				m_Logger(logger)
			{
			}

			//-----------------------------------------------------------------------------------------

			MailAlert::~MailAlert()
			{
			}

			//-----------------------------------------------------------------------------------------

			void MailAlert::injectUserAdminMailConfiguration(libBatch::userAdmin::UserAdminMailConfigurationPtr paramProxy)
			{
				m_ParamProxy = paramProxy;
			}

			//-----------------------------------------------------------------------------------------

			const log4cplus::Logger& MailAlert::getLogger()
			{
				return m_Logger;
			}

			//-----------------------------------------------------------------------------------------

			bool MailAlert::send(const basar::VarString& subject, const basar::VarString& body)
			{
				METHODNAME_DEF(MailAlert, send)
				BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun);

				basar::cmnutil::Smtp		smtpServer(getMailServer(), static_cast<basar::Int16>(getMailPort()));   // todo parameter!!!
				basar::cmnutil::MailMessage	mail;

				mail.setContentType(basar::cmnutil::MailMessage::TEXT_PLAIN);
				mail.setFrom(getMailSender());
				basar::cmnutil::CollBString::const_iterator it = getMailRecipients().begin();

				while (it != getMailRecipients().end())
				{
					mail.setTo(*it);
					++it;
				}

#ifdef _MSC_VER
// needs to be done because of  std::getenv (This function or variable may be unsafe. Consider using _dupenv_s instead....)
#pragma warning (push)
#pragma warning(disable: 4996 )
#endif

				// try to add the servername to the subject
				try
				{
					basar::VarString text = "";
					text += std::getenv("HOSTNAME");
					text += ": " + subject;

					mail.setSubject(text);
				}
				catch (...)
				{
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun);
					mail.setSubject(subject);
				}// catch
			
				// try to append the servername at the end of the body
				try
				{
					basar::VarString text = body + "\n\n" + "Host: " + std::getenv("HOSTNAME");
					mail.setBody(text);
				}
				catch (...) 
				{
					BLOG_ERROR(abbaUW::LoggerPool::LoggerAbbaUW, fun);
					mail.setBody(body);
				}// catch

#ifdef _MSC_VER
#pragma warning (pop)
#endif

				basar::cmnutil::Smtp::RetCodeEnum retval = smtpServer.connect();
				if (retval == basar::cmnutil::Smtp::SMTP_SUCCESS)
				{
					retval = smtpServer.send(mail);
					smtpServer.disconnect();
				}
				return (basar::cmnutil::Smtp::SMTP_SUCCESS == retval);
			}

			//-----------------------------------------------------------------------------------------

			const basar::VarString& MailAlert::getMailServer()
			{
				return m_ParamProxy->getUserMailConfiguration().m_MailServer;
			}

			//-----------------------------------------------------------------------------------------

			basar::Int32 MailAlert::getMailPort()
			{
				return m_ParamProxy->getUserMailConfiguration().m_MailPort.stoi();
			}

			//-----------------------------------------------------------------------------------------

			const basar::VarString& MailAlert::getMailSender()
			{
				return m_ParamProxy->getUserMailConfiguration().m_MailSender;
			}

			//-----------------------------------------------------------------------------------------

			basar::cmnutil::CollBString& MailAlert::getMailRecipients()
			{
				return m_ParamProxy->getUserMailConfiguration().m_MailRecepients;
			}

			//-----------------------------------------------------------------------------------------

		} // namespace mail
	} // namespace infrastructure
} // namespace abbaUW

//-----------------------------------------------------------------------------------------
