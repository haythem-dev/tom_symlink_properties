/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH				                             */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"

#include <BConnect.h>
#include <CConnect.h>

#include "PViewMain.h"
#include "PDlgAbout.h"
#include "PDlgUserList.h"
#include "PDlgGroupList.h"
#include "PDlgAdmRights.h"
#include "PDlgExpRights.h"
#include "PDlgDllAppModRi.h"
#include "PDlgDllBrReg.h"
#include "PDlgDllGroup.h"
#include "PDlgDllUsertype.h"
#include "PDlgDllUserPara.h"
#include "PDlgDllLangText.h"
#include "PDlgDllText.h"
#include "PDlgDllPara.h"
#include "PDlgDllPrev.h"
#include "PDlgRiAnalysis.h"
#include "PDlgRiAnalog.h"
#include "PDlgExpressCorUsr.h"
#include "PDlgUsrAnalysis.h"
#include "PDlgUsrRightAnalysis.h"
#include "PDlgUserManager.h"

#include "PMainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*----------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(PMainFrm, CFrameWndMultiLang)

/**---------------------------------------------------------------------------*/
void PMainFrm::DoDataExchange(CDataExchange* pDX)
{
	CFrameWndMultiLang::DoDataExchange(pDX);
}


/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PMainFrm, CFrameWndMultiLang)
	ON_WM_CREATE()
	ON_COMMAND(ID_INFO_DOKU,	OnInfoDoku)
	ON_COMMAND(ID_INFO_ABOUT,	OnInfoAbout)
	ON_COMMAND(ID_ADM_USER,		OnAdmUser)
	ON_COMMAND(ID_ADM_GROUP,	OnAdmGroup)
	ON_COMMAND(ID_DLL_DOKU,		OnDllDoku)
	ON_COMMAND(ID_ADM_USER_RIGHTS, OnAdmUserRights)
	ON_COMMAND(ID_ADM_GROUP_RIGHTS, OnAdmGroupRights)
	ON_COMMAND(ID_DBKONFIGURIEREN_APPLIKATION, OnDLLAppModRi)
	ON_COMMAND(ID_DBKONFIGURIEREN_FILIALEN, OnDllBranchReg)
	ON_COMMAND(ID_DBKONFIGURIEREN_GRUPPEN, OnDbconfigGroup)
	ON_COMMAND(ID_DBKONFIGURIEREN_TEXTE, OnDbkonfigurierenTexte)
	ON_COMMAND(ID_DBKONFIGURIEREN_LABEL, OnDbkonfigurierenLabel)
	ON_COMMAND(ID_DBCONFIG_USERTYPES, OnUsertypes)
	ON_COMMAND(ID_DBCONFIG_USERPARAMS, OnUserParameters)
	ON_COMMAND(ID_DBCONFIG_APPPARAMS, OnAppParameters)
	ON_COMMAND(ID_DBKONFIGURIEREN_PREVIOUSLOGIN, OnDbkonfigurierenPreviouslogin)
	ON_COMMAND(ID_CHANGE_HOST, OnChangeHost)
	ON_COMMAND(ID_BENUTZER_RECHTE, OnBenutzerRechteAnalyse)
	ON_COMMAND(ID_EXP_USR_RIGHTS,	OnExpUsrRights)
	ON_COMMAND(ID_EXPRESS_GRP_RIGHTS,	OnExpGrpRights)
	ON_COMMAND(ID_EXPRESS_ANALOG, OnExpAnalog)
	ON_COMMAND(ID_EXPRESS_COR_USER, OnExpCorUser)
	ON_COMMAND(ID_ANALYSE_USER, OnUserAnalyse)
	ON_COMMAND(ID_USER_MANAGER, OnUserManager)
	
    ON_COMMAND(ID_ANALYSE_USERRIGHTS, &PMainFrm::OnAnalyseUserrights)
END_MESSAGE_MAP()

