//-------------------------------------------------------------------------------------
/*! \file  qty_10_line.h
*  \brief  subclass of EDIFACTLineBase for German 10-Segment (QTY). 
*  \brief  Main purpose: Extract an DTM-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 28.10.14  pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_QTY_10_LINE_H
#define GUARD_DOMMOD_LINE_QTY_10_LINE_H
//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#include "edifactline.h"
//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//

namespace domMod
{
	namespace line
	{
    //-------------------------------------------------------------------------------------------------//
    // class declaration section
    //-------------------------------------------------------------------------------------------------//
		class Qty_10_Line : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (2 chars long), here "QTY" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data
        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				Qty_10_Line();							// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~Qty_10_Line();

				// enums
				// Enumeration for types of QTY lines 
				// Usage e.g.: domMod::line::Qty_10_Line::Qty_10_QualifierEnum
				enum Qty_10_QualifierEnum 
				{
					enUNKNOWN,		// qualifier is not known yet
					enChargedQty,	// qualifier=21 (ordered quantity (without noncharged qty))
					enNonChargedQty,// qualifier=130 (non charged quantity)
					enOTHER			// qualifier is something else, but not processable by ABBA-UW
				};


				// Getter 
				virtual			LineTypeEnum									getLineTypeEnum()	const;
								domMod::line::Qty_10_Line::Qty_10_QualifierEnum	getQualifier()		const;
								basar::Int32									getQuantity()		const;
								void											setQuantity(const basar::Int32 qty)	;

				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool		isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				// privat member functions
				bool				isQualifierOK()	const;
				void				resetMembers();

				// special member functions
				Qty_10_Line( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				Qty_10_Line& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 
		
				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				domMod::line::Qty_10_Line::Qty_10_QualifierEnum	m_Qualifier;// mandatory: Type of QTYLine, must be "21 " or "47 ", nothing else 
				basar::Int32									m_Quantity;	// mandatory: quantity


		};
		
		typedef boost::shared_ptr< Qty_10_Line > Qty_10_LinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_QTY_10_LINE_H