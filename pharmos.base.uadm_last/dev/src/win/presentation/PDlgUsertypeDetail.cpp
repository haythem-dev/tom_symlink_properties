/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"
#include "presentation.h"
#include <BppUsertype.h>
#include <tbl_ppusertype.h>
#include <BConppUsertype.h>
#include <BTest.h>
#include "PDlgUsertypeDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**---------------------------------------------------------------------------*/
// 
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(PDlgUsertypeDetail, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgUsertypeDetail::PDlgUsertypeDetail(BppUsertype* pBppUsertype)
				: CDialogMultiLang(PDlgUsertypeDetail::IDD ),
				m_pBppUsertypeClone (pBppUsertype->clone())
{
	m_pTest = new BTest;
		assert (m_pTest);

	pUsertypeTestChange = pBppUsertype->clone();
	
	assert(m_pBppUsertypeClone);
	
	m_bUsertypeNew = false;
	m_bDataChanged = false;
	m_strUsertypenamePrev = "";
	m_strUsertypedescPrev = "";
}

/**---------------------------------------------------------------------------*/
PDlgUsertypeDetail::~PDlgUsertypeDetail()
{
	if (m_pBppUsertypeClone != NULL)
	{
		delete m_pBppUsertypeClone;
		m_pBppUsertypeClone = NULL;
	} 

	if (pUsertypeTestChange != NULL)
	{
		delete pUsertypeTestChange;
		pUsertypeTestChange = NULL;
	}
	
	if (m_pTest != NULL)
	{
		delete m_pTest;
		m_pTest = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgUsertypeDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ED_USERTYPE_NAME, m_ed_Usertypename);
	DDX_Control(pDX, IDC_ED_USERTYPE_DESC, m_ed_Usertypedesc);
	DDX_Control(pDX, IDC_BT_USERTYPE_SAVE, m_bt_Usertype_Save);
	DDX_Control(pDX, IDC_BT_USERTYPE_DTL_EXIT, m_bt_Usertype_Exit);
	DDX_Control(pDX, IDC_ED_USERTYPE_ID, m_ed_Usertypeid);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgUsertypeDetail::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();
	
	if(m_bUsertypeNew == false)
	{
		m_ed_Usertypeid.SetWindowText(m_pBppUsertypeClone->getUsertypeidStr().c_str());
		m_ed_Usertypename.SetWindowText(m_pBppUsertypeClone->getName().c_str());
		m_ed_Usertypedesc.SetWindowText(m_pBppUsertypeClone->getDescription().c_str());
		
		m_strUsertypenamePrev = m_pBppUsertypeClone->getName();
		m_strUsertypedescPrev = m_pBppUsertypeClone->getDescription();
	}

	m_ed_Usertypename.SetFocus();

	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
void PDlgUsertypeDetail::setBNewUsertype(bool bNewUsertype)
{
	m_bUsertypeNew = bNewUsertype;
}

/**---------------------------------------------------------------------------*/
const bool PDlgUsertypeDetail::getBNewUsertype()
{
	return m_bUsertypeNew;
}

/**---------------------------------------------------------------------------*/
const bool	PDlgUsertypeDetail::getBDataChanged()
{
	return m_bDataChanged;
}


/**---------------------------------------------------------------------------*/
bool PDlgUsertypeDetail::GetCtrlValues()
{
	//Daten von GUI
	CString csUsertypeName;
	m_ed_Usertypename.GetWindowText(csUsertypeName);
	CString csUsertypeDesc;
	m_ed_Usertypedesc.GetWindowText(csUsertypeDesc);

	//Längenprüfung 
	CBString strUsertypeName = (LPCTSTR)csUsertypeName;
	strUsertypeName.erasespace(CBString::END);
	int iLengthN = (int)strUsertypeName.length();
	CBString strUsertypeDesc = (LPCTSTR)csUsertypeDesc;
	strUsertypeDesc.erasespace(CBString::END);
	int iLengthD = (int)strUsertypeDesc.length();
	bool bUsertypenameExists;

	if ( iLengthN > L_PPUSERTYPE_NAME || iLengthN <= 0)
	{
		MsgBoxOK(IDS_ERR_USERTYPENAME, MB_ICONERROR); //Kennungen nicht länger als 150 Zeichen oder leer
		m_ed_Usertypename.SetFocus();
		return false;
	}
	if (iLengthD > L_PPUSERTYPE_DESC)
	{
		MsgBoxOK(IDS_ERR_USERTYPEDESC, MB_ICONERROR); //Kennungen nicht länger als 254 Zeichen
		m_ed_Usertypedesc.SetFocus();
		return false;
	}

	else if (!(UsertypeNameExists(strUsertypeName, bUsertypenameExists)))
	{
		MsgBoxOK(IDS_ERR_SQL_COUNT_USERTYPENAME); //SQL-Abfrage auf Usertypename FAULT!
		return false;
	}
	else if (bUsertypenameExists == true)
	{
		MsgBoxOK(IDS_ERR_USERTYPENAME_EXISTS, MB_ICONERROR); //Usertypename schon vorhanden!
		m_ed_Usertypename.SetFocus();
		return false;
	}	
	else
	{
		//BsnObj füllen
		m_pBppUsertypeClone->setName(strUsertypeName);
		m_pBppUsertypeClone->setDescription(strUsertypeDesc);
	}
	return true;
}

/**---------------------------------------------------------------------------*/
bool PDlgUsertypeDetail::UsertypeNameExists(CBString &strUsertypeName, bool & bExists)
{
	if( m_bUsertypeNew == false )
	{
		if ( m_strUsertypenamePrev == strUsertypeName )
		{
			bExists = false;
			return true;
		}
	}
	
	m_pTest->setUsertypeName(strUsertypeName);
	
	if(!(m_pTest->SelUsertypeNameExists(bExists)))
	{
		return false;
	}
	return true;
}

/**---------------------------------------------------------------------------*/
void PDlgUsertypeDetail::getCloneObj(BppUsertype* pUsertypeChanged)
{
	*pUsertypeChanged = *m_pBppUsertypeClone;
}


/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgUsertypeDetail, CDialogMultiLang)
	ON_BN_CLICKED(IDC_BT_USERTYPE_SAVE, OnBtUsertypeSave)
	ON_BN_CLICKED(IDC_BT_USERTYPE_DTL_EXIT, OnBtUsertypeExit)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
void PDlgUsertypeDetail::OnBtUsertypeSave()
{
	if(GetCtrlValues()) //Daten von GUI eingelesen!
	{
		if(*pUsertypeTestChange == *m_pBppUsertypeClone) //keine Änderung!
		{
			m_bDataChanged = false;
		}
		else
		{
			if(m_bUsertypeNew == false) // UPDATE
			{
				BppUsertype* pUsertypeUpdTest = new BppUsertype;
				pUsertypeUpdTest->setUsertypeid(m_pBppUsertypeClone->getUsertypeid());
				IPerst::ERet Ret = pUsertypeUpdTest->SelForUpd();
				if (Ret != IPerst::RET_OK)
				{
					//kein Update --> Datensatz zum Updaten nicht gefunden!
					MsgBoxOK(IDS_NO_UPDATE_POSS);
				}
				else if (*pUsertypeUpdTest != *pUsertypeTestChange)
				{
					//kein Update --> Datensatz inzwischen extern verändert!
					MsgBoxOK(IDS_UPDATED_EXTERN);
				}
				else
				{
					IPerst::ERet eRet = m_pBppUsertypeClone->updateItem();
					if (eRet == IPerst::RET_OK)
					{
						m_bDataChanged = true;
					}
					else
					{
						MsgBoxOK(IDS_ERR_UPDATE);
					}
				}
				delete pUsertypeUpdTest;
			}
			else //INSERT
			{
				IPerst::ERet eRet = m_pBppUsertypeClone->insertItem();
				if (eRet == IPerst::RET_OK)
				{
					eRet = m_pBppUsertypeClone->selectID();
					if (eRet != IPerst::RET_OK) //Fehler beim Insert
					{
						MsgBoxOK(IDS_ERR_INSERT);
					}
					m_bDataChanged = true;
				}
				else
				{
					MsgBoxOK(IDS_ERR_INSERT);
				}
			}
		}
		CDialogMultiLang::OnOK();
	}
}

/**---------------------------------------------------------------------------*/
void PDlgUsertypeDetail::OnBtUsertypeExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgUsertypeDetail::PreTranslateMessage(MSG  * pMsg )
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// Fokus steht in UsrListCtrl
			if (GetFocus() == GetDlgItem(IDC_BT_USERTYPE_SAVE))
			{
				if (m_bt_Usertype_Save.IsWindowEnabled())
				{
					OnBtUsertypeSave();
					return TRUE;
				}
			}
			// Fokus steht in AppListCtrl
			if (GetFocus() == GetDlgItem(IDC_BT_USERTYPE_DTL_EXIT))
			{
				if (m_bt_Usertype_Exit.IsWindowEnabled())
				{
					OnBtUsertypeExit();
					return TRUE;
				}
			}
			return FALSE;
			
		case VK_ESCAPE:
			if (m_bt_Usertype_Exit.IsWindowEnabled())
			{
				OnBtUsertypeExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}


