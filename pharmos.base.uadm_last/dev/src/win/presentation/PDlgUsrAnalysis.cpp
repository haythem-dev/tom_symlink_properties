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
#include <BppAppMod.h>
#include <BConppAppMod.h>
#include <BppAppRight.h>
#include <BConppAppRight.h>
#include <BUserAnalysis.h>
#include <BConUserAnalysis.h>
#include "PDlgAppListCtrl.h"
#include "PDlgModListCtrl.h"
#include "PDlgRightListCtrl.h"
#include "PDlgUsrAnalysisListCtrl.h"
#include "presentation.h"
#include "PDlgUsrAnalysis.h"

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
IMPLEMENT_DYNCREATE(PDlgUsrAnalysis, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgUsrAnalysis::PDlgUsrAnalysis()
	: CDialogMultiLang(PDlgUsrAnalysis::IDD)
{
	m_pli_User = new PDlgUsrAnalysisListCtrl;
	   assert(m_pli_User);

	m_pli_Ri = new PDlgRightListCtrl;
	   assert(m_pli_Ri);

	m_pli_App = new PDlgAppListCtrl;
	   assert(m_pli_App);

	m_pli_Mod = new PDlgModListCtrl;
	   assert(m_pli_Mod);

	m_sAppid = 0;
	m_sModid = 0;
	m_sBranchRegid = 0;
	m_strAppname = "";
	m_strModname = "";
	m_strRightname = "";
}

/**---------------------------------------------------------------------------*/
PDlgUsrAnalysis::~PDlgUsrAnalysis()
{
	if (m_pli_Ri != NULL)
	{
		delete m_pli_Ri;
		m_pli_Ri = NULL;
	}

	if (m_pli_App != NULL)
	{
		delete m_pli_App;
		m_pli_App = NULL;
	}

	if (m_pli_Mod != NULL)
	{
		delete m_pli_Mod;
		m_pli_Mod = NULL;
	}
	
	if (m_pli_User != NULL)
	{
		delete m_pli_User;
		m_pli_User = NULL;
	}	
}

/**---------------------------------------------------------------------------*/
void PDlgUsrAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_USRANALY_APP,		*m_pli_App);
	DDX_Control(pDX, IDC_LI_USRANALY_MOD,		*m_pli_Mod);
	DDX_Control(pDX, IDC_LI_USRANALY_RIGHT,		*m_pli_Ri);
	DDX_Control(pDX, IDC_LI_USRANALY_USER,		*m_pli_User);
	DDX_Control(pDX, IDC_BT_USRANALY_SEL_APP,	m_bt_Sel_App);
	DDX_Control(pDX, IDC_BT_USRANALY_SEL_MOD,	m_bt_Sel_Mod);
	DDX_Control(pDX, IDC_BT_USRANALY_SEL_RI,	m_bt_Sel_Right);
	DDX_Control(pDX, IDC_BT_USRANALY_EXIT,		m_bt_Exit);	
	DDX_Control(pDX, IDC_BT_USRANALY_PRINT,		m_bt_Print);	
}

/**---------------------------------------------------------------------------*/
BOOL PDlgUsrAnalysis::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	m_pli_App->init();
	if (m_pli_App->load() == FALSE) 
		m_bt_Sel_App.EnableWindow(false);
	
	m_pli_User->init();
	m_pli_Mod->init();
	m_pli_Ri->init();
	
	m_pli_App->SetFocus();

	//Controls
	SetCtrlState(-1);
		
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

//---------------------------------------------------------------------------
void PDlgUsrAnalysis::OnBtSelApp()
{
	int iItem = -1;

	m_sAppid = (short) m_pli_App->getNextSelectedBsnObj(&iItem)->getAppid();
	m_strAppname = m_pli_App->getBsnObj(m_pli_App->getIdx(iItem))->getAppname();
	
	m_pli_Mod->giveCon().setAppid(m_sAppid);
	
	if (m_pli_Mod->load() == FALSE) 
		m_bt_Sel_Mod.EnableWindow(false);
	else
	{
		m_pli_Mod->EnableWindow(true);
		m_pli_Mod->SetFocus();
	}
}

//---------------------------------------------------------------------------
void PDlgUsrAnalysis::OnBtSelMod()
{
	int iItem = -1;
	m_sModid = (short)m_pli_Mod->getNextSelectedBsnObj(&iItem)->getModuleid();
	m_strModname = m_pli_Mod->getBsnObj(m_pli_Mod->getIdx(iItem))->getModname();
	
	m_pli_Ri->giveCon().setAppid(m_sAppid);
	m_pli_Ri->giveCon().setModid(m_sModid);
	m_pli_Ri->giveCon().setBCorGroupRights(false);
	m_pli_Ri->giveCon().setBCorRights(false);
	m_pli_Ri->giveCon().setBModRights(true);

	if (m_pli_Ri->load() == FALSE) 
		m_bt_Sel_Right.EnableWindow(false);
	else
	{
		m_pli_Ri->EnableWindow(true);
		m_pli_Ri->SetFocus();
	}
}

