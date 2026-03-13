/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BppUsertype.h>
#include <BConppUsertype.h>

#include "PDlgUsertypeListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_COL_USERTYPEID		0
#define VS_LC_COL_USERTYPENAME		1
#define VS_LC_COL_USERTYPEDESC		2
#define VS_LC_COL_USERTYPECNT		3

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgUsertypeListCtrl::init()
{
	insertCol(VS_LC_COL_USERTYPEID, CResString::ex().getStrTblText(IDS_DLG_USERTYPE_LI_ID), LVCFMT_RIGHT, 40);
	insertCol(VS_LC_COL_USERTYPENAME, CResString::ex().getStrTblText(IDS_DLG_USERTYPE_LI_NAME), LVCFMT_LEFT, 220);
	insertCol(VS_LC_COL_USERTYPEDESC, CResString::ex().getStrTblText(IDS_DLG_USERTYPE_LI_DESC), LVCFMT_LEFT, 300);
	insertCol(VS_LC_COL_USERTYPECNT, CResString::ex().getStrTblText(IDS_DLG_USERTYPE_LI_COUNT), LVCFMT_RIGHT, 50);

	return (TVConListCtrl<BConppUsertype, BppUsertype>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgUsertypeListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BppUsertype* pUsertype = giveCon().getBsnObj(getIdx(iItem));
	assert(pUsertype != 0);

	SetItemText(iItem, VS_LC_COL_USERTYPEID, pUsertype->getUsertypeidStr().c_str());
	SetItemText(iItem, VS_LC_COL_USERTYPENAME, pUsertype->getName().c_str());
	SetItemText(iItem, VS_LC_COL_USERTYPEDESC, pUsertype->getDescription().c_str());
	SetItemText(iItem, VS_LC_COL_USERTYPECNT, pUsertype->getUserCountStr().c_str());
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
