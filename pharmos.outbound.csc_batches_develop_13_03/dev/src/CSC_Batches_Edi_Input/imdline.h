//-------------------------------------------------------------------------------------
/*! \file  imdline.h
*  \brief  subclass of EDIFACTLineBase for IMD-Segment. 
*  \brief  Main purpose: Extract an IMD-Line from a flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  \date   27.03.2013, 
*  history 27.03.13  pk new
*/
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_DOMMOD_LINE_IMDLINE_H
#define GUARD_DOMMOD_LINE_IMDLINE_H

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
		class ImdLine : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (3 chars long), here "BGM" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				ImdLine();								// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~ImdLine();

				// Getter 
				virtual			LineTypeEnum		getLineTypeEnum()		const;
				const			basar::I18nString&	getArticleDescMF()		const;
				const			basar::I18nString&	getUnitPharmaFormMF()	const;

				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool		isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				bool				isFixedCommodityDescOK() const;
				void				resetMembers();

				// special member functions
				ImdLine( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				ImdLine& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 
		
				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				basar::I18nString       m_FixedCommodityDesc;		// must be "PHARMA"
				basar::I18nString       m_ArticleDescMF;			// can contain name of article defined by manufacturer
				basar::I18nString       m_UnitPharmaFormMF;			// can contain unit or pharmaceutical form defined by manufacturer


		};
		
		typedef boost::shared_ptr< ImdLine > ImdLinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_IMDLINE_H



