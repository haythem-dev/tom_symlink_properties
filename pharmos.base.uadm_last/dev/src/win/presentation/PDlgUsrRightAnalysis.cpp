/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include <BppApp.h>
#include <BConppApp.h>
#include <BppUser.h>
#include <BConppUser.h>
#include <BppAppMod.h>
#include <BConppAppMod.h>
#include <BppAppRight.h>
#include <BConppAppRight.h>
#include <BUserAnalysis.h>
#include <BConUserAnalysis.h>
#include <BUserGroupAnalysis.h>
#include <BConUserGroupAnalysis.h>
#include <BUserRightAnalysis.h>
#include <BConUserRightAnalysis.h>
#include <BppBranchReg.h>
#include <BConppBranchReg.h>

#include "PDlgUserListCtrl.h"
#include "PDlgAppListCtrl.h"
#include "PDlgRightListCtrl.h"
#include "PDlgUsrGroupAnalysisListCtrl.h"
#include "PDlgUsrRightAnalysisListCtrl.h"
#include "presentation.h"
#include "PDlgUsrRightAnalysis.h"
#include "PDlgBranchListCtrl.h"
#include "ExportCsv.h"

#include <cctype>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*----------------------------------------------------------------------------*/
// Defines
/*----------------------------------------------------------------------------*/
#define VS_APPNAME 				 0
#define VS_MODNAME	 			(VS_APPNAME    + 250)
#define VS_USERNAME			    (VS_MODNAME    + 350)
#define VS_SURNAME	 			(VS_USERNAME   + 270)
#define VS_FORENAME				(VS_SURNAME    + 280)
#define VS_AREANAME             (VS_FORENAME   + 250)
#define VS_RIGHTNAME            (VS_AREANAME   + 300)
#define VS_RIGHTDESC            (VS_RIGHTNAME  + 420)
#define VS_COLENDE              (VS_RIGHTDESC  + 480)

#define VS_PRINTCOL_EINZUG           10

#define VS_PRINTLINE_WIDTH         2600

#if (VS_COLENDE > VS_PRINTLINE_WIDTH)
#error VS_COLENDE > VS_PRINTLINE_WIDTH
#endif


/*----------------------------------------------------------------------------*/
// 
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(PDlgUsrRightAnalysis, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgUsrRightAnalysis::PDlgUsrRightAnalysis()
	: CDialogMultiLang(PDlgUsrRightAnalysis::IDD)
    , m_pli_User(NULL)
    , m_pli_App(NULL)
    , m_pli_UsrGrpAnalysis(NULL)
    , m_SelOption(0)
    , m_Analyse_By_Option(0)
{
       m_pli_User = new PDlgUserListCtrl;
	   assert(m_pli_User);

       m_pli_App = new PDlgAppListCtrl;
       assert(m_pli_App);

       m_pli_UsrGrpAnalysis = new PDlgUsrGroupAnalysisListCtrl;
	   assert(m_pli_UsrGrpAnalysis);

       m_pli_UsrRightAnalysis = new PDlgUsrRightAnalysisListCtrl;
       assert(m_pli_UsrRightAnalysis);

       m_pConBranch = new BConppBranchReg;

	m_sAppid = 0;
    m_Userid = 0;
	m_strAppname = "";
    //m_bRegDlg = false;

}

/**---------------------------------------------------------------------------*/
PDlgUsrRightAnalysis::~PDlgUsrRightAnalysis()
{

	if (m_pli_App != NULL)
	{
		delete m_pli_App;
        m_pli_App = NULL;
	}
	if (m_pli_User != NULL)
	{
		delete m_pli_User;
		m_pli_User = NULL;
	}	
    if (m_pli_UsrGrpAnalysis != NULL)
    {
        delete m_pli_UsrGrpAnalysis;
        m_pli_UsrGrpAnalysis = NULL;
    }
    if (m_pli_UsrRightAnalysis != NULL)
    {
        delete m_pli_UsrRightAnalysis;
        m_pli_UsrRightAnalysis = NULL;
    }

    delete m_pConBranch;
}

/**---------------------------------------------------------------------------*/
void PDlgUsrRightAnalysis::DoDataExchange(CDataExchange* pDX)
{
    CDialogMultiLang::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_LI_USR_RIGHT_ANALY_USER, *m_pli_User);
    DDX_Control(pDX, IDC_LI_USR_RIGHT_ANALY_APP, *m_pli_App);
    DDX_Control(pDX, IDC_LI_USR_RIGHT_ANALY_USER_GROUP, *m_pli_UsrGrpAnalysis);
    DDX_Control(pDX, IDC_LI_USR_RIGHT_ANALY_USER_RIGHT, *m_pli_UsrRightAnalysis);
    DDX_Control(pDX, IDC_BT_USR_RIGHT_ANALY_SEL_APP, m_bt_Sel_App);
    DDX_Control(pDX, IDC_BT_USR_RIGHT_ANALY_SEL_USER, m_bt_Sel_User);
    DDX_Control(pDX, IDC_BT_USR_RIGHT_ANALY_PRINT, m_bt_Print);
    DDX_Control(pDX, IDC_BT_USR_RIGHT_ANALY_EXIT, m_bt_Exit);
    DDX_Control(pDX, IDC_CB_UR_BR,	        m_cb_BrReg);
    DDX_Radio(pDX, IDC_RAD_USER_GROUP, m_SelOption);
    DDX_Radio(pDX, IDC_RAD_ANALY_BY_APP, m_Analyse_By_Option);
    
}

