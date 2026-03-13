//-------------------------------------------------------------------------------------
/*! \file  unbline.h
*  \brief  subclass of EDIFACTLineBase for UNB-Segment. 
*  \brief  Main purpose: Extract an UNB-Line from a flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author Jia Li ,P. Kessling
*  \date   02.01.2013
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_UNBLINE_H
#define GUARD_DOMMOD_LINE_UNBLINE_H


//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
//#include "editransferorder.h" (leads to circular include)
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
		class UnbLine : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (3 chars long), e.g. "UNB" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data
		
        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				UnbLine();								// private -> public : used in Linetypecollection.h (Frage: Brauchen wir die noch??)
														// it is OK with the clone() according to prototype pattern 	
				~UnbLine();

				// Getter 
				virtual     	LineTypeEnum		getLineTypeEnum()		const;
				const			basar::I18nString&	getSupplierEan()		const;
				const			basar::I18nString&	getWholesalerEan()		const;

				virtual void	process(const std::string& lineStr);							// extracts lineStr to members
				virtual bool	isValidLine()const;												// checks whether line was processed correctly
				virtual bool	isTestOrder()const;

				virtual EDIFACTLineBasePtr	clone() const;	


		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				bool				isUewauf()		const;
				void				resetMembers();

				// special member functions
				UnbLine( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				UnbLine& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 

				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				basar::I18nString       m_SupplierEan;			// EAN number of manufacturer
				basar::I18nString       m_WholesalerEan;		// in CH always = for Amedis
				basar::I18nString       m_FixedUewauf;			// must be "UEWAUF"
				bool					m_TestOrderFlag;		// if set: This order is only for testing reasons
		};

		/////////////////////////////////////////////////////////////////////////
		// private member constants
		/////////////////////////////////////////////////////////////////////////
		static const basar::I18nString PHOENIX_AMEDIS_CH_BFA	= "7601001358034 ";		// wholesaler BFA for AMEDIS CH


		typedef boost::shared_ptr< UnbLine > UnbLinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_UNBLINE_H
