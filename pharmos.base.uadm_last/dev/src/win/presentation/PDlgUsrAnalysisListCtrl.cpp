/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BUserAnalysis.h>
#include <BConUserAnalysis.h>

#include "PDlgUsrAnalysisListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_COL_ANALYSIS_APP	        0
#define VS_LC_COL_ANALYSIS_MOD          1
#define VS_LC_COL_ANALYSIS_AREA         2
#define VS_LC_COL_ANALYSIS_USERNAME     3
#define VS_LC_COL_ANALYSIS_SURNAME      4
#define VS_LC_COL_ANALYSIS_FORENAME     5
#define VS_LC_COL_ANALYSIS_RIGHT        6

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgUsrAnalysisListCtrl::init()
{
	insertCol(VS_LC_COL_ANALYSIS_APP  , CResString::ex().getStrTblText(IDS_APP   ), LVCFMT_LEFT  , 50);
	insertCol(VS_LC_COL_ANALYSIS_MOD  , CResString::ex().getStrTblText(IDS_MOD  ), LVCFMT_LEFT  , 70);
	insertCol(VS_LC_COL_ANALYSIS_AREA , CResString::ex().getStrTblText(IDS_VIEW_PREV_LI_AREA   ), LVCFMT_LEFT  , 70);
	insertCol(VS_LC_COL_ANALYSIS_USERNAME  , CResString::ex().getStrTblText(IDS_USERNAME  ), LVCFMT_LEFT  , 70);
	insertCol(VS_LC_COL_ANALYSIS_SURNAME   , CResString::ex().getStrTblText(IDS_SURNAME   ), LVCFMT_LEFT  , 70);
	insertCol(VS_LC_COL_ANALYSIS_FORENAME  , CResString::ex().getStrTblText(IDS_FORENAME  ), LVCFMT_LEFT  , 70);
	insertCol(VS_LC_COL_ANALYSIS_RIGHT     , CResString::ex().getStrTblText(IDS_RIGHT   ), LVCFMT_LEFT  , 100);
	
	return (TVConListCtrl<BConUserAnalysis, BUserAnalysis>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgUsrAnalysisListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BUserAnalysis * pAnaly = giveCon().getBsnObj(getIdx(iItem));
	assert(pAnaly != 0);

	SetItemText(iItem, VS_LC_COL_ANALYSIS_APP   ,  pAnaly->getAppname().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_MOD   ,  pAnaly->getModname().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_AREA  ,  pAnaly->getAreaname().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_USERNAME  ,  pAnaly->getUsername().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_SURNAME   ,  pAnaly->getSurname().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_FORENAME  ,  pAnaly->getForename().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHT     ,  giveCon().getRightname().c_str());	
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