/**---------------------------------------------------------------------------*/
BOOL PDlgUsrRightAnalysis::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

   
    m_SelOption = FALSE;
    m_Analyse_By_Option = 0;
    UpdateData(false);

    m_pli_App->init();
    m_pli_User->init();
    m_pli_App->EnableWindow(false);
    m_pli_User->EnableWindow(false);

    m_pli_UsrRightAnalysis->init();
    m_pli_UsrGrpAnalysis->init();

    loadBranch();
    OnRadUserGroupClicked();
    m_pli_App->SetFocus();
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

//---------------------------------------------------------------------------
void PDlgUsrRightAnalysis::OnBtSelApp()
{
    UpdateData(true);
    int iItem = -1;
    //m_cb_BrReg.ResetContent(); //srk
    m_sAppid = (short) m_pli_App->getNextSelectedBsnObj(&iItem)->getAppid();
    if (m_SelOption == 0)
    {
        m_pli_UsrGrpAnalysis->giveCon().setAppid(m_sAppid);
        m_pli_UsrGrpAnalysis->giveCon().setUserid(0);
        if (m_pli_UsrGrpAnalysis->load() == TRUE)
        {
            SetCtrlState(1);
            m_pli_UsrGrpAnalysis->EnableWindow(true);
            m_pli_UsrGrpAnalysis->SetFocus();
        }
    }
    else
    {

        GetBrRegComboValue();
        m_pli_UsrRightAnalysis->giveCon().setAppid(m_sAppid);
        m_pli_UsrRightAnalysis->giveCon().setUserid(0);
        m_pli_UsrRightAnalysis->giveCon().setBranchRegid(m_sBrRegid);
        if (m_pli_UsrRightAnalysis->load() == TRUE)
        {
            SetCtrlState(1);
            m_pli_UsrRightAnalysis->EnableWindow(true);
            m_pli_UsrRightAnalysis->SetFocus();
        }
    }
}

void PDlgUsrRightAnalysis::loadBranch()
{
        m_pConBranch->setAppid(-1);
        m_pConBranch->setBSelBranch(true);
        m_pConBranch->setBSelReg(false);
        m_pConBranch->setBSelCor(false);
        
        m_pConBranch->load();
        BppBranchReg* pBranch = 0;
        unsigned int uiCnt  = 0;
        while ((pBranch = m_pConBranch->getBsnObj(uiCnt++)) != 0)
        {
            m_cb_BrReg.AddString(pBranch->getBranchname().c_str());
        }
        m_cb_BrReg.AddString(CResString::ex().getStrTblText(IDS_ALL_APP));
        m_cb_BrReg.SetCurSel(0);
        m_sBrRegid = 0;
        m_cb_BrReg.SetFocus();
}

