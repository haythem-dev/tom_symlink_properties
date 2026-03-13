/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef _PDLGUSERTYPEDETAIL_H_____
#define _PDLGUSERTYPEDETAIL_H_____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BppUsertype;
class BTest;

/*----------------------------------------------------------------------------*/
// 
/*----------------------------------------------------------------------------*/
class PDlgUsertypeDetail : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgUsertypeDetail)

public:
	PDlgUsertypeDetail(BppUsertype* pBppUserType);
	virtual ~PDlgUsertypeDetail();

	void		setBNewUsertype(bool bNewGrp);
	const bool	getBNewUsertype();
	const bool	getBDataChanged();
	void		getCloneObj(BppUsertype * pUsertypeChanged);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	OnInitDialog       ();
	virtual BOOL	PreTranslateMessage(MSG  * pMsg );

	bool GetCtrlValues();
	bool UsertypeNameExists(CBString &strUsertypeName, bool & bExists);

private:
	enum { IDD = IDD_DLG_USERTYPES_DTL };
	
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnBtUsertypeSave();
	afx_msg void OnBtUsertypeExit();
	
	DECLARE_MESSAGE_MAP()


	BppUsertype*  m_pBppUsertypeClone;
	BppUsertype*  pUsertypeTestChange;
	bool		  m_bUsertypeNew;
	bool		  m_bDataChanged;
	BTest*   	  m_pTest;
	CBString      m_strUsertypenamePrev;
	CBString      m_strUsertypedescPrev;


	// #### Controls
	CEdit m_ed_Usertypeid;
	CEdit m_ed_Usertypename;
	CEdit m_ed_Usertypedesc;
	CButton m_bt_Usertype_Save;
	CButton m_bt_Usertype_Exit;
};

#endif

