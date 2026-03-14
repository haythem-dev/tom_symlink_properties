//-------------------------------------------------------------------------------------
/*! \file  dtmline.h
*  \brief  subclass of EDIFACTLineBase for German 03-Segment (DTM).  
*  \brief  Main purpose: Extract an DTM-Line from a GERMAN .inh-flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  history 28.10.14  pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_DTM_03_LINE_H
#define GUARD_DOMMOD_LINE_DTM_03_LINE_H

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
		class Dtm_03_Line : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (2 chars long), here "DTM" 
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data
		
        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				Dtm_03_Line();							// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~Dtm_03_Line();

				// Getter 
				virtual			LineTypeEnum	getLineTypeEnum()		const;
				const			basar::Date&	getDate()				const;	// Date in format YYYYMMDD

				bool			isOrderDate()			const;	// Datcode=137, orderdate          , VX27:AUFDAT, date of reading from EDIBox
				bool			isCustomerOrderDate()	const;	// Datcode=4  , --not used for ABBA-UW ---, Datum der Bestellung beim Kunden
				bool			isEdiDeliveryDate()		const;	// Datcode=2  , edideliverydate    , VX27:TERM, gewuenschter Liefertermin
				bool			isDeferredPaymentDate()	const;	// Datcode=209, deferredpaymentdate, VX27:Valuta, Zahlungsziel
				bool			is3MonthDeferredPaymentDate()	const;	// Datcode=ZZZ, deferredpaymentdate in three month, VX27:Valuta, Zahlungsziel ist heute in drei Monaten
				bool			isToleratedDatcodeDate()	const;	// Datcode=117 oder 128, ignore date, log as "unknown Datcode", process continues

				void		process(const std::string& lineStr);	// extracts lineStr to members
				bool		isValidLine()const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				void					resetMembers();

				// special member functions
				Dtm_03_Line( const EDIFACTLineBase& ) ;					// Copy-Konstruktor : abschalten  

				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				// private variables
				basar::I18nString       m_Datcode;					// must be "137", "2  " oder "209" oder "ZZZ"
				basar::cmnutil::Date    m_Datum;					// can contain Auftragsdatum (137), 
																	//             Lieferdatum (2) or Valutadatum (209 or ZZZ)
				basar::I18nString       m_Datform;					// can contain "102": Then format of m_Datum = YYYYMMDD 
																	//             YYYYMMDD is also default format: 
																	//             Therefore m_Datform is irrelevant

		};
		
		typedef boost::shared_ptr< domMod::line::Dtm_03_Line > Dtm_03_LinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_DTM_03_LINE_H