void PDlgUsrRightAnalysis::OnBtSelUser()
{
    UpdateData(true);
    int iItem = -1;

    m_Userid  = (long)m_pli_User->getNextSelectedBsnObj(&iItem)->getUserid();
    
    if (m_SelOption == 0)
    {
        m_pli_UsrGrpAnalysis->giveCon().setAppid(0);
        m_pli_UsrGrpAnalysis->giveCon().setUserid(m_Userid);
        if (m_pli_UsrGrpAnalysis->load() == TRUE)
        {
            SetCtrlState(1);
            m_pli_UsrGrpAnalysis->EnableWindow(true);
            m_pli_UsrGrpAnalysis->SetFocus();
        }
    }
    else
    {
        m_pli_UsrRightAnalysis->giveCon().setAppid(0);
        m_pli_UsrRightAnalysis->giveCon().setUserid(m_Userid);
        m_pli_UsrRightAnalysis->giveCon().setBranchRegid(m_sBrRegid);
        GetBrRegComboValue();

        if (m_pli_UsrRightAnalysis->load() == TRUE)
        {
            SetCtrlState(1);
            m_pli_UsrRightAnalysis->EnableWindow(true);
            m_pli_UsrRightAnalysis->SetFocus();
        }
    }
}

std::string PDlgUsrRightAnalysis::OnFileOpen()
{
    TCHAR szFilters[] = _T("csv Files (*.csv)|*.csv|");

    CFileDialog fileDlg(FALSE, _T("Csv"), _T("*.csv"),
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters);

    if (fileDlg.DoModal() == IDOK)
    {
        CString pathName = fileDlg.GetPathName();
        CT2CA pszConvertedAnsiString(pathName);
        return std::string(pszConvertedAnsiString);
    }
    return "";
}

void PDlgUsrRightAnalysis::OnBtPrint()
{
    
    std::string pathName= OnFileOpen();
    if (pathName.empty())
        return ;
    CsvFile csv(pathName); // throws exceptions!

    if (m_SelOption == 1)
    {
        // Hearer
        csv << CResString::ex().getStrTblText(IDS_APP);
        csv << CResString::ex().getStrTblText(IDS_BRANCH_NO);
        csv << CResString::ex().getStrTblText(IDS_VIEW_PREV_LI_AREA);
        csv << CResString::ex().getStrTblText(IDS_USERNAME);
        csv << CResString::ex().getStrTblText(IDS_SURNAME);
        csv << CResString::ex().getStrTblText(IDS_FORENAME);
        csv << CResString::ex().getStrTblText(IDS_MOD);
        csv << CResString::ex().getStrTblText(IDS_RIGHT);
        csv << CResString::ex().getStrTblText(IDS_DESC);
        csv << endrow;
        WriteUserRightRows(csv);
    }
    else
    {
        // Hearer
        csv << CResString::ex().getStrTblText(IDS_APP);
        csv << CResString::ex().getStrTblText(IDS_DLG_GROUP_LI_NAME);
        csv << CResString::ex().getStrTblText(IDS_USERNAME);
        csv << CResString::ex().getStrTblText(IDS_SURNAME);
        csv << CResString::ex().getStrTblText(IDS_FORENAME);
        csv << CResString::ex().getStrTblText(IDS_VIEW_USER_LI_USERCOUNTRY);
        csv << CResString::ex().getStrTblText(IDS_VIEW_USER_LI_USERTYPE);
        csv << endrow;
        WriteUserGroupRows(csv);
    }
}


inline std::string trim(const std::string& str)
{
    std::string ret = str;

    while (!ret.empty() && std::isspace(static_cast<unsigned char>(ret.front())))
        ret.erase(0, 1);

    while (!ret.empty() && std::isspace(static_cast<unsigned char>(ret.back())))
        ret.pop_back();

    return ret;
}

void PDlgUsrRightAnalysis::WriteUserRightRows(CsvFile&  csv)
{
    BConUserRightAnalysis* pCon = &m_pli_UsrRightAnalysis->giveCon();
    BUserRightAnalysis   * pRiAnaly = NULL;
    unsigned int	   uiIdx = 0;

    while ((pRiAnaly = pCon->getBsnObj(uiIdx++)) != 0)
    {
        csv << pRiAnaly->getAppname().c_str();
        csv << pRiAnaly->getRegionidStr().c_str();
        csv << pRiAnaly->getAreaname().c_str();
        csv << pRiAnaly->getUsername().c_str();
        csv << pRiAnaly->getSurname().c_str();
        csv << pRiAnaly->getForename().c_str();
        csv << pRiAnaly->getModname().c_str();
        csv << pRiAnaly->getRightname().c_str();
        csv << pRiAnaly->getRightDesc().c_str();
        csv << endrow;
    }
}

