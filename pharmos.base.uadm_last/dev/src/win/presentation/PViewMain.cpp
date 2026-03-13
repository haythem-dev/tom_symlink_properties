// PViewMain.cpp : Implementierungsdatei
/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "stdafx.h"
#include "presentation.h"
#include <CVersion.h>
#include <CConnect.h>
#include "PViewMain.h"


// PViewMain
IMPLEMENT_DYNCREATE(PViewMain, CFormViewMultiLang)

PViewMain::PViewMain()
	: CFormViewMultiLang(PViewMain::IDD)
{
}

PViewMain::~PViewMain()
{
}

void PViewMain::DoDataExchange(CDataExchange* pDX)
{
	CFormViewMultiLang::DoDataExchange(pDX);
}

//--------------------------------------------------------------------
// View zeichnen
//-------------------------------------------------------------------- 
void PViewMain::OnDraw(CDC* pDC)
{

	CRect rect;
	CBrush cbr;
	CString csZiel;
	CString csTemp;

	pDC->SetBkColor(RGB(255,255,255));
	pDC->SetBkMode(OPAQUE);
	//Koordinatensystem festlegen
	pDC->SetMapMode(MM_ISOTROPIC); //Aspect 1:1

	GetClientRect(rect);

	pDC->SetWindowOrg(0,0);
	pDC->SetWindowExt(400,400);
	pDC->SetViewportOrg(rect.left,rect.top);
	pDC->SetViewportExt(rect.Width(),rect.Height());
    
	cbr.CreateSolidBrush(RGB(255,255,255)); // Weiß
    pDC->FillSolidRect(rect,RGB(255,255,255));

    pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);

	//Korrektur für Abweichungen vom Quadrat
    if (rect.right > rect.bottom)
    {
        int d = (rect.right - rect.bottom) / 2;
        rect.left  += d;
        rect.right -= d;
    }
    else
    {
        int d = (rect.bottom - rect.right) / 2;
        rect.top    += d;
        rect.bottom -= d;
    }
	pDC->SetWindowOrg(0,0);
	pDC->SetWindowExt(1000,1000);
	pDC->SetViewportOrg(rect.left,rect.top);
	pDC->SetViewportExt(rect.Width(),rect.Height());

    // Ausgangskoordinaten
    int StrichStaerkeKreis = 20;
    short x0 = 250 - StrichStaerkeKreis / 2;
    short y0 = 100 - StrichStaerkeKreis / 2;
    // green Circle in the middle
    CPen MyPen (PS_SOLID, StrichStaerkeKreis, RGB(85,140,110));//grüner Kreis des Logos
    CPen * pOldPen = pDC->SelectObject(&MyPen);
    pDC->Arc(   CRect(  x0 + StrichStaerkeKreis / 2, 
                        y0 + StrichStaerkeKreis / 2, 
                        x0 + 500 - StrichStaerkeKreis / 2, 
                        y0 + 500 - StrichStaerkeKreis / 2), 
                CPoint(x0, y0), 
                CPoint(x0, y0));

    // white quader over the part from the circle, where the font is.


    pDC->FillRect(CRect(x0, y0 + 125, x0 + 500, y0 + 375), &cbr);
    pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
    // Font in green over this
    CFont BoldFont;
    BoldFont.CreateFont(150, 0, 0, 0, FW_EXTRABOLD, 0, 0, 0, ANSI_CHARSET,
															 OUT_DEFAULT_PRECIS, 
															 CLIP_DEFAULT_PRECIS, 
															 DEFAULT_QUALITY,
															 DEFAULT_PITCH | FF_ROMAN, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&BoldFont);
    pDC->SetTextColor(RGB(85, 140, 110));//Schrift "PHOENIX"
    int dxWidth[] = {90, 110, 110, 90, 110, 50, 80};
    pDC->ExtTextOut(x0 + 245, y0 + 300, ETO_OPAQUE, CRect(0, 0, 0, 0), CResString::ex().getStrTblText(IDS_PHOENIX),7, dxWidth);

    CFont BoldFont1;
    BoldFont1.CreateFont(80, 0, 0, 0, FW_EXTRABOLD, 0, 0, 0, ANSI_CHARSET,
															  OUT_DEFAULT_PRECIS, 
															  CLIP_DEFAULT_PRECIS, 
															  DEFAULT_QUALITY,
															  DEFAULT_PITCH | FF_ROMAN, NULL);
    pDC->SetTextColor(RGB(0,0,0));

    pDC->SelectObject(&BoldFont1)->DeleteObject();

	if (CConnect::TEST_CONNECTED)
		csTemp = "TEST - ";
    
	csTemp += CResString::ex().getStrTblText(IDS_LOGIN_TITLE);
	pDC->TextOut(x0 + 250, y0 + 650, csTemp);
   
	csTemp = CResString::ex().getStrTblText(IDS_DLL_CONFIG);
	pDC->TextOut(x0 + 250, y0 + 750, csTemp);	

	CFont SmallFont;
    SmallFont.CreateFont(40, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, ANSI_CHARSET,
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
					DEFAULT_PITCH | FF_ROMAN, NULL);
    pDC->SelectObject(&SmallFont)->DeleteObject();
    pDC->SelectObject(&SmallFont)->DeleteObject();
    pDC->ExtTextOut(x0 + 320, y0 + 325, ETO_OPAQUE, CRect(0,0,0,0),
		CResString::ex().getStrTblText(IDS_PHARMA_AG)+ "             ", 36, NULL); //to avoid curios label in english!?

	CString csText;
	csText.Format("%s %s", CResString::ex().getStrTblText(IDS_VERSION) ,
		CVersion::VERSION_STRING); 
	int ilength = csText.GetLength();
    pDC->TextOut(x0 + 250, y0 + 900, csText);	

    
	CFont ThinFont;
    ThinFont.CreateFont(60, 0, 0, 0, FW_LIGHT, 0, 0, 0, ANSI_CHARSET,
														OUT_DEFAULT_PRECIS, 
														CLIP_DEFAULT_PRECIS, 
														DEFAULT_QUALITY,
														DEFAULT_PITCH | FF_ROMAN, NULL);

    pDC->SelectObject(&ThinFont)->DeleteObject();


    //delete object
    pDC->SelectObject(pOldFont)->DeleteObject();
    pDC->SelectObject(pOldPen)->DeleteObject();
}


BEGIN_MESSAGE_MAP(PViewMain, CFormViewMultiLang)
END_MESSAGE_MAP()

