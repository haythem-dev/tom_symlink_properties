//-------------------------------------------------------------------------------------
/*! \file  nad_04_line.h
*  \brief  subclass of EDIFACTLineBase for German 04-Segment (NAD). 
*  \brief  Main purpose: Extract an NAD-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                All types of NAD-Lines can be processed within this method
*  \author P. Kessling
*  history 28.10.14  pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_NAD_04_LINE_H
#define GUARD_DOMMOD_LINE_NAD_04_LINE_H
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
		class Nad_04_Line : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (2 chars long), here "04" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				Nad_04_Line();								// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~Nad_04_Line();

				// enums
				// Enumeration for Roles of participants with respect to NAD-Lines
				// Usage e.g.: domMod::line::NadLine::NadRoleEnum
				enum NadRoleEnum 
				{
					enUNKNOWN,	// role is not known yet
					enMF,		// manufacturer
					enDS,		// deliverer
					enBY,		// buyer (pharmacy)
					enCN,		// Receiver of goods (good are to be delivered to CN, CN has preference before BY)
					enOTHER		// role is something else, but not relevant to ABBA-UW
				};
				// Enumeration of code type for the NAD-IDENT field
				enum NadCodelistEnum 
				{
					enUNKNOWN_CODELIST,	// role is not known yet
					enBGA,		// BGA number
					enBFA,		// BFA number
					enOTHER_CODELIST	// code type is something else, but not interpretable by ABBA-UW
				};


				// Getter 
				virtual 	LineTypeEnum							getLineTypeEnum()	const;
				const domMod::line::Nad_04_Line::NadRoleEnum&		getRole()			const;
				const basar::I18nString&							getIdent()			const;
				const domMod::line::Nad_04_Line::NadCodelistEnum&	getCodelist()		const;
				const basar::I18nString&							getNameText()		const;
				const basar::I18nString&							getOrtText()		const;

				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool		isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				// privat member functions
				bool				isCodelistOK()	const;
				void				resetMembers();

				// special member functions
				Nad_04_Line( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				Nad_04_Line& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 
		
				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				domMod::line::Nad_04_Line::NadRoleEnum	m_Role;					// Type of NADLine= Role of particpant: MF, DS, BY or something else
				basar::I18nString						m_Ident;				// BFA / BGA number of manufacturer, wholesaler branch or pharmacy according to "role"
																				// In DE it is BFA or BGA, in CH it is EAN
				domMod::line::Nad_04_Line::NadCodelistEnum m_Codelist;			// the codetype utilized in field m_Ident (BFA or BGA)
				basar::I18nString						m_NameText;				// Optional Name of manufacturer, wholesaler branch or pharmacy according to "role"
				basar::I18nString						m_OrtText;				// Optional city etc. of manufacturer, wholesaler branch or pharmacy according to "role"


		};
		
		typedef boost::shared_ptr< Nad_04_Line > Nad_04_LinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_NAD_04_LINE_H