void PDlgUsrRightAnalysis::WriteUserGroupRows(CsvFile&  csv)
{
    BConUserGroupAnalysis* pCon = &m_pli_UsrGrpAnalysis->giveCon();
    BUserGroupAnalysis   * pGrpAnaly = NULL;
    unsigned int	   uiIdx = 0;

    while ((pGrpAnaly = pCon->getBsnObj(uiIdx++)) != 0)
    {
        csv << pGrpAnaly->getAppname().c_str();
        csv << pGrpAnaly->getGroupname().c_str();
        csv << pGrpAnaly->getUsername().c_str();
        csv << pGrpAnaly->getSurname().c_str();
        csv << pGrpAnaly->getForename().c_str();
        csv << pGrpAnaly->getUserCountry().c_str();
        csv << trim(std::string(pGrpAnaly->getUserType().c_str()));
        csv << endrow;
    }
}

void PDlgUsrRightAnalysis::OnBtExit()
{
    CDialogMultiLang::OnCancel();
}
//---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(PDlgUsrRightAnalysis, CDialogMultiLang)
    ON_BN_CLICKED(IDC_RAD_USER_GROUP, &PDlgUsrRightAnalysis::OnRadUserGroupClicked)
    ON_BN_CLICKED(IDC_RAD_USER_RIGHT, &PDlgUsrRightAnalysis::OnRadUserRightClicked)
    
    ON_NOTIFY(NM_DBLCLK, IDC_LI_USR_RIGHT_ANALY_APP, &PDlgUsrRightAnalysis::OnDblclkAppListCtrl)
    ON_NOTIFY(NM_DBLCLK, IDC_LI_USR_RIGHT_ANALY_USER, &PDlgUsrRightAnalysis::OnDblclkUserListCtrl)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_USR_RIGHT_ANALY_APP, &PDlgUsrRightAnalysis::OnItemchangedAppListCtrl)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_USR_RIGHT_ANALY_USER, &PDlgUsrRightAnalysis::OnItemchangeduserListCtrl)

    ON_BN_CLICKED(IDC_BT_USR_RIGHT_ANALY_SEL_USER, &PDlgUsrRightAnalysis::OnBtSelUser)
    ON_BN_CLICKED(IDC_BT_USR_RIGHT_ANALY_SEL_APP, &PDlgUsrRightAnalysis::OnBtSelApp)
    ON_BN_CLICKED(IDC_BT_USR_RIGHT_ANALY_PRINT, &PDlgUsrRightAnalysis::OnBtPrint)
    ON_BN_CLICKED(IDC_BT_USR_RIGHT_ANALY_EXIT, &PDlgUsrRightAnalysis::OnBtExit)
    ON_BN_CLICKED(IDC_RAD_ANALY_BY_APP, &PDlgUsrRightAnalysis::OnRadAnalyByApp)
    ON_BN_CLICKED(IDC_RAD_ANALY_BY_USER, &PDlgUsrRightAnalysis::OnRadAnalyByUser)
    ON_CBN_SELCHANGE(IDC_CB_UR_BR, &PDlgUsrRightAnalysis::OnCbnSelchangeCbUrBr)
END_MESSAGE_MAP()

