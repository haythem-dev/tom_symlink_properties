#include "cscuseradminlogin.h"

#include <libutil/util.h>
#include <libutil/log4cplus.h>
#include <libutil/parameter.h>

#include <libbasarlogin_manager.h>

#include "definitions_uadm.h"
#include "libcscbatch/csc_batches_version.h"

namespace CSC_Batches
{
namespace infrastructure
{
namespace login
{
	CSCUserAdminLogin::CSCUserAdminLogin( const libutil::infrastructure::login::UserLoginData& userLoginData )
	: LoginBase( userLoginData )
	{
		METHODNAME_DEF( CSCUserAdminLogin, CSCUserAdminLogin )
		BLOG_TRACE_METHOD( getLogger(), fun );
	}

	CSCUserAdminLogin::~CSCUserAdminLogin()
	{
		METHODNAME_DEF( CSCUserAdminLogin, ~CSCUserAdminLogin )
		BLOG_TRACE_METHOD( getLogger(), fun );
	}

	bool CSCUserAdminLogin::doLogin()
	{
		METHODNAME_DEF( CSCUserAdminLogin, doLogin )
		BLOG_TRACE_METHOD( getLogger(), fun );

		//basar::I18nString applName = m_UserLoginData.getApplName();
        basar::I18nString applName = UADM_LOGIN_APPLNAME;
		basar::I18nString applVersion = CSC_BATCHES_VERSION;
		basar::I18nString applIni = "";
		basar::I18nString applKey = "";
				
		using basar::login::Manager;
		const bool login = Manager::getInstance().interactiveLoginUADM(applName, applVersion, applIni, applKey); // no ini, no registry key, using basar login as before

		//const bool login = Manager::getInstance().interactiveLogin( applName, UADM_USER, UADM_NO_OF_LOGIN_TRIALS );
		if (login == true)
        {
			libutil::infrastructure::login::UserLoginData userLoginData( Manager::getInstance().getUserName(),
				                            Manager::getInstance().getCryptPW(),
				                            applName,
				                            Manager::getInstance().getAreaID(),
				                            Manager::getInstance().getLanguageID(),
											applVersion, true);
            m_UserLoginData = userLoginData;
		}
				
		return login;
	}

	} // end namespace login
} // end namespace infrastructure
} // end namespace CSC_Batches