/*----------------------------------------------------------------------------*/
UINT PMainFrm::m_uiIndicators[] =
{
	ID_SEPARATOR,						// Statusleistenanzeige
    ID_INDICATOR_USERNAME,              // User-Kennung
    ID_INDICATOR_CONNECT				// Socket-Verbindung
};

/**---------------------------------------------------------------------------*/
PMainFrm::PMainFrm()
{
	// Set to FALSE so no ON_UPDATE_COMMAND_UI or ON_COMMAND handlers are needed, and
	// CMenu::EnableMenuItem() will work as expected.
	m_bAutoMenuEnable = FALSE;
}

/**---------------------------------------------------------------------------*/
PMainFrm::~PMainFrm()
{
}

/**---------------------------------------------------------------------------*/
int PMainFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	BeginWaitCursor();

	if (CFrameWndMultiLang::OnCreate(lpCreateStruct) == -1)
	{
		EndWaitCursor();

		return -1;
	}

	// StatusBar: VZ setzen abhängig von Anmeldung
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(m_uiIndicators,
		  sizeof(m_uiIndicators)/sizeof(UINT)))
	{
		assert(0);
		EndWaitCursor();
		return -1;
	}

	static bool bLoggedIn = false;

	CString csUser, csConnect;

	if (bLoggedIn == false)
	{
		csConnect = CResString::ex().getStrTblText(IDS_INDICATOR_CONNECT);
		csUser    = CResString::ex().getStrTblText(IDS_INDICATOR_USER   );
		
		bLoggedIn = true;
	}
	else
	{

		csUser   .Format(CResString::ex().getStrTblText(IDS_APP_USER), 
							ppBLoginData::ex().GetUsername().c_str());
		csConnect.Format(CResString::ex().getStrTblText(IDS_APP_CONNECT), 
							BConnect::ex().getServer().c_str(),
							BConnect::ex().getCurDatabase().c_str(),
							CConnect::VOODOO_PORT,
							ppBLoginData::ex().GetLanguageID().c_str());           

	}

	SetConnect(csConnect);
	SetUser   (csUser   );

	CString csChangeHost;
	csChangeHost.Format(CResString::ex().getStrTblText(IDS_CHANGE_TO),CConnect::HOST_BACKUP.c_str());
	GetMenu()->ModifyMenu(ID_CHANGE_HOST, 
		                  MF_BYCOMMAND | MF_STRING, 
						  ID_CHANGE_HOST, 
						  csChangeHost );

	//CONTROLS
	this->SetWindowText(CResString::ex().getStrTblText(IDS_APP_NAME));
	EndWaitCursor();

	return 0;
}