//---------------------------------------------------------------------------
void PDlgUsrAnalysis::OnBtSelRight()
{
	int iItem = -1;
	m_strRightname = m_pli_Ri->getNextSelectedBsnObj(&iItem)->getRightname();
	
	m_pli_User->giveCon().setAppid(m_sAppid);
	m_pli_User->giveCon().setModid(m_sModid);
	m_pli_User->giveCon().setRightname(m_strRightname);

	if (m_pli_User->load() == TRUE)
	{
		SetCtrlState(1);
		m_pli_User->EnableWindow(true);
		m_pli_User->setColumnColor(4, RGB(0,0,0), RGB(255,255,0));
		m_pli_User->SetFocus();
	}
}

//---------------------------------------------------------------------------
void PDlgUsrAnalysis::OnBtPrint()
{
	if (MsgBoxYesNo(IDS_PRINT_MSG) == IDYES)
	{
		BeginWaitCursor();

		CPrint * pPrint = new CPrint; 
		assert(pPrint);

		EndWaitCursor();

		CString csTitle;
		csTitle.Format(CResString::ex().getStrTblText(IDS_PRT_USR_ANALY_TITLE),
						m_strAppname.c_str(), m_strModname.c_str(), m_strRightname.c_str());

		if (pPrint->init(GetFont(), 
						csTitle,
						CPrint::LANDSCAPE,
						true) == false)
		{
			delete pPrint;
			return;
		}

		BeginWaitCursor();

		PrintHead(pPrint);									// Kopf

		pPrint->setFooterPageLabel (CResString::ex().getStrTblText(IDS_PRINT_PAGE));
		pPrint->setFooterDateLabel (CResString::ex().getStrTblText(IDS_PRINT_DATE));
		pPrint->storeFooterStandard(0, VS_PRINTLINE_WIDTH);		// Fuß mit Seite und Datum

		PrintRows(pPrint);									// Daten

		delete pPrint;
		EndWaitCursor();
	}
}

//---------------------------------------------------------------------------
void PDlgUsrAnalysis::PrintHead        (CPrint * pPrint)
{
	CString csTitle;
	csTitle.Format(CResString::ex().getStrTblText(IDS_PRT_USR_ANALY_TITLE),
						m_strAppname.c_str(), m_strModname.c_str(), m_strRightname.c_str());

	pPrint->storeHeaderTextLine(csTitle, 0, VS_PRINTLINE_WIDTH, FW_BOLD  , TA_LEFT, true);
	pPrint->storeHeaderTextLine(" "    , 0, VS_PRINTLINE_WIDTH, FW_NORMAL, TA_LEFT, true);

	// Spaltentrennung
	pPrint->storeHeaderVerticLine(VS_MODNAME   - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_USERNAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_SURNAME   - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_FORENAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_AREANAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_RIGHTNAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_RIGHTDESC - VS_PRINTCOL_EINZUG/2);


	// Spaltentext
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_APP), VS_APPNAME + 25 ,0,FW_BOLD  );
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_MOD  ), VS_MODNAME + 25, 0, FW_BOLD, TA_LEFT );
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_USERNAME), VS_USERNAME + 25, 0, FW_BOLD, TA_LEFT);
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_SURNAME  ), VS_SURNAME + 25, 0, FW_BOLD, TA_LEFT);
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_FORENAME), VS_FORENAME + 15, 0, FW_BOLD, TA_LEFT);
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_VIEW_PREV_LI_AREA  ), VS_AREANAME + 25, 0, FW_BOLD, TA_LEFT );
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_RIGHT  ), VS_RIGHTNAME + 25, 0, FW_BOLD, TA_LEFT);
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_DESC  ), VS_RIGHTDESC + 25, 0, FW_BOLD, TA_LEFT, true);

	pPrint->storeHeaderVerticLine(VS_MODNAME   - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_USERNAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_SURNAME   - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_FORENAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_AREANAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_RIGHTNAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_RIGHTDESC - VS_PRINTCOL_EINZUG/2);
	
	pPrint->storeHeaderHorizLine(0, VS_PRINTLINE_WIDTH, true);
}

