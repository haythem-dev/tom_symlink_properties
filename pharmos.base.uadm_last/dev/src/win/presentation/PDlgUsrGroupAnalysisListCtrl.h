/******************************************************************************/
/**                                                                           */
/**   Author : BHaskar		                                   */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef _PDLGUSRGROUPANALYSISLISTCRTL_H__________
#define _PDLGUSRGROUPANALYSISLISTCRTL_H__________

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BUserGroupAnalysis;
class BConUserGroupAnalysis;

/*----------------------------------------------------------------------------*/
//  Class Declaration
/*----------------------------------------------------------------------------*/
class PDlgUsrGroupAnalysisListCtrl : public TVConListCtrl<BConUserGroupAnalysis, BUserGroupAnalysis>
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