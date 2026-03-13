//-------------------------------------------------------------------------------------
/*! \file  unhline.h
*  \brief  subclass of EDIFACTLineBase for UNH-Segment. 
*  \brief  Main purpose: Extract an UNH-Line from a flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  \date   20.03.2013
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_UNHLINE_H
#define GUARD_DOMMOD_LINE_UNHLINE_H

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
		class UnhLine : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (3 chars long), "UNH" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				UnhLine();								

				// Getter: 
				virtual		LineTypeEnum		getLineTypeEnum()		const;

				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool		isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				void					resetMembers();

				// special member functions: the standard ones are ok but necessary
				//~UnhLine();
				//UnhLine( const EDIFACTLineBase& ) ;			// Copy-Konstruktor : abschalten
				//UnhLine& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 

				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				bool					m_extractedCorrectlyFlag;	// if set: process() has read a valid line
		};
		
		typedef boost::shared_ptr< UnhLine > UnhLinePtr;		

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_UNHLINE_H