/**---------------------------------------------------------------------------*/
void PMainFrm::GetMessageString(UINT nID, CString& rMessage) const
{
	switch (nID)
	{
	case ID_APP_EXIT:
		rMessage = CResString::ex().getStrTblText(IDS_MSG_APP_EXIT);
		break;
	case ID_DBCONFIG_APPPARAMS:
		rMessage = CResString::ex().getStrTblText(IDS_DBCONFIG_APPPARAMS);
		break;
	case ID_BENUTZER_RECHTE:
		rMessage = CResString::ex().getStrTblText(IDS_ANALY_RIGHTS);
		break;
	case ID_ANALYSE_USER:
		rMessage = CResString::ex().getStrTblText(IDS_ANALY_USER);
		break;
    case ID_ANALYSE_USERRIGHTS:
        rMessage = CResString::ex().getStrTblText(ID_ANALYSE_USERRIGHTS);
        break;
	case ID_DBCONFIG_USERPARAMS:
		rMessage = CResString::ex().getStrTblText(IDS_DBCONFIG_USERPARAMS);
		break;
	case ID_CHANGE_HOST:
		if ( BConnect::ex().getServer() == CConnect::HOST)
		{
			rMessage = CResString::ex().getStrTblText(IDS_MSG_CHANGE_HOST);
			break;
		}
		else
		{
			rMessage = CResString::ex().getStrTblText(IDS_CHANGE_TO_MAINHOST);
			break;
		}
	case ID_INFO_DOKU:
		rMessage = CResString::ex().getStrTblText(IDS_MENUMSG_QUESTION_DOKUMENTATION);
		break;
	case ID_ADM_USER:
		rMessage = CResString::ex().getStrTblText(IDS_ADM_USER);
		break;
	case ID_ADM_GROUP:
		rMessage = CResString::ex().getStrTblText(IDS_ADM_GROUP);
		break;
	case ID_ADM_RIGHTS:
		rMessage = CResString::ex().getStrTblText(IDS_ADM_RIGHTS);
		break;
	case ID_ADM_USER_RIGHTS:
		rMessage = CResString::ex().getStrTblText(IDS_ADM_USER_RIGHTS);
		break;
	case ID_ADM_GROUP_RIGHTS:
		rMessage = CResString::ex().getStrTblText(IDS_ADM_GROUP_RIGHTS);
		break;
	case ID_DB_CONFIG:
		rMessage = CResString::ex().getStrTblText(IDS_DB_CONFIG);
		break;
	case ID_DLL_DOKU:
		rMessage = CResString::ex().getStrTblText(IDS_DLL_DOKU);
		break;
	case ID_DBKONFIGURIEREN_APPLIKATION:
		rMessage = CResString::ex().getStrTblText(IDS_DBKONFIGURIEREN_APPLIKATION);
		break;
	case ID_DBKONFIGURIEREN_FILIALEN:
		rMessage = CResString::ex().getStrTblText(IDS_DBKONFIGURIEREN_FILIALEN);
		break;
	case ID_DBKONFIGURIEREN_GRUPPEN:
		rMessage = CResString::ex().getStrTblText(IDS_DBKONFIGURIEREN_GRUPPEN);
		break;
	case ID_DBCONFIG_USERTYPES:
		rMessage = CResString::ex().getStrTblText(IDS_DBCONFIG_USERTYPES);
		break;
	case ID_DBKONFIGURIEREN_TEXTE:
		rMessage = CResString::ex().getStrTblText(IDS_DBKONFIGURIEREN_TEXTE);
		break;
	case ID_DBKONFIGURIEREN_LABEL:
		rMessage = CResString::ex().getStrTblText(IDS_DBKONFIGURIEREN_LABEL);
		break;
	case ID_DBKONFIGURIEREN_PREVIOUSLOGIN:
		rMessage = CResString::ex().getStrTblText(IDS_DBKONFIGURIEREN_PREVIOUSLOGIN);
		break;
	case ID_INFO_ABOUT:
		rMessage = CResString::ex().getStrTblText(IDS_INFO_ABOUT);
		break;
	case ID_EXP_USR_RIGHTS:
		rMessage = CResString::ex().getStrTblText(IDS_EXP_USR_RIGHTS);
		break;
	case ID_EXPRESS_GRP_RIGHTS:
		rMessage = CResString::ex().getStrTblText(IDS_EXP_GRP_RIGHTS);
		break;
	case ID_EXPRESS_ANALOG:
		rMessage = CResString::ex().getStrTblText(IDS_EXP_ANALOG);
		break;
	case ID_EXPRESS_COR_USER:
		rMessage = CResString::ex().getStrTblText(IDS_EXP_CORUSR);
		break;
	case ID_USER_MANAGER:
		rMessage = CResString::ex().getStrTblText(IDS_USER_MANAGER);
		break;
		
	default:
		CFrameWndMultiLang::GetMessageString(nID, rMessage);
		break;
	}
}

