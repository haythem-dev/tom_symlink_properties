#ifndef GUARD_INFRASTRUCTURE_MAILALERT_H
#define GUARD_INFRASTRUCTURE_MAILALERT_H

//-----------------------------------------------------------------------------------------

#include "imailalert.h"
#include "../libbatch/useradminmailconfigurationptr.h"
#include <libbasar_definitions.h>
#include <libbasarcmnutil_bstring.h>

//-----------------------------------------------------------------------------------------

namespace log4cplus {
	class Logger;
}

//-----------------------------------------------------------------------------------------

namespace abbaUW {
namespace infrastructure {
namespace mail {

//-----------------------------------------------------------------------------------------

class MailAlert : public IMailAlert
{
public:
	MailAlert(const log4cplus::Logger& logger);
	virtual ~MailAlert();

	void injectUserAdminMailConfiguration(libBatch::userAdmin::UserAdminMailConfigurationPtr paramProxy);

	virtual bool send(const basar::VarString&, const basar::VarString&);

private:
	MailAlert( const MailAlert& );
	MailAlert& operator=( const MailAlert& );

	const basar::VarString&			getMailServer();
	basar::Int32					getMailPort();
	const basar::VarString&			getMailSender();
	basar::cmnutil::CollBString&	getMailRecipients();

	const log4cplus::Logger&		getLogger();

	const log4cplus::Logger&		m_Logger;
	libBatch::userAdmin::UserAdminMailConfigurationPtr	m_ParamProxy;
};

//-----------------------------------------------------------------------------------------

} // namespace mail
} // namespace infrastructure
} // namespace abbaUW

//-----------------------------------------------------------------------------------------

#endif // GUARD_INFRASTRUCTURE_MAILALERT_H

//-----------------------------------------------------------------------------------------
