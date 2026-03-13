//-------------------------------------------------------------------------------------
/*! \file  lin_08_line.h
*  \brief  subclass of EDIFACTLineBase for German 08-Segment (LIN). 
*  \brief  Main purpose: Extract an DTM-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 28.10.14  pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_LIN_08_LINE_H
#define GUARD_DOMMOD_LINE_LIN_08_LINE_H

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
		class Lin_08_Line : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (2 chars long), here "LIN" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				Lin_08_Line();							// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~Lin_08_Line();

				// enums
				// Enumeration for Articleno code with respect to LIN-Lines
				// Usage: domMod::line::LinLine::enOTHER
				enum LinCodeEnum 
				{
					enUNKNOWN,	// code is not known yet
					enEanCode,	// Code for articleno is EAN
					enPznCode,	// Code for articleno is PZN
					enOTHER		// Code is something else, but not interpretable for ABBA-UW
				};

				// Getter 
				virtual			LineTypeEnum		getLineTypeEnum()	const;
								basar::Int32		getPositionno()		const;
				const			basar::I18nString&	getArticleno()		const;
								LinCodeEnum			getArticlenoCode()	const;

				// Checker
								bool				isEan()					const;
								bool				isPzn()					const;

				void		process(const std::string& lineStr);		// extracts lineStr to members
					 bool	isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				// privat member functions
				bool					isArticlenoCodeOK()	const;
				void					resetMembers();

				// special member functions
				Lin_08_Line( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				Lin_08_Line& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 
	
				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				basar::Int32       m_Positionno;	// Position number defined by manufacturer
				basar::I18nString  m_Articleno;		// Article number, interpretation resp. m_ArticlenoCode
				LinCodeEnum		   m_ArticlenoCode;	// Code of Articlenumber, can be "P  " (PZN) or "EAN" (EAN)


		};
		
		typedef boost::shared_ptr< Lin_08_Line > Lin_08_LinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_LIN_08_LINE_H


