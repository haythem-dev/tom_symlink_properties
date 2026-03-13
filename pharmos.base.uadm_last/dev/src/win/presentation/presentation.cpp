// presentation.cpp : Definiert das Klassenverhalten für die Anwendung.
/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH						                    */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "stdafx.h"

//library
#include <libscczlib/libscczlib.h>
#include <libscsaccess/libscsaccess.h>
#include <libscsvoodoo/libscsvoodoo.h>
#include <libscsaccvdoo/libscsaccvdoo.h>
#include <libscsaccodbc/libscsaccodbc.h>
#include <libsccutil/libsccutil.h>

//constants
#include <uadm_version.h>
#include <CVersion.h>
#include <CConnect.h>
#include "UADMLoginManager.h"

//business
#include <BConnect.h>
#include <BConnectOdbc.h>
#include <BSqlStat.h>

//presentation
#include "PCmdLineInfo.h"
#include "PDocUA.h"
#include "PViewMain.h"
#include "PMainFrm.h"

//application
#include "presentation.h"

#include <sys/stat.h>
#include <windows.h>
#include <direct.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------
// version string, is requested by unix tool "version"
//--------------------------------------------------------------------
static char *SCCSID="@(#)PRZ GmbH Stand: " UADM_VERSION " $Header: /prj/UAdm/win32/presentation/presentation.cpp 26    3.11.08 16:22 Thomas.hoerath $"; 

//--------------------------------------------------------------------
// DLL - Versions
//--------------------------------------------------------------------
CpresentationApp::T_Dll CpresentationApp::m_sDll[] = 
{
	T_Dll("libsccutil.dll"      , SCCutGetPkgVersion     , SCCutGetVersion     ),
	T_Dll("libsccdate.dll"      , SCCdtGetPkgVersion     , SCCdtGetVersion     ),
	T_Dll("libsccstring.dll"    , sccmn::getStrPkgVersion, sccmn::getStrVersion),
	T_Dll("libscczlib.dll"      , SCCzlGetPkgVersion     , SCCzlGetVersion     ),
	T_Dll("libsccpattern.dll"   , sccmn::getPattPkgVersion, sccmn::getPattVersion      ),
	T_Dll("libscsaccess.dll"    , scsrv::getAccPkgVersion, scsrv::getAccVersion       ),
	T_Dll("libscsvoodoo.dll"    , SCSvdGetPkgVersion     , SCSvdGetVersion     ),
	T_Dll("libscsaccvdoo.dll"   , scsrv::getAccVdooPkgVersion, scsrv::getAccVdooVersion   ),
	T_Dll("libscsaccodbc.dll"   , scsrv::getAccOdbcPkgVersion, scsrv::getAccOdbcVersion   ),
	T_Dll("libscbmeb.dll"       , scbsn::getMebPkgVersion, scbsn::getMebVersion),
	T_Dll("libscgmctrl.dll"     , scgui::getCtrlPkgVersion, scgui::getCtrlVersion),
	T_Dll("libscgmmultilang.dll", scgui::getMultiLangPkgVersion , scgui::getMultiLangVersion ),
	T_Dll("libscgmmeb.dll"      , scgui::getMebPkgVersion, scgui::getMebVersion),
	T_Dll("libscmlogin.dll"     , scmod::getLogPkgVersion, scmod::getLogVersion)
};

//--------------------------------------------------------------------
// Konstruktor - CpresentationApp-Erstellung
//--------------------------------------------------------------------
CpresentationApp::CpresentationApp()
{
}

//--------------------------------------------------------------------
// Destruktor
//--------------------------------------------------------------------
CpresentationApp::~CpresentationApp()
{
}

//--------------------------------------------------------------------
// Das einzige CpresentationApp-Objekt
//-------------------------------------------------------------------- 
CpresentationApp theApp;

