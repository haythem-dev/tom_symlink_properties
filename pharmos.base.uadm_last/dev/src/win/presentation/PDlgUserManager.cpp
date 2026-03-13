/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"
#include "presentation.h"
#include "PDlgUserManager.h"
#include <libscsvoodoo/libscsvoodoo.h>
#include "UADMLoginManager.h"
#include "srv.h"
#include "defines.h"
#include <string>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(PDlgUserManager, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgUserManager, CDialogMultiLang)
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BTN_REFRESH_USERLIST, OnBtnClickedUsersRefresh)
	ON_BN_CLICKED(IDC_BTN_REPORT_USERS, OnBtnClickedUsersReport)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_USERS, OnUsersList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_USERS, OnUsersList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LST_USERS, OnUsersList)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_USERPARAMS, OnUserParamsList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_USERPARAMS, OnUserParamsList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LST_USERPARAMS, OnUserParamsList)
	ON_BN_CLICKED(IDC_BTN_NEW_USER, OnBtnUserNew)
	ON_BN_CLICKED(IDC_BTN_SAVE_USER, OnBtnUserSave)
	ON_BN_CLICKED(IDC_BTN_DELETE_USER, OnBtnUserDelete)
	ON_CBN_SELCHANGE(IDC_CMB_APPLICATIONSEL, OnApplicationSelChange)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnApplicationDescTipNotify)
	ON_BN_CLICKED(IDC_BTN_PERMITGROUP, OnBtnPermitGroup)
	ON_BN_CLICKED(IDC_BTN_REVOKEGROUP, OnBtnRevokeGroup)
	ON_BN_CLICKED(IDC_BTN_NEW_USERPARAM, OnBtnUserParamNew)
	ON_BN_CLICKED(IDC_BTN_SAVE_USERPARAM, OnBtnUserParamSave)
	ON_BN_CLICKED(IDC_BTN_DELETE_USERPARAM, OnBtnUserParamDelete)
	ON_BN_CLICKED(IDC_BTN_USERMANAGER_EXIT, OnBtnUserManagerExit)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
PDlgUserManager::PDlgUserManager() : CDialogMultiLang(PDlgUserManager::IDD)
{
	m_intUserState = 0;
	m_intUserParamState = 0;
	m_strOrgPasswordPHX = "";
	m_strOrgPasswordAES = "";
	m_strSeparator = ";";
}

/**---------------------------------------------------------------------------*/
PDlgUserManager::~PDlgUserManager()
{
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_USERS, m_lstUsers);
	DDX_Control(pDX, IDC_LST_ALLGROUPS, m_lstAllGroups);
	DDX_Control(pDX, IDC_LST_USERGROUPS, m_lstUserGroups);
	DDX_Control(pDX, IDC_LST_USERPARAMS, m_lstUserParams);
	DDX_Control(pDX, IDC_CHK_USERACTIVE, m_chkUserActive);
	DDX_Control(pDX, IDC_CHK_CRYPTPHX, m_chkCryptPHX);
	DDX_Control(pDX, IDC_CHK_CRYPTAES, m_chkCryptAES);
	DDX_Control(pDX, IDC_CHK_PWDCHANGE, m_chkPwdChange);
	DDX_Control(pDX, IDC_CHK_EXTERNAL, m_chkExternal);
	DDX_Control(pDX, IDC_CMB_USERTYPESEL, m_cmbUserType);
	DDX_Control(pDX, IDC_CMB_COUNTRYSEL, m_cmbCountry);
	DDX_Control(pDX, IDC_CMB_APPLICATIONSEL, m_cmbApplication);
	DDX_Control(pDX, IDC_CMB_BRANCHSEL, m_cmbBranch);
	DDX_Control(pDX, IDC_CMB_PARAMETERSEL, m_cmbUserParam);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgUserManager::OnInitDialog()
{
	CDialogMultiLang::OnInitDialog();

	// predefine the list controls
	// init the user list
	COLUMN_TYPES UserColumns[] =
	{
		0, CResString::ex().getStrTblText(IDS_APP_USERMGR_USERID), 45, LVCFMT_LEFT,
		1, CResString::ex().getStrTblText(IDS_APP_USERMGR_USERNAME), 90, LVCFMT_LEFT,
		2, CResString::ex().getStrTblText(IDS_APP_USERMGR_LASTNAME), 85, LVCFMT_LEFT,
		3, CResString::ex().getStrTblText(IDS_APP_USERMGR_FIRSTNAME), 80, LVCFMT_LEFT,
		4, CResString::ex().getStrTblText(IDS_APP_USERMGR_USERTYPE), 90, LVCFMT_LEFT,
		5, CResString::ex().getStrTblText(IDS_APP_USERMGR_COUNTRY), 45, LVCFMT_LEFT,
		-1, "", 0, 0
	};
	InitListbox(&m_lstUsers, UserColumns);
	m_lstUsers.ShowScrollBar(SB_VERT, 1);
	m_lstUsers.SetExtendedStyle(m_lstUsers.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);

	// init the all groups list
	COLUMN_TYPES AllGroupColumns[] =
	{
		0, CResString::ex().getStrTblText(IDS_APP_USERMGR_GROUPID), 40, LVCFMT_LEFT,
		1, CResString::ex().getStrTblText(IDS_APP_USERMGR_GROUPNAME), 170, LVCFMT_LEFT,
		-1, "", 0, 0
	};
	InitListbox(&m_lstAllGroups, AllGroupColumns);
	m_lstAllGroups.ShowScrollBar(SB_VERT, 0);
	m_lstAllGroups.ShowScrollBar(SB_HORZ, 1);
	m_lstAllGroups.SetExtendedStyle(m_lstAllGroups.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);

	// init the user mapped groups list
	COLUMN_TYPES UserGroupColumns[] =
	{
		0, CResString::ex().getStrTblText(IDS_APP_USERMGR_GROUPID), 40, LVCFMT_LEFT,
		1, CResString::ex().getStrTblText(IDS_APP_USERMGR_GROUPNAME), 170, LVCFMT_LEFT,
		-1, "", 0, 0
	};
	InitListbox(&m_lstUserGroups, UserGroupColumns);
	m_lstUserGroups.ShowScrollBar(SB_VERT, 1);
	m_lstUserGroups.SetExtendedStyle(m_lstUserGroups.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	
	// init the user parameters list
	COLUMN_TYPES UserParamColumns[] =
	{
		0, CResString::ex().getStrTblText(IDS_APP_USERMGR_BRANCH), 100, LVCFMT_LEFT,
		1, CResString::ex().getStrTblText(IDS_APP_USERMGR_PARAMNAME), 90, LVCFMT_LEFT,
		2, CResString::ex().getStrTblText(IDS_APP_USERMGR_PARAMVALUE), 90, LVCFMT_LEFT,
		-1, "", 0, 0
	};
	InitListbox(&m_lstUserParams, UserParamColumns);
	m_lstUserParams.ShowScrollBar(SB_VERT, 1);
	m_lstUserParams.SetExtendedStyle(m_lstUserParams.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);

	ResetUserList(true);
	ResetUserDetails(0);
	InitUsertypes();
	InitCountries();

	m_AppDescTip.Create(this);
	m_AppDescTip.Activate(TRUE);
	m_AppDescTip.AddTool(&m_cmbApplication, _T(""));
	InitApplications();

	ResetAllGroups(true);
	ResetUserGroups(true);
	ResetUserParams(true);
	InitBranches();
	InitUserParams();
	LoadUsers(false, NULL);

	// based on permissions disable some buttons
	CBString strRight = RIGHT_WORK_RI;
	if (ppBLoginData::ex().isLegitimated(MODULE_RIGHTS, strRight) == false)
	{
		// new, save or delete user is off
		GetDlgItem(IDC_BTN_NEW_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVE_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DELETE_USER)->EnableWindow(FALSE);
		// permit or revoke group is off
		GetDlgItem(IDC_BTN_PERMITGROUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REVOKEGROUP)->EnableWindow(FALSE);
		// new, save or delete user param is off
		GetDlgItem(IDC_BTN_NEW_USERPARAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVE_USERPARAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DELETE_USERPARAM)->EnableWindow(FALSE);
	}

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BOOL PDlgUserManager::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// filter edit fields - load user list
			if (GetFocus() == GetDlgItem(IDC_EDT_FILTER_USERID))
			{
				LoadUsers(false, NULL);
				return TRUE;
			}
			if (GetFocus() == GetDlgItem(IDC_EDT_FILTER_USERNAME))
			{
				LoadUsers(false, NULL);
				return TRUE;
			}
			if (GetFocus() == GetDlgItem(IDC_EDT_FILTER_SURNAME))
			{
				LoadUsers(false, NULL);
				return TRUE;
			}
			if (GetFocus() == GetDlgItem(IDC_EDT_FILTER_FORENAME))
			{
				LoadUsers(false, NULL);
				return TRUE;
			}
			if (GetFocus() == GetDlgItem(IDC_EDT_FILTER_USERTYPE))
			{
				LoadUsers(false, NULL);
				return TRUE;
			}
			if (GetFocus() == GetDlgItem(IDC_EDT_FILTER_COUNTRY))
			{
				LoadUsers(false, NULL);
				return TRUE;
			}
			if (GetFocus() == GetDlgItem(IDC_EDT_FILTER_ALLGROUPID))
			{
				LoadAllGroups(-1, -1); // reload all groups with current selected app and user 
				return TRUE;
			}
			if (GetFocus() == GetDlgItem(IDC_EDT_FILTER_ALLGROUPNAME))
			{
				LoadAllGroups(-1, -1); // reload all groups with current selected app and user 
				return TRUE;
			}
			if (GetFocus() == GetDlgItem(IDC_EDT_FILTER_USERGROUPID))
			{
				LoadUserGroups(-1, -1); // reload user groups with current selected app and user 
				return TRUE;
			}
			if (GetFocus() == GetDlgItem(IDC_EDT_FILTER_USERGROUPNAME))
			{
				LoadUserGroups(-1, -1); // reload user groups with current selected app and user 
				return TRUE;
			}
			if (GetFocus() == GetDlgItem(IDC_EDT_FILTER_BRANCH))
			{
				LoadUserParams(-1, -1); // reload user params with current selected app and user 
				return TRUE;
			}
			if (GetFocus() == GetDlgItem(IDC_EDT_FILTER_PARAMETER))
			{
				LoadUserParams(-1, -1); // reload user params with current selected app and user 
				return TRUE;
			}
			if (GetFocus() == GetDlgItem(IDC_EDT_FILTER_VALUE))
			{
				LoadUserParams(-1, -1); // reload user params with current selected app and user 
				return TRUE;
			}
			return FALSE;

		case VK_ESCAPE:
			return FALSE;


		default:
			break;
		}
	}

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}


