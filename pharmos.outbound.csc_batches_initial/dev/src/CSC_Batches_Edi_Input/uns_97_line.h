//-------------------------------------------------------------------------------------
/*! \file  uns_97_line.h
*  \brief  subclass of EDIFACTLineBase for German 97-Segment (UNS).  . 
*  \brief  Main purpose: Extract an 97-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                Both types of UNS-Lines ('D' und 'S') can be processed within this method
*  \author P. Kessling
*  history 27.10.14  pk  new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_UNS_97_LINE_H
#define GUARD_DOMMOD_LINE_UNS_97_LINE_H
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
		class Uns_97_Line : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (2 chars long), here "97" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data
		
        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				Uns_97_Line();							// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	

				// Getter
				virtual			LineTypeEnum		getLineTypeEnum()					const;
				bool								isBeginOfPositionSection()			const;

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
				basar::I18nString       m_Qualifier;			// Type of Uns_97_Line: Must be "D" (begin of Position-section) or "S" (begin or end of position-section) 


		};
		
		typedef boost::shared_ptr< Uns_97_Line > Uns_97_LinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_UNS_97_LINE_H