/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once


/*----------------------------------------------------------------------------*/
//  Class Declaration
/*----------------------------------------------------------------------------*/
class PMainFrm : public CFrameWndMultiLang
{

public:

	virtual		   ~PMainFrm      ();

	void			SetConnect		(LPCTSTR spTxt);
	void			SetUser  		(LPCTSTR spTxt);
	
	
protected: 
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	
	DECLARE_DYNCREATE(PMainFrm)

					PMainFrm   		();
	virtual BOOL	PreCreateWindow		(CREATESTRUCT& cs);
	virtual void    GetMessageString    (UINT nID, CString& rMessage) const;

	
	//MFC-generierte Meldungshandler
	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnInfoDoku();
	afx_msg void	OnInfoAbout();
	afx_msg void	OnAdmUser();
	afx_msg void	OnAdmGroup();
	afx_msg void	OnDllDoku();
	afx_msg void	OnAdmUserRights();
	afx_msg void	OnAdmGroupRights();
	afx_msg void	OnDLLAppModRi();
	afx_msg void	OnDllBranchReg();
	afx_msg void	OnDbconfigGroup();
	afx_msg void	OnDbkonfigurierenTexte();
	afx_msg void	OnDbkonfigurierenLabel();
	afx_msg void	OnUsertypes();
	afx_msg void    OnUserParameters();
	afx_msg void	OnAppParameters();
	afx_msg void	OnDbkonfigurierenPreviouslogin();
	afx_msg void	OnChangeHost();
	afx_msg void	OnExpUsrRights();
	afx_msg void	OnExpGrpRights();
	afx_msg void	OnExpAnalog();
	afx_msg void	OnExpCorUser();
	afx_msg void	OnBenutzerRechteAnalyse();
	afx_msg void	OnUserAnalyse();
	afx_msg void    OnUserManager();
	
	DECLARE_MESSAGE_MAP()


private:

	CStatusBar                  m_wndStatusBar;
	static UINT                 m_uiIndicators[];

public:
    afx_msg void OnAnalyseUserrights();
};

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
