//-------------------------------------------------------------------------------------
/*! \file  alc_05_line.h
*  \brief  subclass of EDIFACTLineBase for German 05-Segment (ALC).  . 
*  \brief  Main purpose: Extract an 05-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 07.11.14  pk  new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_ALC_05_LINE_H
#define GUARD_DOMMOD_LINE_ALC_05_LINE_H
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
		class Alc_05_Line : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (2 chars long), here "05" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data
		
        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				Alc_05_Line();							// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	

				// enums
				// Enumeration for Roles of participants with respect to NAD-Lines
				// Usage e.g.: domMod::line::NadLine::NadRoleEnum
				enum AlcPriceHandlingTypeEnum 
				{
					enUNKNOWN,	// Type is not known yet
					enH,		// price drop
					enM,		// special Steigerwald: same as 'H' but for non-charged quantity
					enF,		// price drop
					enK,		// price increase
					enOTHER		// role is something else, but not relevant to ABBA-UW
				};

				// Getter
				virtual			LineTypeEnum									getLineTypeEnum()			const;
				const domMod::line::Alc_05_Line::AlcPriceHandlingTypeEnum&		getPriceHandling()			const;

				void								process(const std::string& lineStr);		// extracts lineStr to members
				bool								isValidLine()						const;

				virtual EDIFACTLineBasePtr			clone()								const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				// privat member functions
				void					resetMembers();

				// special member functions: the standard ones are ok but necessary
				//~Uns_97_Line();
				//Uns_97_Line( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				//Uns_97_Line& operator=( const EDIFACTLineBase& );		// Assignment operator : abschalten 

				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				AlcPriceHandlingTypeEnum       m_PriceHandling;			// Type of Alc_05_Line: Must be 'F', 'K', (for Steigerwald and others:) 'H', 'M'
																		// When m_PriceHandling ='H' or 'M': NR nonchargedqty of the position would be accepted
																		//				        = otherwise: NR nonchargedqty of the position would be ignored

		};
		
		typedef boost::shared_ptr< Alc_05_Line > Alc_05_LinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_ALC_05_LINE_H