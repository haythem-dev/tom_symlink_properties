/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BUserRightAnalysis.h>
#include <BConUserRightAnalysis.h>

#include "PDlgUsrRightAnalysisListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_COL_ANALYSIS_RIGHT_APPNAME                  0
#define VS_LC_COL_ANALYSIS_RIGHT_REGIONID                 1
#define VS_LC_COL_ANALYSIS_RIGHT_REGIONNAME               2
#define VS_LC_COL_ANALYSIS_RIGHT_USERNAME                 3
#define VS_LC_COL_ANALYSIS_RIGHT_SURNAME                  4
#define VS_LC_COL_ANALYSIS_RIGHT_FORENAME                 5
#define VS_LC_COL_ANALYSIS_RIGHT_MODNAME                  6
#define VS_LC_COL_ANALYSIS_RIGHT_RIGHTNAME                7
#define VS_LC_COL_ANALYSIS_RIGHT_RIGHTDESCRIPTION         8
#define VS_LC_COL_ANALYSIS_RIGHT_RIGHTDESCRIPTION_EN      9

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgUsrRightAnalysisListCtrl::init()
{
    insertCol(VS_LC_COL_ANALYSIS_RIGHT_APPNAME, CResString::ex().getStrTblText(IDS_APP), LVCFMT_LEFT, 70);
    insertCol(VS_LC_COL_ANALYSIS_RIGHT_REGIONID, CResString::ex().getStrTblText(IDS_BRANCH_NO), LVCFMT_LEFT, 50);
    insertCol(VS_LC_COL_ANALYSIS_RIGHT_REGIONNAME, CResString::ex().getStrTblText(IDS_VIEW_PREV_LI_AREA), LVCFMT_LEFT, 70);

   
    insertCol(VS_LC_COL_ANALYSIS_RIGHT_USERNAME, CResString::ex().getStrTblText( IDS_USERNAME ), LVCFMT_LEFT, 70);
    insertCol(VS_LC_COL_ANALYSIS_RIGHT_SURNAME, CResString::ex().getStrTblText( IDS_SURNAME ), LVCFMT_LEFT, 70);
    insertCol(VS_LC_COL_ANALYSIS_RIGHT_FORENAME, CResString::ex().getStrTblText( IDS_FORENAME ), LVCFMT_LEFT, 70);

    insertCol(VS_LC_COL_ANALYSIS_RIGHT_MODNAME, CResString::ex().getStrTblText(IDS_MOD), LVCFMT_LEFT, 100);
    insertCol(VS_LC_COL_ANALYSIS_RIGHT_RIGHTNAME, CResString::ex().getStrTblText(IDS_RIGHT), LVCFMT_LEFT, 100);

    
    insertCol(VS_LC_COL_ANALYSIS_RIGHT_RIGHTDESCRIPTION, CResString::ex().getStrTblText(IDS_DESC), LVCFMT_LEFT, 150);
    //insertCol(VS_LC_COL_ANALYSIS_RIGHT_RIGHTDESCRIPTION_EN, CResString::ex().getStrTblText(IDS_DESC), LVCFMT_LEFT, 150);
	return (TVConListCtrl<BConUserRightAnalysis, BUserRightAnalysis>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgUsrRightAnalysisListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BUserRightAnalysis * pAnaly = giveCon().getBsnObj(getIdx(iItem));
	assert(pAnaly != 0);

    
    SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHT_REGIONID, pAnaly->getRegionidStr().c_str());
    SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHT_REGIONNAME, pAnaly->getAreaname().c_str());
    SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHT_APPNAME, pAnaly->getAppname().c_str());
    

    SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHT_USERNAME, pAnaly->getUsername().c_str());
    SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHT_SURNAME, pAnaly->getSurname().c_str());
    SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHT_FORENAME, pAnaly->getForename().c_str());

    SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHT_MODNAME, pAnaly->getModname().c_str());
    SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHT_RIGHTNAME, pAnaly->getRightname().c_str());
    SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHT_RIGHTDESCRIPTION, pAnaly->getRightDesc().c_str());
   // SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHT_RIGHTDESCRIPTION_EN, pAnaly->getRightDesc_EN().c_str());
}

bool PDlgUsrRightAnalysisListCtrl::searchSurname(CBString strFirst)
{
    BUserRightAnalysis*     pUserRightAnaly = 0;
    unsigned int    uiCnt = 0;
    CBString		cbFirstChar;
    CBString		cbSurname;

    int iSel = -1;
    getNextSelectedBsnObj(&iSel);

    if (iSel >= 0)
        unselectBsnObj(getIdx(iSel));

    while ((pUserRightAnaly = giveCon().getBsnObj(uiCnt++)) != 0)
    {
        cbSurname = pUserRightAnaly->getSurname();
        if (!cbSurname.empty())
        {
            cbFirstChar = cbSurname.substr(0, 1);
            cbFirstChar.upper();

            if (cbFirstChar == strFirst)
            {
                selectBsnObj(uiCnt - 1);
                return true;
                break;
            }
        }
    }
    selectBsnObj(getIdx(iSel)); // alte Selektion wieder setzen
    return false; //Key nicht gefunden
}
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
