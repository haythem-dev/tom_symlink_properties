//-------------------------------------------------------------------------------------
/*! \file  qtyline.h
*  \brief  subclass of EDIFACTLineBase for QTY-Segment. 
*  \brief  Main purpose: Extract an QTY-Line from a flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                All types of QTY-Lines can be processed within this method
*  \author P. Kessling
*  \date   27.03.2013, 
*  history 27.03.13  pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_QTYLINE_H
#define GUARD_DOMMOD_LINE_QTYLINE_H
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
		class QtyLine : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (3 chars long), here "QTY" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data
        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				QtyLine();								// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~QtyLine();

				// enums
				// Enumeration for types of QTY lines 
				// Usage e.g.: domMod::line::QtyLine::QtyQualifierEnum
				enum QtyQualifierEnum 
				{
					enUNKNOWN,		// qualifier is not known yet
					enOrderedQty,	// qualifier=21 (ordered quantity)
					enChargedQty,	// qualifier=47 (charged quantity)
					enOTHER			// qualifier is something else, but not processable by ABBA-UW
				};


				// Getter 
				virtual			LineTypeEnum								getLineTypeEnum()	const;
								domMod::line::QtyLine::QtyQualifierEnum		getQualifier()		const;
								basar::Int32								getQuantity()		const;
				const			basar::I18nString&							getUnit()			const;

				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool		isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				// privat member functions
				bool				isQualifierOK()	const;
				bool				isUnitOK()		const;
				void				resetMembers();

				// special member functions
				QtyLine( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				QtyLine& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 
		
				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				domMod::line::QtyLine::QtyQualifierEnum	m_Qualifier;// mandatory: Type of QTYLine, must be "21 " or "47 ", nothing else 
				basar::Int32							m_Quantity;	// mandatory: quantity
				basar::I18nString						m_Unit;		// optional: Unit of quntity, "PCE" = pieces


		};
		
		typedef boost::shared_ptr< QtyLine > QtyLinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_QTYLINE_H