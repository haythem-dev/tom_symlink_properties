//-------------------------------------------------------------------------------------
/*! \file  pcd_06_line.h
*  \brief  subclass of EDIFACTLineBase for German 06-Segment (PCD). 
*  \brief  Main purpose: Extract an DTM-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 29.10.14  pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_PCD_06_LINE_H
#define GUARD_DOMMOD_LINE_PCD_06_LINE_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasarcmnutil_decimal.h>		// for datatype decimal
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
		class Pcd_06_Line : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (2 chars long), here "PCD" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data
        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				Pcd_06_Line();							// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~Pcd_06_Line();

				// enums
				// Enumeration for base of PCD lines (percentValue causes price increase (Zuschlag) or price decrease (Abschlag))
				// Usage e.g.: domMod::line::PcdLine::PcdBaseEnum
				enum Pcd_06_BaseEnum 
				{
					enUNKNOWN,			// base is not known yet
					enPriceDecrease,	// base="1  " Preisabschlag
					enPriceIncrease,	// base="2  " Preiszuschlag
					enOTHER				// base is something else, but not processable by ABBA-UW
				};


				// Getter 
				virtual LineTypeEnum				getLineTypeEnum()		const;
				const	basar::cmnutil::Decimal		getPercentValue()		const;
						Pcd_06_BaseEnum				getBase()				const;
						basar::I18nString			getCodelist()			const;

				bool								isPriceDecrease()		const;
				bool								isCodelistOK()			const;
				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool		isValidLine()		const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				// private member functions
				bool					isBaseOK()		const;
				void					resetMembers();

				// special member functions
				Pcd_06_Line( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				Pcd_06_Line& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 
		
				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				basar::cmnutil::Decimal		m_PercentValue;			// mandatory: percent value 
				Pcd_06_BaseEnum				m_Base;					// mandatory: price reduction (1) or increase (2)
				basar::I18nString			m_Codelist;				// optional: ???. PBS999 did not evaluate field Codeliste.

		};
		
		typedef boost::shared_ptr< Pcd_06_Line > Pcd_06_LinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_PCD_06_LINE_H