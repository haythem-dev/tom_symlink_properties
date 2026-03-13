/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BUserGroupAnalysis.h>
#include <BConUserGroupAnalysis.h>

#include "PDlgUsrGroupAnalysisListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_COL_ANALYSIS_GROUP_APPNAME      0
#define VS_LC_COL_ANALYSIS_GROUP_GROUPNAME    1
#define VS_LC_COL_ANALYSIS_GROUP_USERNAME     2
#define VS_LC_COL_ANALYSIS_GROUP_SURNAME      3
#define VS_LC_COL_ANALYSIS_GROUP_FORENAME     4
#define VS_LC_COL_ANALYSIS_GROUP_USERCOUNTRY  5
#define VS_LC_COL_ANALYSIS_GROUP_USERTYPE     6


/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgUsrGroupAnalysisListCtrl::init()
{
    insertCol(VS_LC_COL_ANALYSIS_GROUP_APPNAME, CResString::ex().getStrTblText(IDS_APP), LVCFMT_LEFT, 100);
    insertCol(VS_LC_COL_ANALYSIS_GROUP_GROUPNAME, CResString::ex().getStrTblText(IDS_DLG_GROUP_LI_NAME), LVCFMT_LEFT  , 150);
	insertCol(VS_LC_COL_ANALYSIS_GROUP_USERNAME , CResString::ex().getStrTblText(IDS_USERNAME  ), LVCFMT_LEFT  , 100);
	insertCol(VS_LC_COL_ANALYSIS_GROUP_SURNAME  , CResString::ex().getStrTblText(IDS_SURNAME   ), LVCFMT_LEFT  , 100);
	insertCol(VS_LC_COL_ANALYSIS_GROUP_FORENAME , CResString::ex().getStrTblText(IDS_FORENAME  ), LVCFMT_LEFT  , 100);
    insertCol(VS_LC_COL_ANALYSIS_GROUP_USERCOUNTRY, CResString::ex().getStrTblText(IDS_VIEW_USER_LI_USERCOUNTRY), LVCFMT_LEFT, 50);
    insertCol(VS_LC_COL_ANALYSIS_GROUP_USERTYPE, CResString::ex().getStrTblText(IDS_VIEW_USER_LI_USERTYPE), LVCFMT_LEFT, 100);
	return (TVConListCtrl<BConUserGroupAnalysis, BUserGroupAnalysis>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgUsrGroupAnalysisListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BUserGroupAnalysis * pAnaly = giveCon().getBsnObj(getIdx(iItem));
	assert(pAnaly != 0);

    SetItemText(iItem, VS_LC_COL_ANALYSIS_GROUP_APPNAME, pAnaly->getAppname().c_str());
    SetItemText(iItem, VS_LC_COL_ANALYSIS_GROUP_GROUPNAME,  pAnaly->getGroupname().c_str());
    SetItemText(iItem, VS_LC_COL_ANALYSIS_GROUP_USERNAME,   pAnaly->getUsername().c_str());
    SetItemText(iItem, VS_LC_COL_ANALYSIS_GROUP_SURNAME,    pAnaly->getSurname().c_str());
    SetItemText(iItem, VS_LC_COL_ANALYSIS_GROUP_FORENAME,   pAnaly->getForename().c_str());
    SetItemText(iItem, VS_LC_COL_ANALYSIS_GROUP_USERCOUNTRY, pAnaly->getUserCountry().c_str());
    SetItemText(iItem, VS_LC_COL_ANALYSIS_GROUP_USERTYPE, pAnaly->getUserType().c_str());
}

bool PDlgUsrGroupAnalysisListCtrl::searchSurname(CBString strFirst)
{
    BUserGroupAnalysis*     pUserGroupAnaly = 0;
    unsigned int    uiCnt = 0;
    CBString		cbFirstChar;
    CBString		cbSurname;

    int iSel = -1;
    getNextSelectedBsnObj(&iSel);

    if (iSel >= 0)
        unselectBsnObj(getIdx(iSel));

    while ((pUserGroupAnaly = giveCon().getBsnObj(uiCnt++)) != 0)
    {
        cbSurname = pUserGroupAnaly->getSurname();
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
