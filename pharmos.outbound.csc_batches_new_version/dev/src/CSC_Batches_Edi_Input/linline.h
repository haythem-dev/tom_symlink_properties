//-------------------------------------------------------------------------------------
/*! \file  linline.h
*  \brief  subclass of EDIFACTLineBase for LIN-Segment. 
*  \brief  Main purpose: Extract an LIN-Line from a flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                All types of LIN-Lines can be processed within this method
*  \author Jia Li, P. Kessling
*  \date   02.01.2013, 
*  history 26.03.13  pk redesign
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_LINLINE_H
#define GUARD_DOMMOD_LINE_LINLINE_H

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
		class LinLine : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (3 chars long), here "LIN" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				LinLine();								// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~LinLine();

				// enums
				// Enumeration for Articleno code with respect to LIN-Lines
				// Usage: domMod::line::LinLine::enOTHER
				enum LinCodeEnum 
				{
					enUNKNOWN,	// code is not known yet
					enEanCode,	// Code for articleno is EAN
					enMfCode,	// Code for articleno is defined by manufacturer
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
								bool				isManufacturerDefined()	const;
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
				LinLine( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				LinLine& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 
	
				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				basar::Int32       m_Positionno;	// Position number defined by manufacturer
				basar::I18nString  m_Articleno;		// Article number, interpretation resp. m_ArticlenoCode
				LinCodeEnum m_ArticlenoCode;		// Code of Articlenumber, can be manufacturer-defined (ARTNRRC=="MF" orEAN (ARTNRRC=empty or "EN")


		};
		
		typedef boost::shared_ptr< LinLine > LinLinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_LINLINE_H


