/******************************************************************************/
/**                                                                           */
/**   Author : Bhaskar                                     */
/**   Date   : 04.07.2019                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGUSRRIGHTANALYSIS_H____
#define __PDLGUSRRIGHTANALYSIS_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgUsrGroupAnalysisListCtrl;
class PDlgUsrRightAnalysisListCtrl;
class PDlgUserListCtrl;
class PDlgAppListCtrl;
class PDlgModListCtrl;
class PDlgRightListCtrl;
class BUserAnalysis; 
class CsvFile;
class PDlgBranchListCtrl;
class BConppBranchReg;

/*----------------------------------------------------------------------------*/
class PDlgUsrRightAnalysis : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgUsrRightAnalysis)

                     PDlgUsrRightAnalysis();
	virtual			~PDlgUsrRightAnalysis();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG   * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
    afx_msg void OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDblclkUserListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnItemchangeduserListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	
	DECLARE_MESSAGE_MAP()
    afx_msg void OnRadUserGroupClicked();
    afx_msg void OnRadUserRightClicked();
    afx_msg void OnBtSelApp();
    afx_msg void OnBtSelUser();
    afx_msg void OnBtPrint();
    afx_msg void OnBtExit();
    afx_msg void OnRadAnalyByApp();
    afx_msg void OnRadAnalyByUser();

    void           SetCtrlState(const int iItem);
    void           LoadData();
    void           WriteUserGroupRows(CsvFile&  csv);
    void           WriteUserRightRows(CsvFile&  csv);
    std::string    OnFileOpen();
    void		   GetBrRegComboValue();
    void		   loadBranch();

    PDlgUserListCtrl             *m_pli_User;
    PDlgAppListCtrl              *m_pli_App;
    PDlgUsrGroupAnalysisListCtrl *m_pli_UsrGrpAnalysis;
    PDlgUsrRightAnalysisListCtrl *m_pli_UsrRightAnalysis;


    CButton m_bt_Sel_App;
    CButton m_bt_Sel_User;
    CButton m_bt_Print;
    CButton m_bt_Exit;
    CComboBox m_cb_BrReg;
    BConppBranchReg * m_pConBranch;

    short m_sAppid;
    long  m_Userid;
    int m_SelOption;
    int m_Analyse_By_Option;
    short m_sBrRegid;
    //bool m_bRegDlg;

    CBString m_strAppname;
    enum { IDD = IDD_DLG_USR_RIGHT_ANALYSIS };

    public:
    afx_msg void OnCbnSelchangeCbUrBr();
    };

#endif