//--------------------------------------------------------------------
// CpresentationApp Initialisierung
//--------------------------------------------------------------------
BOOL CpresentationApp::InitInstance()
{
	//Check_DLL´s
	if (CheckDlls() == false)
		return false;

	BeginWaitCursor();

	
	// Dokumentvorlagen der Anwendung registrieren. Dokumentvorlagen
	//  dienen als Verbindung zwischen Dokumenten, Rahmenfenstern und Ansichten.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(PDocUA),
		RUNTIME_CLASS(PMainFrm),       // Haupt-SDI-Rahmenfenster
		RUNTIME_CLASS(PViewMain));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	
	// Befehlszeile parsen, um auf Angaben zu Host, Port u.a. zu prüfen
	PCmdLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	EndWaitCursor();
	
	//LOGIN
	if (Login() == FALSE) 
    {
        //HideApplication();
        CloseAllDocuments(FALSE);   // wegen new SingleDocTemplate
        return FALSE;
    }

	BeginWaitCursor();

	// Datenbank-Verbindung
	if (ConnectDB() == false)		// socket, database
	{
		return FALSE;
	}

	// after login
	//Map für Mehrsprachigkeit
	CBString strLang = ppBLoginData::ex().GetLanguageID();
	strLang.upper();
	CResString::ex().init(CConnect::TEXT_TABLE, strLang.c_str(), CVersion::PROGRAM_NAME);

	// Verteilung der in der Befehlszeile angegebenen Befehle; View-, MainFrm-Aufbau
	if (!ProcessShellCommand(cmdInfo))
	{
		EndWaitCursor();
		return FALSE;
	}
	
	// Das einzige Fenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	
	//schreiben in Log-Files nach Login
	BConnect::ex().setVZ(ppBLoginData::ex().GetAreaID());
	BConnect::ex().setUID(ppBLoginData::ex().GetUserID());
	CBString strLogMsg = "";
	strLogMsg.format(": user '%s' login as MAIN-Connect, client-version '%s'",ppBLoginData::ex().GetUsername().c_str(), C_VERSION_STRING);
	BConnect::ex().writeToLog(strLogMsg);
	CBString strError = "";
	BConnect::ex().changeConnection(strError);
	strLogMsg = "";
	strLogMsg.format(": user '%s' login as BACKUP-Connect, client-version '%s'",ppBLoginData::ex().GetUsername().c_str(), C_VERSION_STRING);
	BConnect::ex().writeToLog(strLogMsg);
	BConnect::ex().changeConnection(strError);
	
	//konstante Fehlermeldungen setzen
	BConnect::ex().setNotDis((LPCTSTR)CResString::ex().getStrTblText(IDS_NOT_ALL_DISCONN));
	BConnect::ex().setNotAct((LPCTSTR)CResString::ex().getStrTblText(IDS_NO_CONN));
	BConnect::ex().setNoActiv((LPCTSTR)CResString::ex().getStrTblText(IDS_NO_ACTIV_CONN));

	CString csUser, csConnect;
	CBString strUsername = ppBLoginData::ex().GetUsername(); //aus Login-DLL

	csUser   .Format(CResString::ex().getStrTblText(IDS_APP_USER), 
						strUsername.c_str());
	csConnect.Format(CResString::ex().getStrTblText(IDS_APP_CONNECT), 
						BConnect::ex().getServer().c_str(),
						BConnect::ex().getCurDatabase().c_str(),
						CConnect::VOODOO_PORT,
		                strLang.c_str());

	((PMainFrm*)m_pMainWnd)->SetConnect(csConnect);
	((PMainFrm*)m_pMainWnd)->SetUser   (csUser   );
	
	
	CWinAppMultiLang::InitInstance(); 

	EndWaitCursor();

	return TRUE;
}

//--------------------------------------------------------------------
// Aufräumen der Anwendung:
//    Socket-Verbindung lösen
//    Singleton-Objekte freigeben
//--------------------------------------------------------------------
int CpresentationApp::ExitInstance() 
{
	DisconnectDB();				 // close db, socket

	ppBLoginData::ex().clearObjCLoginData(); // delete login data object

	return CWinAppMultiLang::ExitInstance();
}

//-------------------------------------------------------------------------------------------------//
bool CpresentationApp::openCfgFile()
{
	CBString file;
	m_TextDat = new CStdioFile( );
	BOOL ret;
	
	struct stat fileStatus;
	char path [MAX_PATH + 1] = { 0 };
	::GetModuleFileName(NULL, path, MAX_PATH);

	char drive[_MAX_DRIVE];
	char dir  [_MAX_DIR  ];
	char fname[_MAX_FNAME];
	char ext  [_MAX_EXT  ];

	_splitpath_s(path, drive, dir, fname, ext);

	file = drive;
	file += dir;

	file += "uadm.config.ini";

	if ( ::stat(file.c_str(), &fileStatus) != -1 )
	{
		ret = m_TextDat->Open( file, CFile::modeRead );
		
		if ( ret == TRUE )
			return true;
	}
	
	delete m_TextDat;
	return false;
}