/**---------------------------------------------------------------------------*/
HBRUSH PDlgUserManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogMultiLang::OnCtlColor(pDC, pWnd, nCtlColor);
	int ctrlid = pWnd->GetDlgCtrlID();
	// default gray
	COLORREF gcol = RGB(232, 232, 232);
	COLORREF wcol = RGB(255, 255, 255);
	COLORREF ycol = RGB(255, 255, 192);
	COLORREF col = gcol;
	if ((ctrlid == IDC_EDT_USERID) || (ctrlid == IDC_EDT_LOGINDATE) || (ctrlid == IDC_EDT_PWDCHANGEDATE))
	{
		pDC->SetBkColor(gcol);
	}
	else if ((ctrlid == IDC_EDT_USERNAME) || (ctrlid == IDC_EDT_PASSWORD) || (ctrlid == IDC_EDT_SURNAME) ||
			 (ctrlid == IDC_EDT_FORENAME) || (ctrlid == IDC_CMB_USERTYPESEL) ||
			 (ctrlid == IDC_CMB_COUNTRYSEL) || (ctrlid == IDC_EDT_EMAIL))
	{
		if (m_intUserState == 1) col = wcol; else if (m_intUserState == 2) col = ycol;
		pDC->SetBkColor(col);
	}
	else if ((ctrlid == IDC_EDT_NTUSER) || (ctrlid == IDC_EDT_DESC) || (ctrlid == IDC_EDT_REFERENCE))
	{
		if (m_intUserState == 1) col = wcol; else if (m_intUserState == 2) col = wcol;
		pDC->SetBkColor(col);
	}
	else if ((ctrlid == IDC_CMB_BRANCHSEL) || (ctrlid == IDC_CMB_PARAMETERSEL) || (ctrlid == IDC_EDT_PARAMETERVALUE))
	{
		if (m_intUserParamState == 1) col = wcol; else if (m_intUserParamState == 2) col = ycol;
		pDC->SetBkColor(col);
	}
	return hbr;
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// paint the delimiter on the view between stagein and stageout
	if ((nIDCtl == IDC_DELIMITER1) || (nIDCtl == IDC_DELIMITER2))
	{
		CWnd* pItem = GetDlgItem(nIDCtl);
		CDC* pDC = pItem->GetDC();
		CRect rect;
		pItem->GetClientRect(&rect);

		CBrush bru(RGB(85, 140, 110));
		CBrush* pbru = pDC->SelectObject(&bru);
		CPen pen(PS_SOLID, 1, RGB(85, 140, 110));
		CPen* ppen = pDC->SelectObject(&pen);

		pDC->Rectangle(rect);

		pDC->SelectObject(ppen);
		pDC->SelectObject(pbru);

		pItem->ReleaseDC(pDC);
	}
	CDialogMultiLang::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::InitListbox(CListCtrl* pListbox, COLUMN_TYPES* pColtypes)
{
	if ((pListbox == NULL) || (pColtypes == NULL))
	{
		return;
	}
	LV_COLUMN lvC;

	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (COLUMN_TYPES* pType = pColtypes; pType->Index != -1; pType++)
	{
		lvC.iSubItem = pType->Index;
		lvC.pszText = pType->Columns.GetBuffer();
		lvC.fmt = pType->Align;
		lvC.cx = pType->Width;

		if (pListbox->InsertColumn(pType->Index, &lvC) == -1)
		{
			return;
		}
	}
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::InitUsertypes()
{
	long sqlStatus;
	int  sqlHandle;
	CString sqlQuery;
	CString lastError;

	m_cmbUserType.ResetContent();

	sqlQuery = "SELECT usertypeid, name FROM ppusertype ORDER BY usertypeid";
	sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery);
	if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
	{
		lastError = DynErrmld();
		MsgBoxOK(lastError);
		return;
	}
	short usertypeid;
	char name[151];
	sqlStatus = SqlFetch(sqlHandle, 1, &usertypeid, &name, NULL);
	if (sqlStatus == SQL_NOTFOUND)
	{
		SqlCloseCs(sqlHandle);
		return;
	}
	do
	{
		CString strTmp;
		strTmp.Format("%s", CString(name).Trim());
		int nIndex = m_cmbUserType.AddString(strTmp);
		m_cmbUserType.SetItemData(nIndex, usertypeid);
		// get next
		sqlStatus = SqlFetch(sqlHandle, 1, &usertypeid, &name, NULL);
	} 
	while (sqlStatus == SQL_SUCCESS);
	
	SqlCloseCs(sqlHandle);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::InitCountries()
{
	m_cmbCountry.ResetContent();

	m_cmbCountry.AddString("DE - Germany");
	m_cmbCountry.AddString("AT - Austria");
	m_cmbCountry.AddString("CH - Switzerland");
	m_cmbCountry.AddString("FR - France");
	m_cmbCountry.AddString("BG - Bulgaria");
	m_cmbCountry.AddString("HR - Croatia");
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::InitApplications()
{
	long sqlStatus;
	int  sqlHandle;
	CString sqlQuery;
	CString lastError;

	m_cmbApplication.ResetContent();
	m_vecAppDesc.clear();
	int nIndex = m_cmbApplication.AddString("- please select -");
	m_cmbApplication.SetItemData(nIndex, 0);
	m_cmbApplication.SetCurSel(0);
	m_vecAppDesc.push_back("Please select one of the applications from the list");

	sqlQuery = "SELECT applicationid, name, description FROM ppapplication ORDER BY name";
	sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery);
	if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
	{
		lastError = DynErrmld();
		MsgBoxOK(lastError);
		return;
	}
	long applicationid;
	char name[31];
	char desc[255];
	sqlStatus = SqlFetch(sqlHandle, 1, &applicationid, &name, &desc, NULL);
	if (sqlStatus == SQL_NOTFOUND)
	{
		SqlCloseCs(sqlHandle);
		return;
	}
	do
	{
		CString strTmp;
		strTmp.Format("%03d - %s", applicationid, CString(name).Trim());
		m_vecAppDesc.push_back(CString(desc).Trim());
		nIndex = m_cmbApplication.AddString(strTmp);
		m_cmbApplication.SetItemData(nIndex, applicationid);
		// get next
		sqlStatus = SqlFetch(sqlHandle, 1, &applicationid, &name, &desc, NULL);
	}
	while (sqlStatus == SQL_SUCCESS);

	SqlCloseCs(sqlHandle);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::InitBranches()
{
	long sqlStatus;
	int  sqlHandle;
	CString sqlQuery;
	CString lastError;
	m_cmbBranch.ResetContent();

	sqlQuery = "SELECT regionbranchid, name, country FROM ppregionbranch WHERE regionbranchid < 100 ORDER BY regionbranchid";
	sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery);
	if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
	{
		lastError = DynErrmld();
		MsgBoxOK(lastError);
		return;
	}
	short branchid;
	char name[101];
	char country[3];
	int nIndex;
	sqlStatus = SqlFetch(sqlHandle, 1, &branchid, &name, &country, NULL);
	if (sqlStatus == SQL_NOTFOUND)
	{
		SqlCloseCs(sqlHandle);
		return;
	}
	do
	{
		CString strTmp;
		strTmp.Format("%02d - %s (%s)", branchid, CString(name).Trim().GetBuffer(), CString(country).Trim().GetBuffer());
		nIndex = m_cmbBranch.AddString(strTmp);
		m_cmbBranch.SetItemData(nIndex, branchid);
		// get next
		sqlStatus = SqlFetch(sqlHandle, 1, &branchid, &name, &country, NULL);
	}
	while (sqlStatus == SQL_SUCCESS);

	SqlCloseCs(sqlHandle);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::InitUserParams()
{
	long sqlStatus;
	int  sqlHandle;
	CString sqlQuery;
	CString lastError;
	m_cmbUserParam.ResetContent();

	sqlQuery = "SELECT userparaid, name FROM ppuserpara ORDER BY userparaid";
	sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery);
	if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
	{
		lastError = DynErrmld();
		MsgBoxOK(lastError);
		return;
	}
	short parameterid;
	char name[101];
	int nIndex;
	sqlStatus = SqlFetch(sqlHandle, 1, &parameterid, &name, NULL);
	if (sqlStatus == SQL_NOTFOUND)
	{
		SqlCloseCs(sqlHandle);
		return;
	}
	do
	{
		CString strTmp;
		strTmp.Format("%02d - %s", parameterid, CString(name).Trim());
		nIndex = m_cmbUserParam.AddString(strTmp);
		m_cmbUserParam.SetItemData(nIndex, parameterid);
		// get next
		sqlStatus = SqlFetch(sqlHandle, 1, &parameterid, &name, NULL);
	}
	while (sqlStatus == SQL_SUCCESS);

	SqlCloseCs(sqlHandle);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::ResetUserList(bool filter)
{
	m_lstUsers.DeleteAllItems();
	if (filter == true)
	{
		GetDlgItem(IDC_EDT_FILTER_USERID)->SetWindowText("");
		GetDlgItem(IDC_EDT_FILTER_USERNAME)->SetWindowText("");
		GetDlgItem(IDC_EDT_FILTER_SURNAME)->SetWindowText("");
		GetDlgItem(IDC_EDT_FILTER_FORENAME)->SetWindowText("");
		GetDlgItem(IDC_EDT_FILTER_USERTYPE)->SetWindowText("");
		GetDlgItem(IDC_EDT_FILTER_COUNTRY)->SetWindowText("");
	}
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::ResetUserDetails(int state)
{
	m_intUserState = state;
	m_strOrgPasswordPHX = "";
	m_strOrgPasswordAES = "";
	GetDlgItem(IDC_EDT_USERID)->SetWindowText("");
	GetDlgItem(IDC_EDT_USERNAME)->SetWindowText("");
	m_chkUserActive.SetCheck(state == 2 ? 1 : 0);
	GetDlgItem(IDC_EDT_LOGINDATE)->SetWindowText("");
	GetDlgItem(IDC_EDT_PASSWORD)->SetWindowText("");
	m_chkCryptPHX.SetCheck(0);
	m_chkCryptAES.SetCheck(0);
	m_chkPwdChange.SetCheck(state == 2 ? 1 : 0);
	GetDlgItem(IDC_EDT_PWDCHANGEDATE)->SetWindowText("");
	m_cmbUserType.SetCurSel(-1);
	m_cmbUserType.RedrawWindow();
	m_cmbCountry.SetCurSel(-1);
	m_cmbCountry.RedrawWindow();
	GetDlgItem(IDC_EDT_EMAIL)->SetWindowText("");
	m_chkExternal.SetCheck(0);
	GetDlgItem(IDC_EDT_NTUSER)->SetWindowText("");
	GetDlgItem(IDC_EDT_DESC)->SetWindowText("");
	GetDlgItem(IDC_EDT_REFERENCE)->SetWindowText("");
	GetDlgItem(IDC_EDT_SURNAME)->SetWindowText("");
	GetDlgItem(IDC_EDT_FORENAME)->SetWindowText("");
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::ResetAllGroups(bool filter)
{
	m_lstAllGroups.DeleteAllItems();
	if (filter == true)
	{
		GetDlgItem(IDC_EDT_FILTER_ALLGROUPID)->SetWindowText("");
		GetDlgItem(IDC_EDT_FILTER_ALLGROUPNAME)->SetWindowText("");
	}
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::ResetUserGroups(bool filter)
{
	m_lstUserGroups.DeleteAllItems();
	if (filter == true)
	{
		GetDlgItem(IDC_EDT_FILTER_USERGROUPID)->SetWindowText("");
		GetDlgItem(IDC_EDT_FILTER_USERGROUPNAME)->SetWindowText("");
	}
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::ResetUserParams(bool filter)
{
	m_lstUserParams.DeleteAllItems();
	if (filter == true)
	{
		GetDlgItem(IDC_EDT_FILTER_BRANCH)->SetWindowText("");
		GetDlgItem(IDC_EDT_FILTER_PARAMETER)->SetWindowText("");
		GetDlgItem(IDC_EDT_FILTER_VALUE)->SetWindowText("");
	}
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::ResetUserParamDetails(int state)
{
	m_intUserParamState = state;
	m_cmbBranch.SetCurSel(-1);
	m_cmbBranch.RedrawWindow();
	m_cmbUserParam.SetCurSel(-1);
	m_cmbUserParam.RedrawWindow();
	GetDlgItem(IDC_EDT_PARAMETERVALUE)->SetWindowText("");
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::OnBtnClickedUsersRefresh()
{
	LoadUsers(false, NULL);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::LoadUsers(bool bExport, std::ofstream* pFile)
{
	long sqlStatus;
	int  sqlHandle;
	CString sqlQuery;
	long sqlCount;
	CString lastError;

	if (bExport == true)
	{
		if (pFile == NULL)
		{
			return;
		}
		// write header row
		(*pFile) << "User ID" << m_strSeparator << "Username" << m_strSeparator << "Firstname" << m_strSeparator << "Lastname" << m_strSeparator << "EMail"
			<< m_strSeparator << "Country" << m_strSeparator << "Usertype" << m_strSeparator << "Active" << m_strSeparator << "Last Login Date" << m_strSeparator 
			<< "Force PW Change" << m_strSeparator << "Last PW Change Date" << std::endl;
	}
	else
	{
		ResetUserList(false);
	}

	// get filters
	CString fUserID;
	CString fUsername;
	CString fSurname;
	CString fForename;
	CString fUserType;
	CString fCountry;
	GetDlgItem(IDC_EDT_FILTER_USERID)->GetWindowText(fUserID);
	GetDlgItem(IDC_EDT_FILTER_USERNAME)->GetWindowText(fUsername);
	GetDlgItem(IDC_EDT_FILTER_SURNAME)->GetWindowText(fSurname);
	GetDlgItem(IDC_EDT_FILTER_FORENAME)->GetWindowText(fForename);
	GetDlgItem(IDC_EDT_FILTER_USERTYPE)->GetWindowText(fUserType);
	GetDlgItem(IDC_EDT_FILTER_COUNTRY)->GetWindowText(fCountry);

	// get the total count first
	sqlQuery = "SELECT CAST(COUNT(*) AS INTEGER) FROM ppuser";
	long totalcount = 0;
	SqlRead(sqlQuery, &totalcount, NULL);

	// now get the filtered data, but max 100 rows for view
	CString strCountFilter = "";
	CString strAddColumns = "";
	if (bExport == false)
	{
		strCountFilter = "FIRST 100";
	}
	else
	{
		strAddColumns = ", u.emailuser, u.activ, u.lastlogin, u.changepw, u.lastpwchange, u.external";
	}
	sqlQuery = "SELECT " + strCountFilter + " u.userid, u.username, u.surname, u.forename, u.usertypeid, ut.name, u.country" + strAddColumns + " FROM ppuser u, ppusertype ut WHERE u.usertypeid=ut.usertypeid";
	if (fUserID.GetLength() > 0)
	{
		// make sure, that only numbers are used
		int fid = atoi(fUserID.GetBuffer());
		CString strTmp;
		strTmp.Format("%d", fid);
		sqlQuery += " AND u.userid=" + strTmp;
	}
	if (fUsername.GetLength() > 0)
	{
		sqlQuery += " AND LOWER(u.username) LIKE '%" + fUsername.MakeLower() + "%'";
	}
	if (fSurname.GetLength() > 0)
	{
		sqlQuery += " AND LOWER(u.surname) LIKE '%" + fSurname.MakeLower() + "%'";
	}
	if (fForename.GetLength() > 0)
	{
		sqlQuery += " AND LOWER(u.forename) LIKE '%" + fForename.MakeLower() + "%'";
	}
	if (fUserType.GetLength() > 0)
	{
		sqlQuery += " AND LOWER(ut.name) LIKE '%" + fUserType.MakeLower() + "%'";
	}
	if (fCountry.GetLength() > 0)
	{
		sqlQuery += " AND LOWER(u.country) LIKE '%" + fCountry.MakeLower() + "%'";
	}
	sqlQuery += " ORDER BY u.userid";

	sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery);
	if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
	{
		lastError = DynErrmld();
		MsgBoxOK(lastError);
		return;
	}

	long userid;
	char username[21];
	char surname[31];
	char forename[21];
	short usertypeid;
	char usertype[151];
	char country[3];
	char emailuser[51];
	short active;
	long lastlogin;
	short changepw;
	long lastpwchange;
	short external;

	sqlCount = 0;

	if (bExport == true)
	{ 
		sqlStatus = SqlFetch(sqlHandle, 1, &userid, &username, &surname, &forename, &usertypeid, &usertype, &country, &emailuser, &active, &lastlogin, &changepw, &lastpwchange, &external, NULL);
	}
	else
	{
		sqlStatus = SqlFetch(sqlHandle, 1, &userid, &username, &surname, &forename, &usertypeid, &usertype, &country, NULL);
	}
	if (sqlStatus == SQL_NOTFOUND)
	{
		CString strCount;
		strCount.Format(CResString::ex().getStrTblText(IDS_USER_COUNT), sqlCount, totalcount);
		GetDlgItem(IDC_TXT_USERCOUNT)->SetWindowText(strCount);
		lastError = CResString::ex().getStrTblText(IDS_NO_USERS);
		SqlCloseCs(sqlHandle);
		MsgBoxOK(lastError);
		return;
	}

	do
	{
		if (bExport == true)
		{
			// write into csv file
			(*pFile) << userid << m_strSeparator << CString(username).Trim() << m_strSeparator << CString(forename).Trim() << m_strSeparator
				<< CString(surname).Trim() << m_strSeparator << CString(emailuser).Trim() << m_strSeparator << CString(country).Trim()
				<< m_strSeparator << CString(usertype).Trim() << m_strSeparator;
			if (active == 1)
			{
				(*pFile) << "Yes" << m_strSeparator;
			}
			else
			{
				(*pFile) << "No" << m_strSeparator;
			}
			if (lastlogin > 0)
			{
				// convert yyyymmdd to dd.mm.yyyy
				CString strDate;
				strDate.Format("%02d.%02d.%04d", lastlogin % 100, (lastlogin / 100) % 100, lastlogin / 10000);
				(*pFile) << strDate << m_strSeparator;
			}
			else
			{
				(*pFile) << "-" << m_strSeparator;
			}
			if (changepw == 1)
			{
				(*pFile) << "Yes" << m_strSeparator;
			}
			else
			{
				(*pFile) << "No" << m_strSeparator;
			}
			if (lastpwchange > 0)
			{
				// convert yyyymmdd to dd.mm.yyyy
				CString strDate;
				strDate.Format("%02d.%02d.%04d", lastpwchange % 100, (lastpwchange / 100) % 100, lastpwchange / 10000);
				(*pFile) << strDate;
			}
			else
			{
				(*pFile) << "-";
			}
			(*pFile) << std::endl;
		}
		else
		{
			if (sqlCount < 100)
			{
				// add into user listctrl
				CString strTmp;
				strTmp.Format("%d", userid);
				int nItem = m_lstUsers.InsertItem(sqlCount, strTmp);
				m_lstUsers.SetItemText(nItem, 1, CString(username).Trim());
				m_lstUsers.SetItemText(nItem, 2, CString(surname).Trim());
				m_lstUsers.SetItemText(nItem, 3, CString(forename).Trim());
				m_lstUsers.SetItemText(nItem, 4, CString(usertype).Trim());
				m_lstUsers.SetItemText(nItem, 5, CString(country).Trim());
			}
		}
		// get next
		sqlCount++;
		if (bExport == true)
		{
			sqlStatus = SqlFetch(sqlHandle, 1, &userid, &username, &surname, &forename, &usertypeid, &usertype, &country, &emailuser, &active, &lastlogin, &changepw, &lastpwchange, &external, NULL);
		}
		else
		{
			sqlStatus = SqlFetch(sqlHandle, 1, &userid, &username, &surname, &forename, &usertypeid, &usertype, &country, NULL);
		}
	}
	while ((sqlStatus == SQL_SUCCESS) && (sqlCount < 10000));

	if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
	{
		lastError = DynErrmld();
		SqlCloseCs(sqlHandle);
		MsgBoxOK(lastError);
		return;
	}
	SqlCloseCs(sqlHandle);

	if (bExport == false)
	{
		CString strCount;
		strCount.Format(CResString::ex().getStrTblText(IDS_USER_COUNT), sqlCount, totalcount);
		GetDlgItem(IDC_TXT_USERCOUNT)->SetWindowText(strCount);
	}
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::OnUsersList(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	// if headerline clicked, use for sorting
	NMITEMACTIVATE* pNMIA = (NMITEMACTIVATE*)pNMHDR;
	if (pNMIA->iItem == -1)
	{
		int nCol = pNMIA->iSubItem;
		if ((nCol >= 0) && (nCol <= 5))
		{
		}
	}
	else
	{
		int intRow = pNMIA->iItem;
		CString strSelectedUserID = m_lstUsers.GetItemText(intRow, 0);
		LoadUserDetails(atoi(strSelectedUserID.GetBuffer()));
		LoadAllGroups(-1, -1);
		LoadUserGroups(-1, -1);
		LoadUserParams(-1, -1);
	}
}

void PDlgUserManager::OnBtnClickedUsersReport()
{
	// disable the report button to prevent multiple clicks
	GetDlgItem(IDC_BTN_REPORT_USERS)->EnableWindow(FALSE);

	// get a file name to export
	CFileDialog fileDlg(FALSE, _T("csv"), _T("UserList.csv"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CSV files (*.csv)|*.csv|All files (*.*)|*.*||"));
	// special feature: add combobox to select column separator
	int comboboxID = 1001;
	fileDlg.AddComboBox(comboboxID);
	fileDlg.AddControlItem(comboboxID, 0, "CSV with Semicolon (;)"); // default
	fileDlg.AddControlItem(comboboxID, 1, "CSV with Comma (,)");
	fileDlg.SetSelectedControlItem(comboboxID, 0);
	
	if (fileDlg.DoModal() != IDOK)
	{
		GetDlgItem(IDC_BTN_REPORT_USERS)->EnableWindow(TRUE);
		return;
	}
	CString strCSVFile = fileDlg.GetPathName();
	CString strFiletype = fileDlg.GetFileExt();
	DWORD selectedSeparator = 0;
	fileDlg.GetSelectedControlItem(comboboxID, selectedSeparator);
	if (selectedSeparator == 0)
	{
		m_strSeparator = ";";
	}
	else
	{
		m_strSeparator = ",";
	}

	// Create CSV file and write header row
	std::ofstream outputFile(strCSVFile.GetBuffer(), std::ofstream::out);
	if (outputFile.is_open() == false)
	{
		MsgBoxOK("Failed to open CSV file for writing.");
		GetDlgItem(IDC_BTN_REPORT_USERS)->EnableWindow(TRUE);
		return;
	}
	BeginWaitCursor();

	// get the current list of users
	LoadUsers(true, &outputFile);

	// close File
	outputFile.close();

	EndWaitCursor();

	// open excel with the created csv file
	ShellExecute(NULL, _T("open"), strCSVFile.GetBuffer(), NULL, NULL, SW_SHOW);

	// re-enable the report button
	GetDlgItem(IDC_BTN_REPORT_USERS)->EnableWindow(TRUE);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::LoadUserDetails(int iUserID)
{
	long sqlStatus;
	CString sqlQuery;
	CString lastError;

	// reset at first all details
	ResetUserDetails(1);

	if (iUserID <= 0)
	{
		return;
	}

	long userid;
	char username[21];
	char password[9];
	char pwd_enc[255];
	char surname[31];
	char forename[21];
	char emailuser[51];
	short usertypeid;
	char country[3];
	char ntuser[51];
	char description[255];
	char reference[41];
	short changepw;
	short activ;
	short external;
	long lastlogin;
	long lastpwchange;

	sqlQuery.Format("select userid,username,password,pwd_enc,surname,forename,emailuser,usertypeid,country,ntuser,description,reference,changepw,activ,external,lastlogin,lastpwchange from ppuser where userid=%d", iUserID);
	sqlStatus = SqlRead(sqlQuery, &userid, &username, &password, &pwd_enc, &surname, &forename, &emailuser, &usertypeid, &country, &ntuser, &description, &reference, &changepw, &activ, &external, &lastlogin, &lastpwchange, NULL);
	if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
	{
		lastError = DynErrmld();
		MsgBoxOK(lastError);
		return;
	}
	// show user details
	CString strTmp;
	strTmp.Format("%d", userid);
	GetDlgItem(IDC_EDT_USERID)->SetWindowText(strTmp);
	GetDlgItem(IDC_EDT_USERNAME)->SetWindowText(CString(username).Trim());
	m_chkUserActive.SetCheck(activ);
	strTmp.Format("%d", lastlogin);
	GetDlgItem(IDC_EDT_LOGINDATE)->SetWindowText(strTmp);
	m_strOrgPasswordPHX = CString(password); // do not trim, because its fixed length (8)
	m_strOrgPasswordAES = CString(pwd_enc).Trim();
	GetDlgItem(IDC_EDT_PASSWORD)->SetWindowText("********");
	m_chkCryptPHX.SetCheck((CString(password).Trim().IsEmpty() == TRUE) ? 0 : 1);
	m_chkCryptAES.SetCheck((CString(pwd_enc).Trim().IsEmpty() == TRUE) ? 0 : 1);
	m_chkPwdChange.SetCheck(changepw);
	strTmp.Format("%d", lastpwchange);
	GetDlgItem(IDC_EDT_PWDCHANGEDATE)->SetWindowText(strTmp);
	GetDlgItem(IDC_EDT_SURNAME)->SetWindowText(CString(surname).Trim());
	GetDlgItem(IDC_EDT_FORENAME)->SetWindowText(CString(forename).Trim());
	SetUsertype(usertypeid);
	SetCountry(country);
	GetDlgItem(IDC_EDT_EMAIL)->SetWindowText(CString(emailuser).Trim());
	m_chkExternal.SetCheck(external);
	GetDlgItem(IDC_EDT_NTUSER)->SetWindowText(CString(ntuser).Trim());
	GetDlgItem(IDC_EDT_DESC)->SetWindowText(CString(description).Trim());
	GetDlgItem(IDC_EDT_REFERENCE)->SetWindowText(CString(reference).Trim());
	//done
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::SetUsertype(short iUsertype)
{
	int nCount = m_cmbUserType.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if (m_cmbUserType.GetItemData(i) == iUsertype)
		{
			m_cmbUserType.SetCurSel(i);
			return;
		}
	}
	m_cmbUserType.SetCurSel(-1);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::SetCountry(CString sCountry)
{
	int nCount = m_cmbCountry.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CString strItem;
		m_cmbCountry.GetLBText(i, strItem);
		if (strItem.Left(2).CompareNoCase(sCountry) == 0)
		{
			m_cmbCountry.SetCurSel(i);
			return;
		}
	}
	m_cmbCountry.SetCurSel(-1);
}

/**---------------------------------------------------------------------------*/
bool PDlgUserManager::AllowedChars(CString strInput, bool ext)
{
	bool ret = true;
	std::string allowed = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-_";
	if (ext == true)
	{
		allowed += "!()=+#,;:{}[] ";
	}
	for (int i = 0; i < strInput.GetLength(); i++)
	{
		char ch = strInput.GetAt(i);
		if (allowed.find(ch) == std::string::npos)
		{
			ret = false;
			break;
		}
	}
	return ret;
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::OnBtnUserNew()
{
	ResetUserDetails(2);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::OnBtnUserSave()
{
	long sqlStatus;
	CString sqlQuery;
	CString lastError;

	if (m_intUserState == 0)
	{
		MsgBoxOK("You need to select a user first or create a new one before saving.");
		return;
	}
	// get all data from dialog
	CString strUserID;
	GetDlgItem(IDC_EDT_USERID)->GetWindowText(strUserID);
	long iUserID = atoi(strUserID.GetBuffer());
	CString strUsername;
	GetDlgItem(IDC_EDT_USERNAME)->GetWindowText(strUsername);
	int nUserActive = m_chkUserActive.GetCheck();
	CString strPassword;
	GetDlgItem(IDC_EDT_PASSWORD)->GetWindowText(strPassword);
	int nCryptPHX = m_chkCryptPHX.GetCheck();
	int nCryptAES = m_chkCryptAES.GetCheck();
	CString strPwdEncPHX = "";
	CString strPwdEncAES = "";
	int nPwdChange = m_chkPwdChange.GetCheck();
	CString strSurname;
	GetDlgItem(IDC_EDT_SURNAME)->GetWindowText(strSurname);
	CString strForename;
	GetDlgItem(IDC_EDT_FORENAME)->GetWindowText(strForename);
	int nUsertypeSel = m_cmbUserType.GetCurSel();
	int nUsertypeID = 0;
	if (nUsertypeSel != CB_ERR)
	{
		nUsertypeID = (int)m_cmbUserType.GetItemData(nUsertypeSel);
	}
	CString strCountry = "";
	int nCountrySel = m_cmbCountry.GetCurSel();
	if (nCountrySel != CB_ERR)
	{
		CString strCountryItem;
		m_cmbCountry.GetLBText(nCountrySel, strCountryItem);
		strCountry = strCountryItem.Left(2);
	}
	CString strEmail;
	GetDlgItem(IDC_EDT_EMAIL)->GetWindowText(strEmail);
	int nExternal = m_chkExternal.GetCheck();
	CString strNTUser;
	GetDlgItem(IDC_EDT_NTUSER)->GetWindowText(strNTUser);
	CString strDesc;
	GetDlgItem(IDC_EDT_DESC)->GetWindowText(strDesc);
	CString strReference;
	GetDlgItem(IDC_EDT_REFERENCE)->GetWindowText(strReference);

	bool bNewUser = (m_intUserState == 2);

	// verify mandatory fields
	if ((bNewUser == false) && (iUserID <= 0))
	{
		MsgBoxOK("User ID is missing.");
		return;
	}
	if (strUsername.GetLength() == 0)
	{
		MsgBoxOK("Username is missing.");
		return;
	}
	if (AllowedChars(strUsername, false) == false)
	{
		MsgBoxOK("Username contains invalid characters. Only alphanumeric characters and underscore (_), point (.) or minus (-) are allowed.");
		return;
	}
	if ((bNewUser == true) && (strPassword.GetLength() == 0))
	{
		MsgBoxOK("Password is missing.");
		return;
	}
	if (strSurname.GetLength() == 0)
	{
		MsgBoxOK("Surname is missing.");
		return;
	}
	if (strForename.GetLength() == 0)
	{
		MsgBoxOK("Forename is missing.");
		return;
	}
	if (nUsertypeID == 0)
	{
		MsgBoxOK("User type is unknown or not selected.");
		return;
	}
	if (strCountry.GetLength() == 0)
	{
		MsgBoxOK("Country is not selected.");
		return;
	}
	if (strEmail.GetLength() == 0)
	{
		MsgBoxOK("Email is missing.");
		return;
	}
	// verify uniqueness of username and email for new user
	if (bNewUser == true)
	{
		sqlQuery.Format("SELECT CAST(COUNT(*) AS INTEGER) FROM ppuser WHERE username='%s'", strUsername.GetBuffer());
		long countUsername = 0;
		sqlStatus = SqlRead(sqlQuery, &countUsername, NULL);
		if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
		{
			lastError = DynErrmld();
			MsgBoxOK(lastError);
			return;
		}
		if (countUsername > 0)
		{
			MsgBoxOK("Username already exists. Please choose another username.");
			return;
		}
		if (strEmail != _T("-"))
		{
			sqlQuery.Format("SELECT CAST(COUNT(*) AS INTEGER) FROM ppuser WHERE emailuser='%s'", strEmail.GetBuffer());
			long countEmail = 0;
			sqlStatus = SqlRead(sqlQuery, &countEmail, NULL);
			if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
			{
				lastError = DynErrmld();
				MsgBoxOK(lastError);
				return;
			}
			if (countEmail > 0)
			{
				MsgBoxOK("Email address already exists. Please choose another email address.");
				return;
			}
		}
	}
	else
	{
		// for existing user, check if username or email was changed and verify uniqueness
		CString strOrigUsername, strOrigEmail;
		char sUsername[21], sEmail[51];
		sqlQuery.Format("SELECT username, emailuser FROM ppuser WHERE userid=%d", iUserID);
		sqlStatus = SqlRead(sqlQuery, &sUsername, &sEmail, NULL);
		if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
		{
			lastError = DynErrmld();
			MsgBoxOK(lastError);
			return;
		}
		strOrigUsername = CString(sUsername).Trim();
		strOrigEmail = CString(sEmail).Trim();
		if (strOrigUsername.Compare(strUsername) != 0)
		{
			// username changed, verify uniqueness
			sqlQuery.Format("SELECT CAST(COUNT(*) AS INTEGER) FROM ppuser WHERE username='%s' AND userid<>%d", strUsername.GetBuffer(), iUserID);
			long countUsername = 0;
			sqlStatus = SqlRead(sqlQuery, &countUsername, NULL);
			if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
			{
				lastError = DynErrmld();
				MsgBoxOK(lastError);
				return;
			}
			if (countUsername > 0)
			{
				MsgBoxOK("Username already exists for different user. Please choose another username.");
				return;
			}
		}
		if (strOrigEmail.Compare(strEmail) != 0 && strEmail != _T("-"))
		{
			// email changed, verify uniqueness
			sqlQuery.Format("SELECT CAST(COUNT(*) AS INTEGER) FROM ppuser WHERE LOWER(emailuser)='%s' AND userid<>%d", strEmail.MakeLower().GetBuffer(), iUserID);
			long countEmail = 0;
			sqlStatus = SqlRead(sqlQuery, &countEmail, NULL);
			if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
			{
				lastError = DynErrmld();
				MsgBoxOK(lastError);
				return;
			}
			if (countEmail > 0)
			{
				MsgBoxOK("Email address already exists for different user. Please choose another email address.");
				return;
			}
		}
	}
	// verify password
	if (strPassword.Compare("********") != 0)
	{
		// a new password was entered
		if (AllowedChars(strPassword, true) == false)
		{
			MsgBoxOK("Password contains invalid characters. Only alphanumeric characters and special characters are allowed.");
			return;
		}
		// force changepw flag
		nPwdChange = 1;
		m_chkPwdChange.SetCheck(BST_CHECKED);
		// verify encryption selections
		if ((nCryptPHX == 0 && nCryptAES == 0) || (nCryptPHX == 1 && nCryptAES == 1))
		{
			// both selected, verify password length
			if (strPassword.GetLength() > 8)
			{
				// for longer passwords
				nCryptPHX = 0; // disable PHX encryption
				nCryptAES = 1; // enable AES encryption
			}
			if (strPassword.GetLength() <= 8)
			{
				// for shorter passwords
				nCryptPHX = 1; // enable PHX encryption
				nCryptAES = 0; // disable AES encryption
			}
			m_chkCryptPHX.SetCheck(nCryptPHX);
			m_chkCryptAES.SetCheck(nCryptAES);
		}
		if (nCryptPHX == 1)
		{
			if (strPassword.GetLength() > 8)
			{
				MsgBoxOK("Password must be <= 8 characters long for internal PHX encryption.");
				return;
			}
			UADM::CUADMLoginManager uadm;
			strPwdEncPHX = uadm.encryptPassword(strPassword.GetBuffer(), strUsername.GetBuffer(),1).c_str();
		}
		if (nCryptAES == 1)
		{
			if (strPassword.GetLength() < 8)
			{
				MsgBoxOK("Password must be at least 8 characters long for standard AES encryption.");
				return;
			}
			UADM::CUADMLoginManager uadm;
			strPwdEncAES = uadm.encryptPassword(strPassword.GetBuffer(), strUsername.GetBuffer(), 2).c_str();
		}
	}
	else
	{
		// keep original password
		strPwdEncPHX = m_strOrgPasswordPHX;
		strPwdEncAES = m_strOrgPasswordAES;
	}
	// store into database
	if (bNewUser == true)
	{
		// create new userid and insert into ppuser
		sqlQuery.Format("INSERT INTO ppuser (username, password, surname, forename, description, emailuser"
			", changepw, activ, ntuser, reference, external, pwd_enc, country, usertypeid)"
			"VALUES ('%s', '%s', '%s', '%s', '%s', '%s', %d, %d, '%s', '%s', %d, '%s', '%s', %d)",
			strUsername.GetBuffer(), strPwdEncPHX.GetBuffer(), strSurname.GetBuffer(), strForename.GetBuffer(), 
			strDesc.GetBuffer(), strEmail.GetBuffer(), nPwdChange, nUserActive, strNTUser.GetBuffer(), 
			strReference.GetBuffer(), nExternal, strPwdEncAES.GetBuffer(), strCountry.GetBuffer(), nUsertypeID);
		sqlStatus = SqlExecuteImm(sqlQuery);
		if (sqlStatus != SQL_SUCCESS)
		{
			lastError = DynErrmld();
			// write log
			SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, ": new username %s failed to insert by operator '%s', error %s", strUsername.GetBuffer(), scmod::ppBLoginData::ex().GetUsername().c_str(), lastError.GetBuffer());
			// show error
			CString strErr;
			strErr.Format("Inserting new user '%s' failed. %s", strUsername.GetBuffer(), lastError.GetBuffer());
			MsgBoxOK(strErr);
			return;
		}
		// write log
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, ": new username %s inserted by operator '%s'", strUsername.GetBuffer(), scmod::ppBLoginData::ex().GetUsername().c_str());
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, ":   new user data: username=%s, surname=%s, forename=%s, usertypeid=%d, country=%s, emailuser=%s, changepw=%d, activ=%d, password=%s, pwd_enc=%s, external=%d, ntuser=%s, description=%s, reference=%s",
			strUsername.GetBuffer(), strSurname.GetBuffer(), strForename.GetBuffer(), nUsertypeID, strCountry.GetBuffer(), strEmail.GetBuffer(), nPwdChange, nUserActive, strPwdEncPHX.GetBuffer(), strPwdEncAES.GetBuffer(),
			nExternal, strNTUser.GetBuffer(), strDesc.GetBuffer(), strReference.GetBuffer());
		// read new userid as this was generated by the database
		sqlQuery.Format("SELECT userid FROM ppuser WHERE username='%s'", strUsername.GetBuffer());
		sqlStatus = SqlRead(sqlQuery, &iUserID, NULL);
		if (sqlStatus != SQL_SUCCESS)
		{
			lastError = DynErrmld();
			// show error
			CString strErr;
			strErr.Format("Reading new user ID for user '%s' failed. %s", strUsername.GetBuffer(), lastError.GetBuffer());
			MsgBoxOK(strErr);
			return;
		}
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, ":   new userid is %d for username %s", iUserID, strUsername.GetBuffer());
	}
	else
	{
		// read original user for logging
		long userid;
		char username[21], password[9], pwd_enc[255], surname[31], forename[21], emailuser[51];
		short usertypeid, changepw, activ, external;
		char country[3], ntuser[51], description[255], reference[41];
		sqlQuery.Format("select userid,username,password,pwd_enc,surname,forename,emailuser,usertypeid,country,ntuser,description,reference,changepw,activ,external from ppuser where userid=%d", iUserID);
		sqlStatus = SqlRead(sqlQuery, &userid, &username, &password, &pwd_enc, &surname, &forename, &emailuser, &usertypeid, &country, &ntuser, &description, &reference, &changepw, &activ, &external, NULL);
		if (sqlStatus != SQL_SUCCESS)
		{
			lastError = DynErrmld();
			CString strErr;
			strErr.Format("Reading original user data for user ID %d failed. %s", iUserID, lastError.GetBuffer());
			MsgBoxOK(strErr);
			return;
		}
		// update ppuser with current userid
		sqlQuery.Format("UPDATE ppuser SET username='%s', password='%s', pwd_enc='%s', changepw=%d, activ=%d"
			", surname='%s', forename='%s', usertypeid=%d, country='%s', emailuser='%s', external=%d"
			", ntuser='%s', description='%s', reference='%s' WHERE userid=%d",
			strUsername.GetBuffer(), strPwdEncPHX.GetBuffer(), strPwdEncAES.GetBuffer(), nPwdChange, nUserActive,
			strSurname.GetBuffer(), strForename.GetBuffer(), nUsertypeID, strCountry.GetBuffer(), strEmail.GetBuffer(),
			nExternal, strNTUser.GetBuffer(), strDesc.GetBuffer(), strReference.GetBuffer(), iUserID);
		sqlStatus = SqlExecuteImm(sqlQuery);
		if (sqlStatus != SQL_SUCCESS)
		{
			lastError = DynErrmld();
			// write log
			SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, ": update username %s (userid %d) failed by operator '%s', error %s", strUsername.GetBuffer(), iUserID, scmod::ppBLoginData::ex().GetUsername().c_str(), lastError.GetBuffer());
			// show error
			CString strErr;
			strErr.Format("Updating user '%s' failed. %s", strUsername.GetBuffer(), lastError.GetBuffer());
			MsgBoxOK(strErr);
			return;
		}
		// write log
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, ": update username %s (userid %d) by operator '%s'", strUsername.GetBuffer(), iUserID, scmod::ppBLoginData::ex().GetUsername().c_str());
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, ":   original user data: username=%s, surname=%s, forename=%s, usertypeid=%d, country=%s, emailuser=%s, changepw=%d, activ=%d, password=%s, pwd_enc=%s, external=%d, ntuser=%s, description=%s, reference=%s",
			CString(username).Trim().GetBuffer(), CString(surname).Trim().GetBuffer(), CString(forename).Trim().GetBuffer(), usertypeid, CString(country).Trim().GetBuffer(), CString(emailuser).Trim().GetBuffer(),
			changepw, activ, CString(password).Trim().GetBuffer(), CString(pwd_enc).Trim().GetBuffer(), external, CString(ntuser).Trim().GetBuffer(), CString(description).Trim().GetBuffer(), CString(reference).Trim().GetBuffer());
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, ":   changed user data : username=%s, surname=%s, forename=%s, usertypeid=%d, country=%s, emailuser=%s, changepw=%d, activ=%d, password=%s, pwd_enc=%s, external=%d, ntuser=%s, description=%s, reference=%s",
			strUsername.GetBuffer(), strSurname.GetBuffer(), strForename.GetBuffer(), nUsertypeID, strCountry.GetBuffer(), strEmail.GetBuffer(), nPwdChange, nUserActive, strPwdEncPHX.GetBuffer(), strPwdEncAES.GetBuffer(),
			nExternal, strNTUser.GetBuffer(), strDesc.GetBuffer(), strReference.GetBuffer());
	}
	MsgBoxOK("User data saved successfully.");
	// reload user list and details
	LoadUsers(false, NULL);
	LoadUserDetails(iUserID);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::OnBtnUserDelete()
{
	if (m_intUserState != 1)
	{
		MsgBoxOK("You need to select a user first before deleting.");
		return;
	}
	CString strUserID;
	CString strUsername;
	GetDlgItem(IDC_EDT_USERID)->GetWindowText(strUserID);
	GetDlgItem(IDC_EDT_USERNAME)->GetWindowText(strUsername);
	long iUserID = atoi(strUserID.GetBuffer());
	if (iUserID <= 0)
	{
		MsgBoxOK("User ID is missing.");
		return;
	}
	long sqlStatus;
	CString sqlQuery;
	CString lastError;
	long lGroupCount;
	long lParaCount;
	// verify if user is used in other tables
	sqlQuery.Format("SELECT CAST(COUNT(*) AS INTEGER) FROM ppcorappusergroup WHERE userid=%d", iUserID);
	sqlStatus = SqlRead(sqlQuery, &lGroupCount, NULL);
	if (sqlStatus != SQL_SUCCESS)
	{
		lastError = DynErrmld();
		MsgBoxOK(lastError);
		return;
	}
	sqlQuery.Format("SELECT CAST(COUNT(*) AS INTEGER) FROM ppcorappbruser WHERE userid=%d", iUserID);
	sqlStatus = SqlRead(sqlQuery, &lParaCount, NULL);
	if (sqlStatus != SQL_SUCCESS)
	{
		lastError = DynErrmld();
		MsgBoxOK(lastError);
		return;
	}
	// confirm delete
	CString strConfirm;
	strConfirm.Format("The user '%s' is granted to %d roles and mapped to %d user parameters. "
		"The user will be disabled but not deleted.\r\n"
		"The deletion process will happen after 3month automatically with some historization to fullfill audit-requirements.\r\n\r\n"
		"Are you sure to disable the user ID %d?", strUsername.GetBuffer(), lGroupCount, lParaCount, iUserID);
	if (MsgBoxYesNo(strConfirm) != IDYES)
	{
		return;
	}

	// disable user only
	sqlQuery.Format("UPDATE ppuser SET activ=0 WHERE userid=%d", iUserID);
	sqlStatus = SqlExecuteImm(sqlQuery);
	if (sqlStatus != SQL_SUCCESS)
	{
		lastError = DynErrmld();
		// write log
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, ": disable username %s (userid %d) by operator '%s' failed, %s", strUsername.GetBuffer(), iUserID, scmod::ppBLoginData::ex().GetUsername().c_str(), lastError.GetBuffer());
		// show error
		CString strErr;
		strErr.Format("Disabling user '%s' failed. %s", strUsername.GetBuffer(), lastError.GetBuffer());
		MsgBoxOK(strErr);
		return;
	}
	// write log
	SLog::ex().write(__FUNCTION__, VAccess::RET_OK, ": disable username %s (userid %d) by operator '%s'", strUsername.GetBuffer(), iUserID, scmod::ppBLoginData::ex().GetUsername().c_str());

	// done
	MsgBoxOK("User disabled successfully.");
	LoadUsers(false, NULL);
	LoadUserDetails(iUserID);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::OnApplicationSelChange()
{
	// application selection changed
	ResetAllGroups(true);
	ResetUserGroups(true);
	ResetUserParams(true);

	int nAppSel = m_cmbApplication.GetCurSel();
	if (nAppSel != CB_ERR)
	{
		CString strUserID;
		GetDlgItem(IDC_EDT_USERID)->GetWindowText(strUserID);
		int userid = atoi(strUserID.GetBuffer());
		// get the application id
		int applicationid = (int)m_cmbApplication.GetItemData(nAppSel);
		LoadAllGroups(userid, applicationid);
		LoadUserGroups(userid, applicationid);
		LoadUserParams(userid, applicationid);
		if (nAppSel < (int)m_vecAppDesc.size())
		{
			m_AppDescTip.UpdateTipText(m_vecAppDesc[nAppSel], &m_cmbApplication);
		}
	}
}

/**---------------------------------------------------------------------------*/
BOOL PDlgUserManager::OnApplicationDescTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	id;
	TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;
	UINT_PTR nID = pNMHDR->idFrom;

	if (nID == (UINT_PTR)m_cmbApplication.GetDlgCtrlID())
	{
		int nAppSel = m_cmbApplication.GetCurSel();
		if (nAppSel != CB_ERR)
		{
			if (nAppSel < (int)m_vecAppDesc.size())
			{
				strcpy_s(pTTT->szText, sizeof(pTTT->szText), m_vecAppDesc[nAppSel].GetBuffer());
			}
		}
	}
	*pResult = 0;
	return TRUE;
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::LoadAllGroups(int userid, int applicationid)
{
	long sqlStatus;
	int  sqlHandle;
	CString sqlQuery;
	long sqlCount;
	CString lastError;
	CString strFilter;

	ResetAllGroups(false);
	if (applicationid < 0)
	{
		// get the application id from combobox
		int nAppSel = m_cmbApplication.GetCurSel();
		if (nAppSel != CB_ERR)
		{
			applicationid = (int)m_cmbApplication.GetItemData(nAppSel);
		}
	}
	if (applicationid <= 0)
	{
		return;
	}
	// create filter for where clause
	strFilter = "";
	// get filter values
	CString fGroupID;
	CString fGroupName;
	GetDlgItem(IDC_EDT_FILTER_ALLGROUPID)->GetWindowText(fGroupID);
	GetDlgItem(IDC_EDT_FILTER_ALLGROUPNAME)->GetWindowText(fGroupName);
	if (fGroupID.IsEmpty() == FALSE)
	{
		// make sure, that only numbers are used
		int fgid = atoi(fGroupID.GetBuffer());
		CString strTmp;
		strTmp.Format("%d", fgid);
		strFilter.Format(" AND g.groupid = %s", strTmp.GetBuffer());
	}
	if (fGroupName.IsEmpty() == FALSE)
	{
		strFilter.AppendFormat(" AND LOWER(g.name) LIKE '%%%s%%'", fGroupName.MakeLower());
	}

	// load all groups for the selected application
	sqlQuery.Format("SELECT FIRST 50 DISTINCT a.groupid, g.name from ppappright a, ppgroup g WHERE a.applicationid=%d AND a.groupid>0 AND a.groupid=g.groupid%s ORDER BY g.name", applicationid, strFilter.GetBuffer());
	sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery);
	if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
	{
		lastError = DynErrmld();
		MsgBoxOK(lastError);
		return;
	}
	short groupid;
	char name[31];
	sqlStatus = SqlFetch(sqlHandle, 1, &groupid, &name, NULL);
	if (sqlStatus == SQL_NOTFOUND)
	{
		lastError = "No groups for this applications found!";
		SqlCloseCs(sqlHandle);
		MsgBoxOK(lastError);
		return;
	}
	sqlCount = 0;
	do
	{
		// add into all groups listctrl
		CString strTmp;
		strTmp.Format("%d", groupid);
		int nItem = m_lstAllGroups.InsertItem(sqlCount, strTmp);
		m_lstAllGroups.SetItemText(nItem, 1, CString(name).Trim());
		// get next
		sqlCount++;
		sqlStatus = SqlFetch(sqlHandle, 1, &groupid, &name, NULL);
	} 
	while ((sqlStatus == SQL_SUCCESS) && (sqlCount < 10000));

	SqlCloseCs(sqlHandle);

	// remove the entries which are already assigned to the user
	if (userid < 0)
	{
		CString strUserID;
		GetDlgItem(IDC_EDT_USERID)->GetWindowText(strUserID);
		userid = atoi(strUserID.GetBuffer());
	}
	if (userid > 0)
	{
		sqlQuery.Format("SELECT groupid from ppcorappusergroup WHERE applicationid=%d and userid=%d", applicationid, userid);
		sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery);
		if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
		{
			lastError = DynErrmld();
			MsgBoxOK(lastError);
			return;
		}
		sqlStatus = SqlFetch(sqlHandle, 1, &groupid, NULL);
		if (sqlStatus == SQL_SUCCESS)
		{
			do
			{
				// find and remove from all groups listctrl
				int nItemCount = m_lstAllGroups.GetItemCount();
				for (int i = 0; i < nItemCount; i++)
				{
					CString strGroupID = m_lstAllGroups.GetItemText(i, 0);
					if (atoi(strGroupID.GetBuffer()) == groupid)
					{
						m_lstAllGroups.DeleteItem(i);
						break;
					}
				}
				// get next
				sqlStatus = SqlFetch(sqlHandle, 1, &groupid, NULL);
			}
			while (sqlStatus == SQL_SUCCESS);
		}
		SqlCloseCs(sqlHandle);
	}
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::LoadUserGroups(int userid, int applicationid)
{
	long sqlStatus;
	int  sqlHandle;
	CString sqlQuery;
	long sqlCount;
	CString lastError;
	CString strFilter;

	ResetUserGroups(false);
	if (applicationid < 0)
	{
		// get the application id from combobox
		int nAppSel = m_cmbApplication.GetCurSel();
		if (nAppSel != CB_ERR)
		{
			applicationid = (int)m_cmbApplication.GetItemData(nAppSel);
		}
	}
	if (userid < 0)
	{
		CString strUserID;
		GetDlgItem(IDC_EDT_USERID)->GetWindowText(strUserID);
		userid = atoi(strUserID.GetBuffer());
	}
	if ((applicationid <= 0) || (userid <= 0))
	{
		return;
	}
	// create filter for where clause
	strFilter = "";
	// get filter values
	CString fGroupID;
	CString fGroupName;
	GetDlgItem(IDC_EDT_FILTER_USERGROUPID)->GetWindowText(fGroupID);
	GetDlgItem(IDC_EDT_FILTER_USERGROUPNAME)->GetWindowText(fGroupName);
	if (fGroupID.IsEmpty() == FALSE)
	{
		// make sure, that only numbers are used
		int fgid = atoi(fGroupID.GetBuffer());
		CString strTmp;
		strTmp.Format("%d", fgid);
		strFilter.Format(" AND g.groupid = %s", strTmp.GetBuffer());
	}
	if (fGroupName.IsEmpty() == FALSE)
	{
		strFilter.AppendFormat(" AND LOWER(g.name) LIKE '%%%s%%'", fGroupName.MakeLower().GetBuffer());
	}
	// load user groups for the selected application
	sqlQuery.Format("SELECT a.groupid, g.name from ppcorappusergroup a, ppgroup g WHERE a.applicationid=%d and a.userid=%d and a.groupid=g.groupid%s ORDER BY g.name", applicationid, userid, strFilter.GetBuffer());
	sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery);
	if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
	{
		lastError = DynErrmld();
		MsgBoxOK(lastError);
		return;
	}
	short groupid;
	char name[31];
	sqlStatus = SqlFetch(sqlHandle, 1, &groupid, &name, NULL);
	if (sqlStatus != SQL_SUCCESS)
	{
		lastError = "No groups for this application and user found!";
		SqlCloseCs(sqlHandle);
		return;
	}
	sqlCount = 0;
	do
	{
		// add into user groups listctrl
		CString strTmp;
		strTmp.Format("%d", groupid);
		int nItem = m_lstUserGroups.InsertItem(sqlCount, strTmp);
		m_lstUserGroups.SetItemText(nItem, 1, CString(name).Trim());
		// get next
		sqlCount++;
		sqlStatus = SqlFetch(sqlHandle, 1, &groupid, &name, NULL);
	}
	while ((sqlStatus == SQL_SUCCESS) && (sqlCount < 10000));

	SqlCloseCs(sqlHandle);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::OnBtnPermitGroup()
{
	// check first if user, app and 1-n groups are selected
	CString strUserID;
	GetDlgItem(IDC_EDT_USERID)->GetWindowText(strUserID);
	long userid = atoi(strUserID.GetBuffer());
	if (userid <= 0)
	{
		MsgBoxOK("Please select a user first.");
		return;
	}
	if (m_chkUserActive.GetCheck() == 0)
	{
		MsgBoxOK("Cannot permit groups to a disabled user. Please enable the user first.");
		return;
	}
	CString strAppID;
	int nAppSel = m_cmbApplication.GetCurSel();
	if (nAppSel == CB_ERR)
	{
		MsgBoxOK("Please select an application first.");
		return;
	}
	long applicationid = (long)m_cmbApplication.GetItemData(nAppSel);
	if (applicationid <= 0)
	{
		MsgBoxOK("Please select a valid application first.");
		return;
	}
	POSITION pos = m_lstAllGroups.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MsgBoxOK("Please select at least one group to permit.");
		return;
	}
	long sqlStatus;
	CString sqlQuery;
	CString lastError;
	// loop through all selected groups and add to user
	while (pos != NULL)
	{
		int nItem = m_lstAllGroups.GetNextSelectedItem(pos);
		CString strGroupID = m_lstAllGroups.GetItemText(nItem, 0);
		long groupid = atoi(strGroupID.GetBuffer());
		if (groupid > 0)
		{
			// insert into ppcorappusergroup
			sqlQuery.Format("INSERT INTO ppcorappusergroup (applicationid, userid, groupid) VALUES (%d, %d, %d)", applicationid, userid, groupid);
			sqlStatus = SqlExecuteImm(sqlQuery);
			if (sqlStatus != SQL_SUCCESS)
			{
				lastError = DynErrmld();
				// write log
				SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, ": permit group %d to user %d for application %d by operator '%s' failed, %s", groupid, userid, applicationid, scmod::ppBLoginData::ex().GetUsername().c_str(), lastError.GetBuffer());
				// show error
				CString strErr;
				strErr.Format("Permitting group %d to user %d for application %d failed. %s", groupid, userid, applicationid, lastError.GetBuffer());
				MsgBoxOK(strErr);
				return;
			}
			// write log
			SLog::ex().write(__FUNCTION__, VAccess::RET_OK, ": permit group %d to user %d for application %d by operator '%s'", groupid, userid, applicationid, scmod::ppBLoginData::ex().GetUsername().c_str());
		}
	}
	LoadAllGroups(userid, applicationid);
	LoadUserGroups(userid, applicationid);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::OnBtnRevokeGroup()
{
	// check first if user, app and 1-n groups are selected
	CString strUserID;
	GetDlgItem(IDC_EDT_USERID)->GetWindowText(strUserID);
	long userid = atoi(strUserID.GetBuffer());
	if (userid <= 0)
	{
		MsgBoxOK("Please select a user first.");
		return;
	}
	CString strAppID;
	int nAppSel = m_cmbApplication.GetCurSel();
	if (nAppSel == CB_ERR)
	{
		MsgBoxOK("Please select an application first.");
		return;
	}
	long applicationid = (long)m_cmbApplication.GetItemData(nAppSel);
	if (applicationid <= 0)
	{
		MsgBoxOK("Please select a valid application first.");
		return;
	}
	POSITION pos = m_lstUserGroups.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MsgBoxOK("Please select at least one group to revoke.");
		return;
	}
	long sqlStatus;
	CString sqlQuery;
	CString lastError;
	// loop through all selected groups and remove from user
	while (pos != NULL)
	{
		int nItem = m_lstUserGroups.GetNextSelectedItem(pos);
		CString strGroupID = m_lstUserGroups.GetItemText(nItem, 0);
		long groupid = atoi(strGroupID.GetBuffer());
		if (groupid > 0)
		{
			// delete from ppcorappusergroup
			sqlQuery.Format("DELETE FROM ppcorappusergroup WHERE applicationid=%d AND userid=%d AND groupid=%d", applicationid, userid, groupid);
			sqlStatus = SqlExecuteImm(sqlQuery);
			if (sqlStatus != SQL_SUCCESS)
			{
				lastError = DynErrmld();
				// write log
				SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, ": revoke group %d from user %d for application %d by operator '%s' failed, %s", groupid, userid, applicationid, scmod::ppBLoginData::ex().GetUsername().c_str(), lastError.GetBuffer());
				// show error
				CString strErr;
				strErr.Format("Revoking group %d from user %d for application %d failed. %s", groupid, userid, applicationid, lastError.GetBuffer());
				MsgBoxOK(strErr);
				return;
			}
			// write log
			SLog::ex().write(__FUNCTION__, VAccess::RET_OK, ": revoke group %d from user %d for application %d by operator '%s'", groupid, userid, applicationid, scmod::ppBLoginData::ex().GetUsername().c_str());
		}
	}
	LoadAllGroups(userid, applicationid);
	LoadUserGroups(userid, applicationid);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::LoadUserParams(int userid, int applicationid)
{
	long sqlStatus;
	int  sqlHandle;
	CString sqlQuery;
	long sqlCount;
	CString lastError;
	CString strFilter;

	ResetUserParams(false);
	ResetUserParamDetails(0);
	if (applicationid < 0)
	{
		// get the application id from combobox
		int nAppSel = m_cmbApplication.GetCurSel();
		if (nAppSel != CB_ERR)
		{
			applicationid = (int)m_cmbApplication.GetItemData(nAppSel);
		}
	}
	if (userid < 0)
	{
		CString strUserID;
		GetDlgItem(IDC_EDT_USERID)->GetWindowText(strUserID);
		userid = atoi(strUserID.GetBuffer());
	}
	if ((applicationid <= 0) || (userid <= 0))
	{
		return;
	}
	// create filter for where clause
	strFilter = "";
	// get filter values
	CString fBranch;
	CString fParameter;
	CString fValue;
	GetDlgItem(IDC_EDT_FILTER_BRANCH)->GetWindowText(fBranch);
	GetDlgItem(IDC_EDT_FILTER_PARAMETER)->GetWindowText(fParameter);
	GetDlgItem(IDC_EDT_FILTER_VALUE)->GetWindowText(fValue);
	if (fBranch.IsEmpty() == FALSE)
	{
		strFilter.Format(" AND LOWER(rb.name) like '%%%s%%'", fBranch.MakeLower().GetBuffer());
	}
	if (fParameter.IsEmpty() == FALSE)
	{
		strFilter.AppendFormat(" AND LOWER(up.name) LIKE '%%%s%%'", fParameter.MakeLower().GetBuffer());
	}
	if (fValue.IsEmpty() == FALSE)
	{
		strFilter.AppendFormat(" AND c.value LIKE '%%%s%%'", fValue);
	}

	// load all userparameters for the selected application and user
	sqlQuery.Format("SELECT c.branchregionid, rb.name, c.userparaid, up.name, c.userparavalue "
		"FROM ppcorappbruser c, ppregionbranch rb, ppuserpara up "
		"WHERE c.branchregionid=rb.regionbranchid AND c.userparaid=up.userparaid AND c.applicationid=%d "
		"AND c.userid=%d%s order by c.branchregionid, c.userparaid", applicationid, userid, strFilter.GetBuffer());
	sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery);
	if ((sqlStatus != SQL_SUCCESS) && (sqlStatus != SQL_NOTFOUND))
	{
		lastError = DynErrmld();
		MsgBoxOK(lastError);
		return;
	}
	short branchid;
	char branchname[101];
	short userparaid;
	char userparaname[101];
	char paravalue[255];
	sqlStatus = SqlFetch(sqlHandle, 1, &branchid, &branchname, &userparaid, &userparaname, &paravalue, NULL);
	if (sqlStatus == SQL_NOTFOUND)
	{
		lastError = "No user parameters for this applications found!";
		SqlCloseCs(sqlHandle);
		return;
	}
	sqlCount = 0;
	do
	{
		// add into userpara listctrl
		CString strTmp;
		strTmp.Format("%02d - ", branchid);
		strTmp.Append(CString(branchname).Trim());
		int nItem = m_lstUserParams.InsertItem(sqlCount, strTmp);
		strTmp.Format("%d - ", userparaid);
		strTmp.Append(CString(userparaname).Trim());
		m_lstUserParams.SetItemText(nItem, 1, strTmp);
		m_lstUserParams.SetItemText(nItem, 2, CString(paravalue).Trim());
		// get next
		sqlCount++;
		sqlStatus = SqlFetch(sqlHandle, 1, &branchid, &branchname, &userparaid, &userparaname, &paravalue, NULL);
	} 
	while ((sqlStatus == SQL_SUCCESS) && (sqlCount < 10000));

	SqlCloseCs(sqlHandle);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::OnUserParamsList(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	// if headerline clicked, use for sorting
	NMITEMACTIVATE* pNMIA = (NMITEMACTIVATE*)pNMHDR;
	if (pNMIA->iItem == -1)
	{
		int nCol = pNMIA->iSubItem;
		if ((nCol >= 0) && (nCol <= 3))
		{
		}
	}
	else
	{
		int intRow = pNMIA->iItem;
		CString strSelectedBranchID = m_lstUserParams.GetItemText(intRow, 0).Left(2).Trim();
		CString strSelectedParameterID = m_lstUserParams.GetItemText(intRow, 1).Left(2).Trim();
		CString strSelectedValue = m_lstUserParams.GetItemText(intRow, 2);
		// show user parameter details for editing
		ResetUserParamDetails(1);
		// find branchid in combobox
		int branchid = atoi(strSelectedBranchID.GetBuffer());
		int nCount = m_cmbBranch.GetCount();
		for (int i = 0; i < nCount; i++)
		{
			if (m_cmbBranch.GetItemData(i) == branchid)
			{
				m_cmbBranch.SetCurSel(i);
				break;
			}
		}
		// find userparaid in combobox
		int userparaid = atoi(strSelectedParameterID.GetBuffer());
		nCount = m_cmbUserParam.GetCount();
		for (int i = 0; i < nCount; i++)
		{
			if (m_cmbUserParam.GetItemData(i) == userparaid)
			{
				m_cmbUserParam.SetCurSel(i);
				break;
			}
		}
		// set value
		GetDlgItem(IDC_EDT_PARAMETERVALUE)->SetWindowText(strSelectedValue);
	}
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::OnBtnUserParamNew()
{
	// check first if user, app is selected
	CString strUserID;
	GetDlgItem(IDC_EDT_USERID)->GetWindowText(strUserID);
	long userid = atoi(strUserID.GetBuffer());
	if (userid <= 0)
	{
		MsgBoxOK("Please select a user first.");
		return;
	}
	if (m_chkUserActive.GetCheck() == 0)
	{
		MsgBoxOK("Cannot add user parameters to a disabled user. Please enable the user first.");
		return;
	}
	CString strAppID;
	int nAppSel = m_cmbApplication.GetCurSel();
	if (nAppSel == CB_ERR)
	{
		MsgBoxOK("Please select an application first.");
		return;
	}
	long applicationid = (long)m_cmbApplication.GetItemData(nAppSel);
	if (applicationid <= 0)
	{
		MsgBoxOK("Please select a valid application first.");
		return;
	}
	ResetUserParamDetails(2);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::OnBtnUserParamSave()
{
	// check first if user, app is selected
	CString strUserID;
	GetDlgItem(IDC_EDT_USERID)->GetWindowText(strUserID);
	long userid = atoi(strUserID.GetBuffer());
	if (userid <= 0)
	{
		MsgBoxOK("Please select a user first.");
		return;
	}
	if (m_chkUserActive.GetCheck() == 0)
	{
		MsgBoxOK("Cannot save user parameters to a disabled user. Please enable the user first.");
		return;
	}
	CString strAppID;
	int nAppSel = m_cmbApplication.GetCurSel();
	if (nAppSel == CB_ERR)
	{
		MsgBoxOK("Please select an application first.");
		return;
	}
	long applicationid = (long)m_cmbApplication.GetItemData(nAppSel);
	if (applicationid <= 0)
	{
		MsgBoxOK("Please select a valid application first.");
		return;
	}
	// now check the branch, userpara and value
	int nBranchSel = m_cmbBranch.GetCurSel();
	if (nBranchSel == CB_ERR)
	{
		MsgBoxOK("Please select a branch/region first.");
		return;
	}
	long branchid = (long)m_cmbBranch.GetItemData(nBranchSel);
	if (branchid <= 0)
	{
		MsgBoxOK("Please select a valid branch/region first.");
		return;
	}
	int nUserParaSel = m_cmbUserParam.GetCurSel();
	if (nUserParaSel == CB_ERR)
	{
		MsgBoxOK("Please select a user parameter first.");
		return;
	}
	long userparaid = (long)m_cmbUserParam.GetItemData(nUserParaSel);
	if (userparaid <= 0)
	{
		MsgBoxOK("Please select a valid user parameter first.");
		return;
	}
	CString strParamValue;
	GetDlgItem(IDC_EDT_PARAMETERVALUE)->GetWindowText(strParamValue);
	if (strParamValue.GetLength() == 0)
	{
		MsgBoxOK("Please enter a value for the user parameter.");
		return;
	}
	long sqlStatus;
	CString sqlQuery;
	CString lastError;
	// check if new or existing
	if (m_intUserParamState == 2)
	{
		// new user parameter, insert into ppcorappbruser
		sqlQuery.Format("INSERT INTO ppcorappbruser (applicationid, userid, branchregionid, userparaid, userparavalue) "
			"VALUES (%d, %d, %d, %d, '%s')", applicationid, userid, branchid, userparaid, strParamValue.GetBuffer());
		sqlStatus = SqlExecuteImm(sqlQuery);
		if (sqlStatus != SQL_SUCCESS)
		{
			lastError = DynErrmld();
			// write log
			SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, ": insert user parameter %d for branch %d to user %d for application %d by operator '%s' failed, %s", userparaid, branchid, userid, applicationid, scmod::ppBLoginData::ex().GetUsername().c_str(), lastError.GetBuffer());
			// show error
			CString strErr;
			strErr.Format("Inserting user parameter %d (value %s) for branch %d to user %d for application %d failed. %s", userparaid, strParamValue.GetBuffer(), branchid, userid, applicationid, lastError.GetBuffer());
			MsgBoxOK(strErr);
			return;
		}
		// write log
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, ": insert user parameter %d (value %s) for branch %d to user %d for application %d by operator '%s'", userparaid, strParamValue.GetBuffer(), branchid, userid, applicationid, scmod::ppBLoginData::ex().GetUsername().c_str());
	}
	else if (m_intUserParamState == 1)
	{
		// existing user parameter, update ppcorappbruser
		sqlQuery.Format("UPDATE ppcorappbruser SET userparavalue='%s' WHERE applicationid=%d AND userid=%d "
			"AND branchregionid=%d AND userparaid=%d", strParamValue.GetBuffer(), applicationid, userid, branchid, userparaid);
		sqlStatus = SqlExecuteImm(sqlQuery);
		if (sqlStatus != SQL_SUCCESS)
		{
			lastError = DynErrmld();
			// write log
			SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, ": update user parameter %d for branch %d to user %d for application %d by operator '%s' failed, %s", userparaid, branchid, userid, applicationid, scmod::ppBLoginData::ex().GetUsername().c_str(), lastError.GetBuffer());
			// show error
			CString strErr;
			strErr.Format("Updating user parameter %d (value %s) for branch %d to user %d for application %d failed. %s", userparaid, strParamValue.GetBuffer(), branchid, userid, applicationid, lastError.GetBuffer());
			MsgBoxOK(strErr);
			return;
		}
		// write log
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, ": update user parameter %d (value %s) for branch %d to user %d for application %d by operator '%s'", userparaid, strParamValue.GetBuffer(), branchid, userid, applicationid, scmod::ppBLoginData::ex().GetUsername().c_str());
	}
	MsgBoxOK("User parameter saved successfully.");
	LoadUserParams(userid, applicationid);
}

/**---------------------------------------------------------------------------*/
void PDlgUserManager::OnBtnUserParamDelete()
{
	// check first if user, app is selected
	CString strUserID;
	GetDlgItem(IDC_EDT_USERID)->GetWindowText(strUserID);
	long userid = atoi(strUserID.GetBuffer());
	if (userid <= 0)
	{
		MsgBoxOK("Please select a user first.");
		return;
	}
	CString strAppID;
	int nAppSel = m_cmbApplication.GetCurSel();
	if (nAppSel == CB_ERR)
	{
		MsgBoxOK("Please select an application first.");
		return;
	}
	long applicationid = (long)m_cmbApplication.GetItemData(nAppSel);
	if (applicationid <= 0)
	{
		MsgBoxOK("Please select a valid application first.");
		return;
	}
	// now check the branch, userpara
	int nBranchSel = m_cmbBranch.GetCurSel();
	if (nBranchSel == CB_ERR)
	{
		MsgBoxOK("Please select an entry from the user parameter list for deletion.");
		return;
	}
	long branchid = (long)m_cmbBranch.GetItemData(nBranchSel);
	if (branchid <= 0)
	{
		MsgBoxOK("Please select an entry from the user parameter list for deletion.");
		return;
	}
	int nUserParaSel = m_cmbUserParam.GetCurSel();
	if (nUserParaSel == CB_ERR)
	{
		MsgBoxOK("Please select an entry from the user parameter list for deletion.");
		return;
	}
	long userparaid = (long)m_cmbUserParam.GetItemData(nUserParaSel);
	if (userparaid <= 0)
	{
		MsgBoxOK("Please select an entry from the user parameter list for deletion.");
		return;
	}
	// confirm delete
	CString strConfirm;
	strConfirm.Format("Are you sure you want to delete user parameter %d for branch %d? This action cannot be undone.", userparaid, branchid);
	if (MsgBoxYesNo(strConfirm) != IDYES)
	{
		return;
	}

	// now delete the relation
	long sqlStatus;
	CString sqlQuery;
	CString lastError;

	sqlQuery.Format("DELETE FROM ppcorappbruser WHERE applicationid=%d AND userid=%d "
		"AND branchregionid=%d AND userparaid=%d", applicationid, userid, branchid, userparaid);
	sqlStatus = SqlExecuteImm(sqlQuery);
	if (sqlStatus != SQL_SUCCESS)
	{
		lastError = DynErrmld();
		// write log
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, ": delete user parameter %d for branch %d from user %d for application %d by operator '%s' failed, %s", userparaid, branchid, userid, applicationid, scmod::ppBLoginData::ex().GetUsername().c_str(), lastError.GetBuffer());
		// show error
		CString strErr;
		strErr.Format("Deleting user parameter %d for branch %d from user %d for application %d failed. %s", userparaid, branchid, userid, applicationid, lastError.GetBuffer());
		MsgBoxOK(strErr);
		return;
	}
	// write log
	SLog::ex().write(__FUNCTION__, VAccess::RET_OK, ": delete user parameter %d for branch %d from user %d for application %d by operator '%s'", userparaid, branchid, userid, applicationid, scmod::ppBLoginData::ex().GetUsername().c_str());

	MsgBoxOK("User parameter deleted successfully.");
	LoadUserParams(userid, applicationid);
}

void PDlgUserManager::OnBtnUserManagerExit()
{
	CDialogMultiLang::OnCancel();
}