//---------------------------------------------------------------------------
void PDlgUsrRightAnalysis::SetCtrlState(const int iItem)
{
    BOOL    bPrint,
        bAnzeige;

    if (iItem == -1)					// nichts markiert
    {
        if (m_SelOption == 0)
        {
            bPrint = (m_pli_UsrGrpAnalysis->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
        }
        else
        {
            bPrint = (m_pli_UsrRightAnalysis->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
        }
        bAnzeige = FALSE;
    }
    else								// Zeile markiert
    {
        bPrint = TRUE;
        bAnzeige = TRUE;
    }

    // setzen der Controls
    m_bt_Print.EnableWindow(bAnzeige);
}

//--------------------------------------------------------------------------- 
BOOL PDlgUsrRightAnalysis::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

    if (pMsg->message == WM_KEYDOWN)
    {
        switch (pMsg->wParam)
        {
        case VK_RETURN:

            if (GetFocus() == GetDlgItem(IDC_LI_USR_RIGHT_ANALY_APP))
            {
                if (m_bt_Sel_App.IsWindowEnabled())
                {
                    OnBtSelApp();
                    return TRUE;
                }
            }
            if (GetFocus() == GetDlgItem(IDC_LI_USR_RIGHT_ANALY_USER))
            {
                if (m_bt_Sel_User.IsWindowEnabled())
                {
                    OnBtSelUser();
                    return TRUE;
                }
            }
            if (GetFocus() == GetDlgItem(IDC_BT_USRANALY_PRINT))
            {
                if (m_bt_Print.IsWindowEnabled())
                {
                    OnBtPrint();
                    return TRUE;
                }
            }
            if (GetFocus() == GetDlgItem(IDC_BT_USRANALY_EXIT))
            {
                if (m_bt_Exit.IsWindowEnabled())
                {
                    OnBtExit();
                    return TRUE;
                }
            }
            return FALSE;

        case VK_ESCAPE:
            if (m_bt_Exit.IsWindowEnabled())
            {
                OnBtExit();
                return TRUE;
            }
            return FALSE;


        default:
            break;
        }
    }
 
    if (GetFocus() == GetDlgItem(IDC_LI_USR_RIGHT_ANALY_USER_RIGHT))
    {
        if (pMsg->wParam >= CHAR_A && pMsg->wParam <= CHAR_Z)
        {
            CBString cbStr;
            cbStr.format("%c", char(pMsg->wParam));
            m_pli_UsrRightAnalysis->searchSurname(cbStr);
            return TRUE;
        }
    }
    if (GetFocus() == GetDlgItem(IDC_LI_USR_RIGHT_ANALY_USER_GROUP))
    {
        if (pMsg->wParam >= CHAR_A && pMsg->wParam <= CHAR_Z)
        {
            CBString cbStr;
            cbStr.format("%c", char(pMsg->wParam));
            m_pli_UsrGrpAnalysis->searchSurname(cbStr);
            return TRUE;
        }
    }
    if (GetFocus() == GetDlgItem(IDC_LI_USR_RIGHT_ANALY_USER) )
    {
        if (pMsg->wParam >= CHAR_A && pMsg->wParam <= CHAR_Z)
        {
            CBString cbStr;
            cbStr.format("%c", char(pMsg->wParam));
            m_pli_User->searchSurname(cbStr);
            return TRUE;
        }
    }
	return CDialogMultiLang::PreTranslateMessage(pMsg);
}


void PDlgUsrRightAnalysis::OnRadUserGroupClicked()
{
    UpdateData(true);
    GetDlgItem(IDC_LI_USR_RIGHT_ANALY_USER_RIGHT)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_LI_USR_RIGHT_ANALY_USER_GROUP)->ShowWindow(SW_SHOW);
    LoadData();
    SetDlgItemText(IDC_USER_RIGHT_ANALY_RESULT, _T("User Group"));

}


void PDlgUsrRightAnalysis::OnRadUserRightClicked()
{
    UpdateData(true);
    GetDlgItem(IDC_LI_USR_RIGHT_ANALY_USER_RIGHT)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_LI_USR_RIGHT_ANALY_USER_GROUP)->ShowWindow(SW_HIDE);

    SetDlgItemText(IDC_USER_RIGHT_ANALY_RESULT, _T("User Right"));
    LoadData();
}


void PDlgUsrRightAnalysis::OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

    if (pNMIA->iItem != -1)
    {
        TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);

        if (m_bt_Sel_App.IsWindowEnabled())
            OnBtSelApp();
    }

    *pResult = 0;
}


void PDlgUsrRightAnalysis::OnDblclkUserListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

    if (pNMIA->iItem != -1)
    {
        TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);

        if (m_bt_Sel_User.IsWindowEnabled())
            OnBtSelUser();
    }

    *pResult = 0;
}


