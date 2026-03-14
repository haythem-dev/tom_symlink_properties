//-------------------------------------------------------------------------------------
/*! \file  bgm_02_line.h
*  \brief  subclass of EDIFACTLineBase for German 02-Segment (BGM).
*  \brief  Main purpose: Extract an BGM-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 27.10.14  pk neu
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_BGM_02_LINE_H
#define GUARD_DOMMOD_LINE_BGM_02_LINE_H

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
		class Bgm_02_Line : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (2 chars long), here "02" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data
		
        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				Bgm_02_Line();							// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~Bgm_02_Line();

				// Getter 
				virtual			LineTypeEnum		getLineTypeEnum()	const;
				const			basar::I18nString&	getOrdernoMF()		const;

				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool		isValidLine()const;

				bool		isDokcode640()		const;
				bool		isDokcode130()		const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				void				resetMembers();

				// special member functions
				Bgm_02_Line( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				Bgm_02_Line& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 

				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				basar::I18nString       m_FixedDokCode;			// must be "640"
				basar::I18nString       m_OrdernoMF;			// can contain Ordernumber defined by manufacturer


		};
		
		typedef boost::shared_ptr< Bgm_02_Line > Bgm_02_LinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_BGM_02_LINE_H



