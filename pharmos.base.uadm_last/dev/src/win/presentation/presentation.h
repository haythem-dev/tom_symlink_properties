// presentation.h : Hauptheaderdatei für die presentation-Anwendung
/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __PRESENTATION_H______
#define __PRESENTATION_H______

#include "resource.h"       

class ppBLogIn;
/*----------------------------------------------------------------------------*/
class CpresentationApp : public CWinAppMultiLang
{
public:
	CpresentationApp();
	virtual ~CpresentationApp();

	//für DLL-Check
	struct T_Dll
	{
		const char *  m_spName;
		const char *  (* m_fpProdVersion)();
		const char *  (* m_fpVersion    )();

		T_Dll (const char * spName            ,
			   const char * (*fpProdVersion)(),
			   const char * (*fpVersion    )())
			  : m_spName       (spName       ),
			    m_fpProdVersion(fpProdVersion),
			    m_fpVersion    (fpVersion    )
		{};
	};

	static T_Dll m_sDll[14];

	CStringList m_lstUserRoles;

protected:
	virtual BOOL	InitInstance   ();
	virtual int		ExitInstance   ();
	
	BOOL			Login          ();
	bool			ConnectDB      ();
	bool			DisconnectDB   ();
	bool            CheckDlls      ();

	bool			openCfgFile		();
	bool			processCfgFile	(std::string& host_main, std::string& host_backup, std::string& db_main, std::string& db_backup);
	CString         GetBackgroundImagePath();

	class CStdioFile *m_TextDat;
};

extern CpresentationApp theApp;

#endif