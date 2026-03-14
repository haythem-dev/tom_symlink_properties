//-------------------------------------------------------------------------------------
/*! \file  irrline.h
*  \brief  subclass of EDIFACTLineBase for Segments TDT, LOC, FTX, RFF, PRI, MOA. 
*  \brief  These segments can be included in an UEWAUF flatfile, but they are not 
*  \brief  evaluated by ABBA-UW.
*  \brief  Main purpose: Extract an irrelevant Line from a flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                All types of known irelevant Lines can be processed within this method
*  \author P. Kessling
*  \date   28.03.2013, 
*  history 28.03.13  pk new
*/
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_DOMMOD_LINE_IRRLINE_H
#define GUARD_DOMMOD_LINE_IRRLINE_H

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
		class IrrLine : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				IrrLine();								// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~IrrLine();

				// enums
				// Enumeration for line types of irrelevant or unknown line types 
				// found in UEWAUF flatfiles
				// Usage e.g.: domMod::line::IrrLine::IrrLineTypeEnum
				enum IrrLineTypeEnum 
				{
					enUNKNOWN,	// line type is not known yet
					enTDT,		// TDT line
					enLOC,		// LOC line
					enFTX,		// FTX line
					enRFF,		// RFF line
					enPRI,		// PRI line
					enMOA,		// MOA line
					enOTHER		// line type is something else, it is not described in the UEWAUF documentation.
								// This is suspicious to ABBA-UW! Some Error may have occurred.
				};


				// Getter 
				virtual 	LineTypeEnum					getLineTypeEnum()	const;
				domMod::line::IrrLine::IrrLineTypeEnum		getLineType()		const;
				const basar::I18nString&					getLine()			const;

				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool	isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				// privat member functions
				void					resetMembers();

				// special member functions
				IrrLine( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				IrrLine& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 

				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				domMod::line::IrrLine::IrrLineTypeEnum	m_LineType;		// Type of Line
				basar::I18nString						m_Line;			// Complete line as string

		};
		
		typedef boost::shared_ptr< IrrLine > IrrLinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_IRRLINE_H