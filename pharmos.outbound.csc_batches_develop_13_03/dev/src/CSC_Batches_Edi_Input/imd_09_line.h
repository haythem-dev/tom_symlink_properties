//-------------------------------------------------------------------------------------
/*! \file  imd_09_line.h
*  \brief  subclass of EDIFACTLineBase for German 09-Segment (IMD). 
*  \brief  Main purpose: Extract an IMD-Line rom a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 27.10.14  pk new
*/
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_DOMMOD_LINE_IMD_09_LINE_H
#define GUARD_DOMMOD_LINE_IMD_09_LINE_H

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
		class Imd_09_Line : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (2 chars long), here "09" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				Imd_09_Line();							// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~Imd_09_Line();

				// Getter 
				virtual			LineTypeEnum		getLineTypeEnum()		const;

				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool		isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				void				resetMembers();

				// special member functions
				Imd_09_Line( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				Imd_09_Line& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 
		
				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				bool					m_extractedCorrectlyFlag;	// if set: process() has read a valid line

		};
		
		typedef boost::shared_ptr< Imd_09_Line > Imd_09_LinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_IMD_09_LINE_H




