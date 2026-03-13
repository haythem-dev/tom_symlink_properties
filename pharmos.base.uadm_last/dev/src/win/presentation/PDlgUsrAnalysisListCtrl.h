/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef _PDLGUSRANALYSISLISTCRTL_H__________
#define _PDLGUSRANALYSISLISTCRTL_H__________

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BUserAnalysis;
class BConUserAnalysis;

/*----------------------------------------------------------------------------*/
//  Class Declaration
/*----------------------------------------------------------------------------*/
class PDlgUsrAnalysisListCtrl : public TVConListCtrl<BConUserAnalysis, BUserAnalysis>
{
public:
	virtual bool                 init                ();

private:
  	virtual void                 refreshItem         (const int iItem);
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/