void PDlgUsrRightAnalysis::OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    static int   iSelect = -1;

    if (((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT)
        && (pNMLV->iItem == iSelect))
    {
        TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
    }

    if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE)
    {
        if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
        {
            iSelect = pNMLV->iItem;
            TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

        }
    }

    SetCtrlState(-1);
    if (m_SelOption == 0)
    {
        m_pli_UsrGrpAnalysis->EnableWindow(false);
        m_pli_UsrGrpAnalysis->clearAllBsnObj();
    }
    else
    {
        m_pli_UsrRightAnalysis->EnableWindow(false);
        m_pli_UsrRightAnalysis->clearAllBsnObj();
    }

    *pResult = 0;
}


void PDlgUsrRightAnalysis::OnItemchangeduserListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    static int   iSelect = -1;

    if (((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT)
        && (pNMLV->iItem == iSelect))
    {
        TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
    }

    if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE)
    {
        if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
        {
            iSelect = pNMLV->iItem;
            TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

        }
    }

    SetCtrlState(-1);
    if (m_SelOption == 0)
    {
        m_pli_UsrGrpAnalysis->EnableWindow(false);
        m_pli_UsrGrpAnalysis->clearAllBsnObj();
    }
    else
    {
        m_pli_UsrRightAnalysis->EnableWindow(false);
        m_pli_UsrRightAnalysis->clearAllBsnObj();
    }

    *pResult = 0;
}

void PDlgUsrRightAnalysis::OnRadAnalyByApp()
{
    UpdateData(true);
    LoadData();
}


void PDlgUsrRightAnalysis::OnRadAnalyByUser()
{
    UpdateData(true);
    LoadData();
}

void PDlgUsrRightAnalysis::LoadData()
{
    if (m_Analyse_By_Option == 0)
    {
        if (m_pli_App->IsWindowEnabled() == false)
        {
            m_pli_User->EnableWindow(false);
            m_pli_User->clearAllBsnObj();

            if (m_pli_App->load() == TRUE )
            {
                m_pli_App->EnableWindow(true);
            }
        }
        m_pli_App->SetFocus();
    }
    else
    {
        if (m_pli_User->IsWindowEnabled() == false)
        {
            m_pli_App->EnableWindow(false);
            m_pli_App->clearAllBsnObj();

            m_pli_User->giveCon().setGroupid(0);
            m_pli_User->giveCon().setCorUsrGrp(false);
            m_pli_User->giveCon().setAppid(0);

            if (m_pli_User->load() == TRUE )
            {
                m_pli_User->EnableWindow(true);
            }
        }
        m_pli_User->SetFocus();
    }

    SetCtrlState(-1);

    m_pli_UsrRightAnalysis->EnableWindow(false);
    m_pli_UsrRightAnalysis->clearAllBsnObj();

    m_pli_UsrGrpAnalysis->EnableWindow(false);
    m_pli_UsrGrpAnalysis->clearAllBsnObj();
}


void PDlgUsrRightAnalysis::OnCbnSelchangeCbUrBr()
{
    // TODO: Add your control notification handler code here
    GetBrRegComboValue();
}

void PDlgUsrRightAnalysis::GetBrRegComboValue()
{
//	m_sBrRegid = -1;
	CString csAreaName;
	CBString cbAreaName, cbAll;
    int sel = m_cb_BrReg.GetCurSel();
    if (sel <= 0)
    {
        m_sBrRegid = 0;
        return;
    }
	m_cb_BrReg.GetLBText(m_cb_BrReg.GetCurSel(),csAreaName);
	cbAreaName = (LPCTSTR)csAreaName;

	cbAll = (LPCTSTR)CResString::ex().getStrTblText(IDS_ALL_APP);

	BppBranchReg	  * pBrReg = 0;
	
	unsigned int        uiCnt  = 0;
	
	if ( cbAll != cbAreaName )	// wenn alle, dann bleibt m_sBrRegid = -1
	{
		while ((pBrReg = m_pConBranch->getBsnObj(uiCnt++)) != 0)
		{
			if ( pBrReg->getBranchname() == cbAreaName )
			{
				m_sBrRegid = pBrReg->getBranchid();
				//m_bRegDlg = false;
				break;
			}
		}
	}
}

