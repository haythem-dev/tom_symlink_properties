//-------------------------------------------------------------------------------------
/*! \file  bgmline.h
*  \brief  subclass of EDIFACTLineBase for BGM-Segment. 
*  \brief  Main purpose: Extract an BGM-Line from a flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author Jia Li ,P. Kessling
*  \date   02.01.2013, 
*  history 22.03.13  pk redesign
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_BGMLINE_H
#define GUARD_DOMMOD_LINE_BGMLINE_H

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
		class BgmLine : public EDIFACTLineBase 
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
				BgmLine();								// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~BgmLine();

				// Getter 
				virtual			LineTypeEnum		getLineTypeEnum()	const;
				const			basar::I18nString&	getOrdernoMF()		const;

				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool		isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				bool				isDokcode640()		const;
				void				resetMembers();

				// special member functions
				BgmLine( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				BgmLine& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 

				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				basar::I18nString       m_FixedDokCode;			// must be "640"
				basar::I18nString       m_OrdernoMF;			// can contain Ordernumber defined by manufacturer


		};
		
		typedef boost::shared_ptr< BgmLine > BgmLinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_BGMLINE_H



