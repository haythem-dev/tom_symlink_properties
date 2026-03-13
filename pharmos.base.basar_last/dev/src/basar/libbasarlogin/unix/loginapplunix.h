//----------------------------------------------------------------------------
/*! \file
 *  \brief  login implementation class for unix (without dialogs)
 *  \author Thomas Hörath
 *  \date   26.02.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LOGINAPPLUNIX_H
#define GUARD_LOGINAPPLUNIX_H

//----------------------------------------------------------------------------
#include "loginappl.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace login	{

//----------------------------------------------------------------------------
//! login implementation class \n final class
class LoginApplUnix : public LoginAppl
{
	friend class LoginAppl;    //!< for construction from LoginAppl::create()

public:
	/*! \brief destructor
		\n	   no-throw */
	virtual ~LoginApplUnix();

	virtual bool interactiveLoginUADM(basar::I18nString& rApplName,     // unique name of app, which has to map to the UADM application name
		basar::I18nString& rApplVersion,  // version which will be shown in the login as user info
		basar::I18nString& rApplINIFile,  // optional if an INI should be used instead of basar.config.xml
		basar::I18nString& rApplKey       // optional if local registry should be used instead of uadm previous login infos
	);

	/*! \brief	start login by calling login-dialog, which is created modal.
		\n		no-throw */
	virtual bool interactiveLogin(basar::I18nString& rApplName,	//!< unique applicationname to start login for
				 const basar::I18nString& rWindowTitle, //!< window title to show in login dialog
				 const basar::Int16 maxTrials			//!< number of login trials
				 );

	/*! \brief  start login without calling login-dialog reduced to standard UADM functionality needed for win apps
		\n      no-throw */
	virtual bool nonInteractiveLoginUADM(basar::I18nString& rApplName,			//!< unique applicationname to start login for
		const basar::I18nString& rUsername,			//!< name of user to login
		const basar::I18nString& rCryptPW,				//!< according password to username
		const basar::I18nString& rCountry,             //!< country to login (can also be empty for compatibility)
		const basar::Int32       rRegionBranchID,		//!< region or branch id to login
		basar::ConstString       rLanguageID = ""		//!< language can be set (e.g. "en" / "de") because no selection without login dialog
	);

	/*! \brief	handle gui-event ok button pressed in login dlg
		\n		no-throw */
	virtual void	handleLogin( bool loginWithADCredentials = false );

	/*! \brief	handle gui-event ok button pressed in change pw dlg
		\n		no-throw */
	virtual void	handlePwChanged();

	/*! \brief	handle gui-event cancel button pressed
		\n		no-throw */
	virtual void	handleCancel();

	/*! \brief	call change password dialog and manage input check
		\n		no-throw */
	virtual bool	managePwChanging();

	/*! \brief	return multilang-text from pptext to reftext
		\n		NoTextFoundException */
	virtual const basar::I18nString	getLabel(basar::I18nString refText //!< reference text to search in label / message
									);


private:
	LoginApplUnix				();								//!< forbidden constructor
	LoginApplUnix				(const LoginApplUnix & rCopy);	//!< forbidden copy constructor
	LoginApplUnix & operator =	(const LoginApplUnix & rCopy);	//!< forbidden assignment operator

	/*! \brief	initialize members
		\n		no-throw */
	virtual void	init		();

	/*! \brief	clear members
		\n		no-throw */
	virtual void	clear		();
};

//----------------------------------------------------------------------------
}	// namespace login
}	// namespace basar

#endif	// GUARD
