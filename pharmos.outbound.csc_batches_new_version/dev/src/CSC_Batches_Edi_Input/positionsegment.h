//-------------------------------------------------------------------------------------
/*! \file  positionsegment.h
*  \brief  class for the payload in header-segment. 
*  \author Jia Li, P. Kessling
*  \date   02.01.2013 
*  history: 09.04.13 pk rework
*			10.11.14 pk DE stuff added
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_TRANSFERORDERDM_POSITIONSEGMENT_H
#define GUARD_DOMMOD_TRANSFERORDERDM_POSITIONSEGMENT_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#include "linline.h"
#include "imdline.h"
#include "qtyline.h"
#include "pcdline.h"

#include "lin_08_line.h"
#include "qty_10_line.h"
#include "alc_05_line.h"
#include "pcd_06_line.h"
#include "ftx_11_line.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//

namespace domMod
{
	namespace transferOrderDM
	{
    //-------------------------------------------------------------------------------------------------//
    // class declaration section
    //-------------------------------------------------------------------------------------------------//
		class PositionSegment 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
			public:

				PositionSegment();
				~PositionSegment();

				// Getter
				basar::Int32					getArticleno() const;	
				void							setArticleno(const basar::Int32 articleno);	
				const basar::I18nString&		getArticleCode() const;	
				void							setArticleCode(const basar::I18nString article_code);	
				const basar::I18nString&		getTextfield() const;	
				void							setTextfield(const basar::I18nString textfield);	
				basar::cmnutil::Decimal			getWholesalerpurchaseprice() const;	
				void							setWholesalerpurchaseprice(const basar::Decimal wholesalerpurchaseprice);	
				basar::cmnutil::Decimal			getDiscountPct() const;	
				void							setDiscountPct(const basar::cmnutil::Decimal discountPct);	

				// for InhCH
				domMod::line::LinLinePtr		getLinLine_LinLinePtr()	const;	
				domMod::line::ImdLinePtr		getImdLine_ImdLinePtr()	const;	
				domMod::line::QtyLinePtr		getQtyLine_OrderedQtyPtr()	const;	
				domMod::line::QtyLinePtr		getQtyLine_ChargedQuantityPtr()	const;	
				domMod::line::PcdLinePtr		getPcdLine_PcdLinePtr()	const;	

				// for InhDE
				domMod::line::Lin_08_LinePtr	getLin_08_Line_Lin_08_LinePtr()	const;	
				domMod::line::Alc_05_LinePtr	getAlc_05_Line_Alc_05_LinePtr()	const;	
				domMod::line::Pcd_06_LinePtr	getPcd_06_Line_Pcd_06_LinePtr()	const;	
				domMod::line::Qty_10_LinePtr	getQty_10_Line_ChargedQtyPtr()	const;	
				domMod::line::Qty_10_LinePtr	getQty_10_Line_NonChargedQtyPtr()	const;	
				domMod::line::Ftx_11_LinePtr	getFtx_11_Line_PosPtr()	const;	


				bool empty();						// clear all stored lines

				// mount a processed and valid line into this position segment
				bool mountLINLine(const domMod::line::LinLinePtr&);	
				bool mountIMDLine(const domMod::line::ImdLinePtr&);	
				bool mountQTYLine(const domMod::line::QtyLinePtr&);	
				bool mountPCDLine(const domMod::line::PcdLinePtr&);	

				bool mountLIN_08_Line(const domMod::line::Lin_08_LinePtr&);
				bool mountQTY_10_Line(const domMod::line::Qty_10_LinePtr&);
				bool mountALC_05_Line(const domMod::line::Alc_05_LinePtr&);
				bool mountPCD_06_Line(const domMod::line::Pcd_06_LinePtr&);
				bool mountFTX_11_Pos_Line(const domMod::line::Ftx_11_LinePtr&);

																		// put this line to the correct place in envelope-segment
				bool isValidInhCH() const;								// is Info complete and ok for storage?
				bool isValidInhDE() const;								// is Info complete and ok for storage?

			private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////

				void					resetMembers();

				PositionSegment( const PositionSegment& );
				PositionSegment& operator=( const PositionSegment& );
	

				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				domMod::line::LinLinePtr m_LinLinePtr;			
				domMod::line::ImdLinePtr m_ImdLinePtr;			
				domMod::line::QtyLinePtr m_QtyLine_OrderedQtyPtr;			// QTY_21	(Gesamte Menge: Bestellmenge inkl. NR!)
				domMod::line::QtyLinePtr m_QtyLine_ChargedQuantityPtr;		// QTY_47	(Bestellmenge = Rechnungsmenge (ohne NR))
				domMod::line::PcdLinePtr m_PcdLinePtr;	


				domMod::line::Lin_08_LinePtr	m_Lin_08_LinePtr;			
				domMod::line::Qty_10_LinePtr	m_Qty_10_Line_ChargedQtyPtr;			// Qualifier 21 = ordered quantity without non-charged qty 
				domMod::line::Qty_10_LinePtr	m_Qty_10_Line_NonChargedQtyPtr;			// Qualifier 130	= noncharged quantity
				domMod::line::Alc_05_LinePtr	m_Alc_05_LinePtr;			
				domMod::line::Pcd_06_LinePtr	m_Pcd_06_LinePtr;			
				domMod::line::Ftx_11_LinePtr	m_Ftx_11_Line_PosPtr;					// FTX_11 as a Position text (Zucode="AAA")	



				// Additional order info for info into orderpos table
				basar::Int32				m_Articleno;				// interne Articleno im preferred format (preferred_fag gesetzt)
				basar::I18nString			m_Article_code;				// interne Articleno im preferred format
				basar::I18nString			m_Textfield;				// 
				basar::cmnutil::Decimal		m_Wholesalerpurchaseprice;	// price for one item, 
																		// For display it has to be multiplicated by (orderedqty minus NR und BR) 			
				basar::cmnutil::Decimal		m_DiscountPct;

		};

		typedef boost::shared_ptr< PositionSegment > PositionSegmentPtr;

	} // namespace transferOrderDM
} // namespace domMod

#endif // GUARD_DOMMOD_TRANSFERORDERDM_POSITIONSEGMENT_H