//-------------------------------------------------------------------------------------
/*! \file  unzline.h
*  \brief  subclass of EDIFACTLineBase for UNZ-Segment. 
*  \brief  Main purpose: Extract an UNZ-Line from a flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  \date   26.03.2013
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_UNZLINE_H
#define GUARD_DOMMOD_LINE_UNZLINE_H

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
		class UnzLine : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (3 chars long), "UNZ" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data
        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				UnzLine();								// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~UnzLine();

				// Getter: 
				virtual 	LineTypeEnum		getLineTypeEnum()		const;

				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool		isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				void					resetMembers();

				// special member functions
				UnzLine( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				UnzLine& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 
		
				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				bool					m_extractedCorrectlyFlag;	// if set: process() has read a valid line


		};
		
		typedef boost::shared_ptr< UnzLine > UnzLinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_UNZLINE_H
