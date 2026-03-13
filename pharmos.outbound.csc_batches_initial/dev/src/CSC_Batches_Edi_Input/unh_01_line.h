//-------------------------------------------------------------------------------------
/*! \file  unh_01_line.h
*  \brief  subclass of EDIFACTLineBase for German 01-Segment. 
*  \brief  Main purpose: Extract an 01-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  \date   27.10.2014
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_UNH_01_LINE_H
#define GUARD_DOMMOD_LINE_UNH_01_LINE_H

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
		class Unh_01_Line : public EDIFACTLineBase 
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
				Unh_01_Line();								

				// Getter: 
				virtual		LineTypeEnum		getLineTypeEnum()		const;

				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool		isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				bool					isUnhTypOK()			const;
				bool					isUnhVersionOK()		const;
				bool					isUnhFreigabeOK()		const;
				void					resetMembers();

				// special member functions: the standard ones are ok but necessary
				//~UnhLine();
				//UnhLine( const EDIFACTLineBase& ) ;			// Copy-Konstruktor : abschalten
				//UnhLine& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 

				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				basar::I18nString       m_FixedUnhTyp;				// must/should be "UEWAUF" regarding documentation
				basar::I18nString       m_FixedUnhVersion;			// must/should be "S  " regarding documentation
				basar::I18nString       m_FixedUnhFreigabe;			// must/should be "93A" regarding documentation

				bool					m_extractedCorrectlyFlag;	// if set: process() has read a valid line
		};
		
		typedef boost::shared_ptr< Unh_01_Line > Unh_01_LinePtr;		

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_UNH_01_LINE_H