//---------------------------------------------------------------------------
void PDlgUsrAnalysis::PrintRows        (CPrint * pPrint)
{
	BConUserAnalysis* pCon			= &m_pli_User->giveCon();
	BUserAnalysis   * pUsrAnaly		= NULL;
	unsigned int     uiIdx			= 0;
	CBString           csTxt;
	unsigned int	   length;


	while ((pUsrAnaly = pCon->getBsnObj(uiIdx++)) != 0)
	{
		csTxt = pUsrAnaly->getAppname();
		length = 15;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_APPNAME);

		csTxt = pUsrAnaly->getModname();
		length = 20;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_MODNAME + VS_PRINTCOL_EINZUG);
		
		csTxt = pUsrAnaly->getUsername();
		length = 15;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_USERNAME + VS_PRINTCOL_EINZUG);
		
		csTxt = pUsrAnaly->getSurname();
		length = 15;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_SURNAME + VS_PRINTCOL_EINZUG);
		
		csTxt = pUsrAnaly->getForename();
		length = 12;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_FORENAME + VS_PRINTCOL_EINZUG);
		
		csTxt = pUsrAnaly->getAreaname();
		length = 15;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_AREANAME + VS_PRINTCOL_EINZUG);
		
		csTxt = m_strRightname;
		length = 20;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_RIGHTNAME + VS_PRINTCOL_EINZUG);
		
		csTxt = pUsrAnaly->getRightDesc();
		length = 40;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_RIGHTDESC + VS_PRINTCOL_EINZUG);

		// Spaltentrennung
		pPrint->printVerticLine(VS_MODNAME   - VS_PRINTCOL_EINZUG/2);
		pPrint->printVerticLine(VS_USERNAME - VS_PRINTCOL_EINZUG/2);
		pPrint->printVerticLine(VS_SURNAME   - VS_PRINTCOL_EINZUG/2);
		pPrint->printVerticLine(VS_FORENAME - VS_PRINTCOL_EINZUG/2);
		pPrint->printVerticLine(VS_AREANAME - VS_PRINTCOL_EINZUG/2);
		pPrint->printVerticLine(VS_RIGHTNAME - VS_PRINTCOL_EINZUG/2);
		pPrint->printVerticLine(VS_RIGHTDESC - VS_PRINTCOL_EINZUG/2);

		pPrint->linefeed();
	}
}

//---------------------------------------------------------------------------
void PDlgUsrAnalysis::OnBtExit()
{
	CDialogMultiLang::OnCancel();
}

//---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(PDlgUsrAnalysis, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_USRANALY_APP, OnItemchangedAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_USRANALY_APP, OnDblclkAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_USRANALY_MOD, OnDblclkModListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_USRANALY_MOD, OnItemchangedModListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_USRANALY_RIGHT, OnDblclkRightListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_USRANALY_RIGHT, OnItemchangedRightListCtrl)

	ON_BN_CLICKED(IDC_BT_USRANALY_SEL_APP, OnBtSelApp)
	ON_BN_CLICKED(IDC_BT_USRANALY_SEL_MOD, OnBtSelMod)
	ON_BN_CLICKED(IDC_BT_USRANALY_SEL_RI, OnBtSelRight)
	ON_BN_CLICKED(IDC_BT_USRANALY_PRINT, OnBtPrint)
	ON_BN_CLICKED(IDC_BT_USRANALY_EXIT, OnBtExit)
END_MESSAGE_MAP()


//---------------------------------------------------------------------------
void PDlgUsrAnalysis::SetCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;

	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_User->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;
	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Print.EnableWindow(bAnzeige);
}

//---------------------------------------------------------------------------
void PDlgUsrAnalysis::OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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

//---------------------------------------------------------------------------
void PDlgUsrAnalysis::OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
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
	
	m_pli_Mod->EnableWindow(false);
	m_pli_Mod->clearAllBsnObj();

	SetCtrlState (-1);
	
	m_pli_Ri->EnableWindow(false);
	m_pli_Ri->clearAllBsnObj();

	m_pli_User->EnableWindow(false);
	m_pli_User->clearAllBsnObj();

	*pResult = 0;
}

//---------------------------------------------------------------------------
void PDlgUsrAnalysis::OnDblclkModListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Sel_Mod.IsWindowEnabled())
			OnBtSelMod();
	}

	*pResult = 0;
}

//---------------------------------------------------------------------------
void PDlgUsrAnalysis::OnItemchangedModListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
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

	SetCtrlState (-1);
	m_pli_Ri->EnableWindow(false);
	m_pli_Ri->clearAllBsnObj();

	m_pli_User->EnableWindow(false);
	m_pli_User->clearAllBsnObj();

	*pResult = 0;
}

//---------------------------------------------------------------------------
void PDlgUsrAnalysis::OnDblclkRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Sel_Right.IsWindowEnabled())
			OnBtSelRight();
	}

	*pResult = 0;
}

//---------------------------------------------------------------------------
void PDlgUsrAnalysis::OnItemchangedRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
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

	SetCtrlState (-1);
	m_pli_User->EnableWindow(false);
	m_pli_User->clearAllBsnObj();

	*pResult = 0;
}

//--------------------------------------------------------------------------- 
BOOL PDlgUsrAnalysis::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			
			if (GetFocus() == GetDlgItem(IDC_LI_USRANALY_APP))
			{
				if (m_bt_Sel_App.IsWindowEnabled())
				{
					OnBtSelApp();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LI_USRANALY_MOD))
			{
				if (m_bt_Sel_Mod.IsWindowEnabled())
				{
					OnBtSelMod();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LI_USRANALY_RIGHT))
			{
				if (m_bt_Sel_Right.IsWindowEnabled())
				{
					OnBtSelRight();
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

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}
