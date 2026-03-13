/******************************************************************************/
/**                                                                           */
/**   Author : BHaskar		                                   */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef _PDLGUSRRIGHTANALYSISLISTCRTL_H__________
#define _PDLGUSRRIGHTANALYSISLISTCRTL_H__________

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BUserRightAnalysis;
class BConUserRightAnalysis;

/*----------------------------------------------------------------------------*/
//  Class Declaration
/*----------------------------------------------------------------------------*/
class PDlgUsrRightAnalysisListCtrl : public TVConListCtrl<BConUserRightAnalysis, BUserRightAnalysis>
{
public:
	virtual bool                 init                ();
    virtual bool                 searchSurname       (CBString);

private:
  	virtual void                 refreshItem         (const int iItem);
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/