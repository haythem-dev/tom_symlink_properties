/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGUSRANALYSIS_H____
#define __PDLGUSRANALYSIS_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgUsrAnalysisListCtrl;
class PDlgAppListCtrl;
class PDlgModListCtrl;
class PDlgRightListCtrl;
class BUserAnalysis; 

/*----------------------------------------------------------------------------*/
class PDlgUsrAnalysis : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgUsrAnalysis)

					PDlgUsrAnalysis();           
	virtual			~PDlgUsrAnalysis();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG   * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkModListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedModListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBtSelApp();
	afx_msg void OnBtSelMod();
	afx_msg void OnBtPrint();
	afx_msg void OnBtExit();
	afx_msg void OnBtSelRight();

	void           SetCtrlState		(const int iItem);

	void           PrintHead        (CPrint * pPrint);
	void           PrintRows        (CPrint * pPrint);
	
	PDlgUsrAnalysisListCtrl		* m_pli_User;
	PDlgModListCtrl				* m_pli_Mod;
	PDlgAppListCtrl				* m_pli_App;
	PDlgRightListCtrl			* m_pli_Ri;

	CButton m_bt_Sel_App;
	CButton m_bt_Sel_Mod;
	CButton m_bt_Print;
	CButton m_bt_Exit;
	CButton m_bt_Sel_Right;
	
	short m_sAppid;
	short m_sModid;
	short m_sBranchRegid;
	
	CBString m_strAppname;
	CBString m_strModname;
	CBString m_strRightname;

	enum { IDD = IDD_DLG_USR_ANALYSIS };
};

#endif
