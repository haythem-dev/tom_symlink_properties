/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PVIEWMAIN_H____
#define __PVIEWMAIN_H____


// PViewMain-Formularansicht
class PViewMain : public CFormViewMultiLang
{
	DECLARE_DYNCREATE(PViewMain)

protected:
	PViewMain();           // Dynamische Erstellung verwendet geschützten Konstruktor
	virtual ~PViewMain();
	virtual void	OnDraw(CDC* pDC);      // overridden to draw this view


public:
	enum { IDD = IDD_VIEW_MAIN };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
};

#endif

