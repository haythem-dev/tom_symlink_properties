//-------------------------------------------------------------------------------------
/*! \file  nadline.h
*  \brief  subclass of EDIFACTLineBase for NAD-Segment. 
*  \brief  Main purpose: Extract an NAD-Line from a flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                All types of NAD-Lines can be processed within this method
*  \author Jia Li, P. Kessling
*  \date   02.01.2013, 
*  history 26.03.13  pk redesign
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_NADLINE_H
#define GUARD_DOMMOD_LINE_NADLINE_H
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
		class NadLine : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (3 chars long), here "NAD" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				NadLine();								// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~NadLine();

				// enums
				// Enumeration for Roles of participants with respect to NAD-Lines
				// Usage e.g.: domMod::line::NadLine::NadRoleEnum
				enum NadRoleEnum 
				{
					enUNKNOWN,	// role is not known yet
					enMF,		// manufacturer
					enDS,		// deliverer
					enBY,		// buyer (pharmacy)
					enOTHER		// role is something else, but not relevant to ABBA-UW
				};


				// Getter 
				virtual 	LineTypeEnum					getLineTypeEnum()		const;
				const domMod::line::NadLine::NadRoleEnum&	getRole()		const;
				const basar::I18nString&					getIdentEan()	const;
				const basar::I18nString&					getText()		const;

				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool		isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				// privat member functions
				bool				isCodepflOK()	const;
				void				resetMembers();

				// special member functions
				NadLine( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				NadLine& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 
		
				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				domMod::line::NadLine::NadRoleEnum	m_Role;					// Type of NADLine= Role of particpant: MF, DS, BY or something else
				basar::I18nString       m_IdentEan;				// EAN number of manufacturer, wholesaler branch or pharmacy according to "role"
				basar::I18nString       m_FixedCodepfl;			// must/should be "104" regarding documentation, but it was never observed in real life examples
				basar::I18nString       m_Text;					// EAN number of manufacturer, wholesaler branch or pharmacy according to "role"


		};
		
		typedef boost::shared_ptr< NadLine > NadLinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_NADLINE_H