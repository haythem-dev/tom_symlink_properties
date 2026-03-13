//-------------------------------------------------------------------------------------
/*! \file  dtmline.h
*  \brief  subclass of EDIFACTLineBase for DTM-Segment. 
*  \brief  Main purpose: Extract an DTM-Line from a flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author Jia Li, P. Kessling
*  \date   02.01.2013, 
*  history 22.03.13  pk redesign
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_DTMLINE_H
#define GUARD_DOMMOD_LINE_DTMLINE_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "edifactline.h"
#include <libbasarcmnutil_date.h>

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
		class DtmLine : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (3 chars long), here "DTM" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data
		
        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				DtmLine();								// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~DtmLine();

				// Getter 
				virtual			LineTypeEnum	getLineTypeEnum()		const;
				const			basar::Date&	getDate()				const;	// Date in format YYYYMMDD

				bool			isOrderDate()			const;	// Datcode=137, orderdate          , VX27:AUFDAT, date of reading from EDIBox
				bool			isCustomerOrderDate()	const;	// Datcode=4  , --not used for ABBA-UW ---, Datum der Bestellung beim Kunden
				bool			isEdiDeliveryDate()		const;	// Datcode=2  , edideliverydate    , VX27:TERM, gewuenschter Liefertermin
				bool			isDeferredPaymentDate()	const;	// Datcode=209, deferredpaymentdate, VX27:Valuta, Zahlungsziel

				void		process(const std::string& lineStr);	// extracts lineStr to members
				bool	isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				void					resetMembers();

				// special member functions
				DtmLine( const EDIFACTLineBase& ) ;					// Copy-Konstruktor : abschalten  

				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				// private variables
				basar::I18nString       m_Datcode;					// must be "137", "4  ", "2  " oder "209"
				basar::cmnutil::Date    m_Datum;					// can contain Auftragsdatum (137), Bestelldatum (4), 
																	//             Lieferdatum (2) or Valutadatum (209)
				basar::I18nString       m_Datform;					// can contain "102": Then format of m_Datum = YYYYMMDD 
																	//             YYYYMMDD is also default format: 
																	//             Therefore m_Datform is irrelevant

		};
		
		typedef boost::shared_ptr< domMod::line::DtmLine > DtmLinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_DTMLINE_H

