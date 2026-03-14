//-------------------------------------------------------------------------------------
/*! \file  unbline.h
*  \brief  subclass of EDIFACTLineBase for German 00-Segment (UNB). 
*  \brief  Main purpose: Extract an IMD-Line rom a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 29.10.14  pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_UNB_00_LINE_H
#define GUARD_DOMMOD_LINE_UNB_00_LINE_H


//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
//#include "editransferorder.h" (leads to circular include)
#include "edifactline.h"
#include <list>

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
		class Unb_00_Line : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (2 chars long), e.g. "00" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data
		
        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				Unb_00_Line();								// private -> public : used in Linetypecollection.h (Frage: Brauchen wir die noch??)
														// it is OK with the clone() according to prototype pattern 	
				~Unb_00_Line();

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
				bool				isSyntaxOk()		const;
				bool				isWholesaleOk()		const;
				bool				isFromPharmaMall()	const;
				void				resetMembers();

				// special member functions
				Unb_00_Line( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				Unb_00_Line& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 

				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				basar::I18nString       m_FixedSyntax;			// must be "UNOB2"
				basar::I18nString       m_SupplierBfa;			// BFA number of manufacturer
				basar::I18nString       m_WholesalerBfa;		// in CH always = for Amedis
				basar::I18nString       m_CreationDate;			// Date of creation of the order. Format: YYMMDD
				basar::I18nString       m_CreationTime;			// Date of creation of the order. Format: HHMM
																// TODO: Do not use date and time??
				bool					m_TestOrderFlag;		// if set: This order is only for testing reasons



		};

		/////////////////////////////////////////////////////////////////////////
		// private member constants
		/////////////////////////////////////////////////////////////////////////
		static const basar::I18nString PHOENIX_DE_BFA			= "76539419";			// wholesaler BFA for PHOENIX DE and HAGEDA
																						//		= address of PROD mailbox in OpenText and Pironet
		static const basar::I18nString PHOENIX_DE_QASMAILBOX	= "PHOENIXQ";			// address of QAS mailbox in OpenText/GXS VAN, only utilized for testing the connection of new suppliers
																						// (full name is PHOENIXQAS, but the UNB-EMPFBEZ field comprises only 8 char, afterwards filler)
																						// This QAS mailbox address is written into the testmessages UNB-EMPFBEZ segment for UEWAUFs coming via OpenText VAN provider.

		
		static std::list<basar::I18nString> CMI_BFA_LIST;								// Astellas = 87525074; MSD = 87741032; (via PharmaMall = ordertype DG) 
		static basar::I18nString BFA_ASTELLAS = "87525074";
		static basar::I18nString BFA_MSD = "87741032";


		typedef boost::shared_ptr< Unb_00_Line > Unb_00_LinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_UNB_00_LINE_H