/**---------------------------------------------------------------------------*/
BOOL PMainFrm::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndMultiLang::PreCreateWindow(cs) )
		return FALSE;

	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.
	cs.x          = 0;
	cs.y          = 0;
	cs.cx         = 800;		            // me021106: Resolution für 800 x 600
	cs.cy         = 573;		            //           abzüglich Taskleiste

	return TRUE;
}

/**---------------------------------------------------------------------------*/
void PMainFrm::SetConnect(LPCTSTR spTxt)
{
    m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_CONNECT), 
		                       spTxt, 
							   TRUE);
}

/**---------------------------------------------------------------------------*/
void PMainFrm::SetUser(LPCTSTR spTxt)
{
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_USERNAME), 
		                       spTxt, 
							   TRUE);
}


/**---------------------------------------------------------------------------*/
void PMainFrm::OnInfoDoku()
{
	ShellExecute(NULL,														// hWnd
				 "open",													// operation
				 CResString::ex().getStrTblText(IDS_DOKU_URL),	// file
				 NULL,														// parameters
				 NULL,														// directory
				 SW_SHOWNORMAL);	
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnInfoAbout()
{
	PDlgAbout dlgAbout; 
    dlgAbout.DoModal();
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnAdmUser()
{
	CBString strRight = RIGHT_READ_RI;
	if (ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight))
	{
		PDlgUserList dlgUser;
		dlgUser.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnAdmGroup()
{
	CBString strRight = RIGHT_READ_RI;
	if (ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight))
	{
		PDlgGroupList dlgGroup;
		dlgGroup.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnDLLAppModRi()
{
	CBString strRight = RIGHT_READ_DLL;
	if (ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight))
	{
		PDlgDllAppModRi dlgDLLAppModRi;
		dlgDLLAppModRi.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}	
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnDllBranchReg()
{
	CBString strRight = RIGHT_READ_DLL;
	if (ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight))
	{
		PDlgDllBrReg dlgDLLBrReg;
		dlgDLLBrReg.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnDllDoku()
{
	ShellExecute(NULL,														// hWnd
				 "open",													// operation
				 CResString::ex().getStrTblText(IDS_DOKU_DLL_URL),// file
				 NULL,														// parameters
				 NULL,														// directory
				 SW_SHOWNORMAL);}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnAdmUserRights()
{
	CBString strRight = RIGHT_READ_RI;
	if (ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight))
	{
		bool bGroup = false;
		PDlgAdmRights dlgAdmRights;
		dlgAdmRights.setBGroup(bGroup);
		dlgAdmRights.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnExpUsrRights()
{
	//CBString strRight = RIGHT_READ_RI;
	CBString strRight = RIGHT_UA25_RI;
	if (ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight))
	{
		bool bGroup = false;
		PDlgExpRights dlgExpRights;
		dlgExpRights.setBGroup(bGroup);
		dlgExpRights.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnExpAnalog()
{
	CBString strRight = RIGHT_READ_RI;
	if (ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight))
	{
		PDlgRiAnalog dlgAnalog;
		dlgAnalog.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnExpCorUser()
{
	CBString strRight = RIGHT_READ_RI;
	if (ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight))
	{
		PDlgExpressCorUsr dlgExpCorUsr;
		dlgExpCorUsr.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnExpGrpRights()
{
	//CBString strRight = RIGHT_READ_RI;
	CBString strRight = RIGHT_UA25_RI;
	if (ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight))
	{
		bool bGroup = true;
		PDlgExpRights dlgExpRights;
		dlgExpRights.setBGroup(bGroup);
		dlgExpRights.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnAdmGroupRights()
{
	CBString strRight = RIGHT_READ_RI;
	if (ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight))
	{
		bool bGroup = true;
		PDlgAdmRights dlgAdmRights;
		dlgAdmRights.setBGroup(bGroup);
		dlgAdmRights.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnDbconfigGroup()
{
	CBString strRight = RIGHT_READ_DLL;
	if (ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight))
	{
		PDlgDllGroup dlgGroups;
		dlgGroups.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnDbkonfigurierenTexte() //ppLoginLanguage
{
	CBString strRight = RIGHT_READ_DLL;
	if (ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight))
	{
		PDlgDllLangText dlgLangText;
		dlgLangText.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnDbkonfigurierenLabel() //ppText
{
	CBString strRight = RIGHT_READ_DLL;
	if (ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight))
	{
		PDlgDllText dlgText;
		dlgText.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnAppParameters()
{
	CBString strRight = RIGHT_READ_DLL;
	if (ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight))
	{
		PDlgDllPara	dlgPara;
		dlgPara.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnUserParameters()
{
	CBString strRight = RIGHT_READ_DLL;
	if (ppBLoginData::ex().isLegitimated(MODULE_DLL, strRight))
	{
		PDlgDllUserPara	dlgPara;
		dlgPara.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnUsertypes()
{
	CBString strRight = RIGHT_READ_DLL;
	if (ppBLoginData::ex().isLegitimated(MODULE_DLL, strRight))
	{
		PDlgDllUsertype	dlgUsertype;
		dlgUsertype.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnDbkonfigurierenPreviouslogin()
{
	CBString strRight = RIGHT_READ_DLL;
	if (ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight))
	{
		PDlgDllPrev	dlgPrev;
		dlgPrev.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}


/**---------------------------------------------------------------------------*/
void PMainFrm::OnChangeHost()
{
	CBString strRight = RIGHT_DEL_DLL;
	if (ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight))
	{
		CBString strError;
		CBString strHost = BConnect::ex().getServer();
		//Log-Eintrag
		CBString strMsg;
		strMsg.format( "   : user '%s' set connect to BACKUP",ppBLoginData::ex().GetUsername().c_str());
		BConnect::ex().writeToLog(strMsg);

		if (BConnect::ex().changeConnection(strError) == false)
		{
			MsgBoxOK(strError);
		}
		else
		{
			//Anblendung Menü
			CString csChangeHost;
			csChangeHost.Format(CResString::ex().getStrTblText(IDS_CHANGE_TO),strHost.c_str());
			GetMenu()->ModifyMenu(ID_CHANGE_HOST, 
		                  MF_BYCOMMAND | MF_STRING, 
						  ID_CHANGE_HOST, 
						  csChangeHost);

			//Anblendung Statusleiste
			CString csConnect;
			csConnect.Format(CResString::ex().getStrTblText(IDS_APP_CONNECT), 
							BConnect::ex().getServer().c_str(),
							BConnect::ex().getCurDatabase().c_str(),
							CConnect::VOODOO_PORT,
							ppBLoginData::ex().GetLanguageID().c_str());           

			SetConnect(csConnect);

			//Log-Eintrag
			strMsg.format(  ": user '%s' set connect to MAIN",ppBLoginData::ex().GetUsername().c_str());
			BConnect::ex().writeToLog(strMsg);
		}
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnBenutzerRechteAnalyse()
{
	CBString strRight = RIGHT_READ_RI;
	if (ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight))
	{
		PDlgRiAnalysis dlgRiAnaly;
		dlgRiAnaly.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnUserAnalyse()
{
	CBString strRight = RIGHT_READ_RI;
	if (ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight))
	{
		PDlgUsrAnalysis dlgUsrAnaly;
		dlgUsrAnaly.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnUserManager()
{
	CBString strRight = RIGHT_READ_RI;
	if (ppBLoginData::ex().isLegitimated(MODULE_RIGHTS, strRight))
	{
		PDlgUserManager dlgUserManager;
		dlgUserManager.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/**---------------------------------------------------------------------------*/
void PMainFrm::OnAnalyseUserrights()
{
    CBString strRight = RIGHT_READ_RI;
	if (ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight))
	{
        PDlgUsrRightAnalysis dlgRiAnaly;
		dlgRiAnaly.DoModal();
	}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
