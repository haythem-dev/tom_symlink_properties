//-------------------------------------------------------------------------------------
/*! \file  irrelevcant_de_line.h
*  \brief  subclass of EDIFACTLineBase for Segments 07 (MOA), 11 (FTX) , 12 (PRI)
*  \brief  These segments can be included in a GERMAN .inh-flatfile, but they are not 
*  \brief  evaluated by ABBA-UW.
*  \brief  Main purpose: Extract an irrelevant Line from a flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                All types of known irelevant Lines can be processed within this method
*  \author P. Kessling
*  history 29.10.14  pk new
*/
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_DOMMOD_LINE_IRRELEVANT_DE_LINE_H
#define GUARD_DOMMOD_LINE_IRRELEVANT_DE_LINE_H

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
		class IrrLine_DE : public EDIFACTLineBase 
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
				IrrLine_DE();							// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~IrrLine_DE();

				// enums
				// Enumeration for line types of irrelevant or unknown line types 
				// found in UEWAUF flatfiles
				// Usage e.g.: domMod::line::IrrLine::IrrLineTypeEnum
				enum IrrLine_DE_TypeEnum 
				{
					enUNKNOWN,	// line type is not known yet
					en07,		// MOA line
					en09,		// IMD line
					en12,		// PRI line
					en98,		// UNT
					en99,		// UNZ
					enOTHER		// line type is something else, it is not described in the UEWAUF documentation.
								// This is suspicious to ABBA-UW! Some Error may have occurred.
				};


				// Getter 
				virtual 	LineTypeEnum							getLineTypeEnum()	const;
				domMod::line::IrrLine_DE::IrrLine_DE_TypeEnum		getLineType()		const;
				const basar::I18nString&							getLine()			const;

				void	process(const std::string& lineStr);		// extracts lineStr to members
				bool	isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				// privat member functions
				void					resetMembers();

				// special member functions
				IrrLine_DE( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				IrrLine_DE& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 

				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				domMod::line::IrrLine_DE::IrrLine_DE_TypeEnum	m_LineType;		// Type of Line
				basar::I18nString								m_Line;			// Complete line as string

		};
		
		typedef boost::shared_ptr< IrrLine_DE > IrrLine_DE_Ptr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_IRRELEVANT_DE_LINE_H