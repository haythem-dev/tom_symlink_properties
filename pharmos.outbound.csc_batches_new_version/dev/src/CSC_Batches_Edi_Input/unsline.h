//-------------------------------------------------------------------------------------
/*! \file  unsline.h
*  \brief  subclass of EDIFACTLineBase for UNS-Segment. 
*  \brief  Main purpose: Extract an UNS-Line from a flatfile-line
*  \brief                into an internal data structure and represent it 
*  \brief                Both types of UNS-Lines can be processed within this method
*  \author P. Kessling
*  \date   26.03.2013, 
*  history 26.03.13  pk  new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_UNSLINE_H
#define GUARD_DOMMOD_LINE_UNSLINE_H
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
		class UnsLine : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (3 chars long), here "UNS" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data
		
        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				UnsLine();								// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	

				// Getter
				virtual			LineTypeEnum		getLineTypeEnum()					const;
				bool								isBeginOfPositionSection()			const;

				void								process(const std::string& lineStr);		// extracts lineStr to members
				bool								isValidLine()						const;

				virtual EDIFACTLineBasePtr			clone()								const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				// privat member functions
				void					resetMembers();

				// special member functions: the standard ones are ok but necessary
				//~UnsLine();
				//UnsLine( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				//UnsLine& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 

				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				basar::I18nString       m_Qualifier;			// Type of UNSLine: Must be "D" (begin of Position-section) or "S" (begin or end of position-section) 


		};
		
		typedef boost::shared_ptr< UnsLine > UnsLinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_UNSLINE_H