//-------------------------------------------------------------------------------------------------//
bool CpresentationApp::processCfgFile(std::string& host_main, std::string& host_backup, std::string& db_main, std::string& db_backup)
{
	BOOL ret;
	CString cZeile;
	
	ret = m_TextDat->ReadString( cZeile );
	if ( !ret )
	{
        // Datei leer
		m_TextDat->Close( );
		delete m_TextDat;
		return false;
	}

	CBString zeile;
	size_t pos;

	while (ret == TRUE)
	{
		zeile.format("%s", cZeile);
		
		if (pos = zeile.find("HOST_MAIN:") != CBString::npos)
			host_main = zeile.substr(10, (int)zeile.length() - 10);
		else if (pos = zeile.find("HOST_BACKUP:") != CBString::npos)
			host_backup = zeile.substr(12, (int)zeile.length() - 12);
		else if (pos = zeile.find("DB_MAIN:") != CBString::npos)
			db_main = zeile.substr(8, (int)zeile.length() - 8);
		else if (pos = zeile.find("DB_BACKUP:") != CBString::npos)
			db_backup = zeile.substr(10, (int)zeile.length() - 10);
		else
		{
			// key nicht bekannt 
		}

		ret = m_TextDat->ReadString( cZeile );
	}

	delete m_TextDat;

	// prüfen, ob alle Werte gefüllt wurden
	if ( host_main.empty() || host_backup.empty() || db_main.empty() ||db_backup.empty() )
	{
		return false;
	}
	return true;
}

//--------------------------------------------------------------------
// Login-DLL
//--------------------------------------------------------------------
BOOL CpresentationApp::Login()
{
	if (openCfgFile() == false)
	{
		MessageBox(NULL, "Cannot open config file uadm.config.ini", "ERROR", MB_OK | MB_ICONERROR);
		return false;
	}

	std::string host_main, host_backup, db_main, db_backup;
	if (processCfgFile(host_main, host_backup, db_main, db_backup) == false)
		return false;

	// call login dlg
	UADM::CUADMLoginManager loginMgr;

	if (loginMgr.initialize("", "", host_main, db_main, host_backup, db_backup, "", "", CVersion::PROGRAM_NAME, UADM_VERSION) == false)
	{
		return FALSE;
	}
	if (loginMgr.loginWithDialog(GetBackgroundImagePath().GetString(), NULL) == false)
	{
		BConnect::ex().setNoLoginData(true); // for logout logging
		return FALSE;
	}
	else
	{
		// store the values from login manager into the ppBLoginData object
		ppBLoginData::ex().createObjCLoginData();
		ppBLoginData::ex().SetAppID(loginMgr.getAppID());
		ppBLoginData::ex().SetGlobalUser(loginMgr.getOSAccount().c_str());
		ppBLoginData::ex().SetAppName(loginMgr.getAppName().c_str());
		ppBLoginData::ex().SetLanguageID(loginMgr.getLanguage().c_str());
		ppBLoginData::ex().SetAreaID(loginMgr.getRegionBranchNoVal());
		ppBLoginData::ex().SetUID(loginMgr.getUserID());
		ppBLoginData::ex().SetUsername(loginMgr.getUsername().c_str());
		ppBLoginData::ex().SetPassword(""); // do not store password
		ppBLoginData::ex().SetAreaName(loginMgr.getRegionBranchName().c_str());
		ppBLoginData::ex().SetLanguageName(loginMgr.getLanguageName().c_str());
		ppBLoginData::ex().SetChangedPW(""); // do not store changed password
		ppBLoginData::ex().SetSurname(loginMgr.getLastname().c_str());
		ppBLoginData::ex().SetForename(loginMgr.getFirstname().c_str());
		ppBLoginData::ex().SetMail(loginMgr.getEMail().c_str());
		ppBLoginData::ex().SetEmail(loginMgr.getEMail().c_str());
		ppBLoginData::ex().SetNtUser(loginMgr.getDomainAccount());

		// set rights and parameters into ppBLoginData object (this is filled with module.regionbranch.right)
		std::vector<UADM::UADM_APP_PERMISSION*>* vecPerms = loginMgr.getPermissions();
		if (vecPerms != NULL)
		{
			for (size_t i = 0; i < vecPerms->size(); i++)
			{
				UADM::UADM_APP_PERMISSION* pPerm = vecPerms->at(i);
				if (pPerm != NULL)
				{
					CBString strModBranchRight = std::to_string(pPerm->intModuleID) + "." + std::to_string(pPerm->intRegionBranch) + "." + pPerm->strRight;
					ppBLoginData::ex().setRightMap(strModBranchRight, true);
				}
			}
		}
		// check if user have roles and add it into the string list
		std::vector<UADM::UADM_USER_ROLE*>* vecRoles = loginMgr.getUserRoles();
		if (vecRoles != NULL)
		{
			for (size_t i = 0; i < vecRoles->size(); i++)
			{
				UADM::UADM_USER_ROLE* pRole = vecRoles->at(i);
				if (pRole != NULL)
				{
					CString strRole = pRole->strRoleName.c_str();
					m_lstUserRoles.AddTail(strRole);
				}
			}
		}
		// set parameters into ppBLoginData object (this is filled with para1para2parabez and its value)
		std::vector<UADM::UADM_APP_PARAMETER*>* vecParams = loginMgr.getParameters();
		if (vecParams != NULL)
		{
			for (size_t i = 0; i < vecParams->size(); i++)
			{
				UADM::UADM_APP_PARAMETER* pParam = vecParams->at(i);
				if (pParam != NULL)
				{
					CBString strPara = pParam->strPara1 + pParam->strPara2 + pParam->strParaName;
					ppBLoginData::ex().setParaMAP(strPara, pParam->strParaValue);
				}
			}
		}

		return TRUE; // login dlg ok: username is now set
	}
}

