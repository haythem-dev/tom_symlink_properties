//-------------------------------------------------------------------------------------
/*! \file  ftx_11_line.h
*  \brief  subclass of EDIFACTLineBase for German 11-Segment (FTX). 
*  \brief  Main purpose: Extract an FTX-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 07.11.14  pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_FTX_11_LINE_H
#define GUARD_DOMMOD_LINE_FTX_11_LINE_H

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
		class Ftx_11_Line : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (2 chars long), here "FTX" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				Ftx_11_Line();							// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~Ftx_11_Line();

				// Getter 
				virtual			LineTypeEnum		getLineTypeEnum()	const;
				const			basar::I18nString&	getFreeText()		const;

				// Checker
				bool			isOrderheadText()	const;
				bool			isOrderposText()	const;

				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool		isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				// privat member functions
				void					resetMembers();

				// special member functions
				Ftx_11_Line( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				Ftx_11_Line& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 
	
				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				basar::I18nString  m_ZuCode;		// should contain 'AAQ' to be orderheadtext
				basar::I18nString  m_FreeText;		// text

		};
		
		typedef boost::shared_ptr< Ftx_11_Line > Ftx_11_LinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_FTX_11_LINE_H