//--------------------------------------------------------------------
// ConnectDB
//---------------------------------------------------------------------------
bool CpresentationApp::ConnectDB()
{
	CBString strError;
	/* LESEN AUS INI oder SIEHE CCONNECT*/

	if (BConnect::ex().connect(strError) == false)
	{
		EndWaitCursor();
		MsgBox1(strError, "OK");

		return false;
	}

	return true;
}

//--------------------------------------------------------------------
// DisconnectDB
//---------------------------------------------------------------------------
bool CpresentationApp::DisconnectDB()
{
	CBString strError;

	if (BConnect::ex().disconnect(strError) == false)
	{
		MsgBoxOK(strError);

		return false;
	}

	if (BConnectOdbc::ex().disconnect(strError) == false)
	{
		MsgBoxOK(strError, MB_ICONERROR);
		return false;
	}

	return true;

	return true;
}

//--------------------------------------------------------------------
// CheckDlls
//---------------------------------------------------------------------------
bool CpresentationApp::CheckDlls()
{
	const char * const spPkgVersion = CVersion::LIBVERSION_STRING;
	char             * spFormat     = "product version %s (%s) <> expected version (%s)";
	char               szText[256];
	int                iCntDlls     = sizeof(m_sDll) / sizeof(T_Dll);

	for (int i = 0; i < iCntDlls; i++)
	{
		if (strcmp(spPkgVersion, m_sDll[i].m_fpProdVersion()) != 0)	// crypt dll
		{
			sprintf_s(szText, spFormat, 
							m_sDll[i].m_spName, 
							m_sDll[i].m_fpProdVersion(),
				            spPkgVersion);
			MsgBox1(szText, "OK");

			return false;
		}
	}

	return true;
}


//--------------------------------------------------------------------
// Get path to background image
//---------------------------------------------------------------------------
CString CpresentationApp::GetBackgroundImagePath()
{
	char appPath[MAX_PATH] = { 0 };

	// Get full path to the executable
	if (GetModuleFileNameA(NULL, appPath, sizeof(appPath)) == 0)
	{
		return CString(); // return empty if failed
	}

	// Remove the exe filename, leave only folder
	PathRemoveFileSpecA(appPath);

	// Build the path to your BMP
	CString strBack(appPath);
	strBack.Append("\\UADMLogin.bmp");

	return strBack;
}
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
