


//-------------------------------------------------------------------------------------
/*! \file  headersegment.h
 *  \brief  class for the payload in header-segment. 
 *  \author Jia Li, P. Kessling
 *  \date   02.01.2013 
 *  history: 09.04.13 pk rework
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_TRANSFERORDERDM_HEADERSEGMENT_H
#define GUARD_DOMMOD_TRANSFERORDERDM_HEADERSEGMENT_H

#include "unhline.h"
#include "bgmline.h"
#include "dtmline.h"
#include "nadline.h"
#include "unsline.h"

namespace domMod
{
	namespace transferOrderDM
	{
		class HeaderSegment 
		{
			// ToDo kes: Vorschlag: 
			// - Konstruktor, 
			// - isValid()-Methode, 
			// - insertLine(), 
			// - feste Pointer für notwendige Zeilen, 
			// - Pointervector für zusätzliche Zeilen

			public:
				HeaderSegment();
				~HeaderSegment();

				bool empty();										// clear all stored lines
				bool mountLine(domMod::line::EDIFACTLineBasePtr);	// param linePtr [transfer ownership]
																	// put this line to the correct place in envelope-segment
				bool isValid();										// is Info complete and ok for storage?

			private:
				HeaderSegment( const HeaderSegment& );
				HeaderSegment& operator=( const HeaderSegment& );

				void resetMembers();									// resets everything in class
		        
				domMod::line::UnhLinePtr m_UnhLine;
				domMod::line::BgmLinePtr m_BgmLine;
				domMod::line::DtmLinePtr m_DtmLine_OrderDate;			// DTM 137 	
				domMod::line::DtmLinePtr m_DtmLine_EdiDeliveryDate;		// DTM 2
				domMod::line::DtmLinePtr m_DtmLine_DeferredPaymentDate;	// DTM 209	(valutadate, Zahlungsziel)
				//domMod::line::DtmLinePtr m_DtmLine_4;					// DTM 4		// info of this field is not utilized	: ToDo kes: Weg?
				domMod::line::NadLinePtr m_NadLine_Manufacturer;		// NAD MF		
				domMod::line::NadLinePtr m_NadLine_Branch;				// NAD DS (wholesale, deliveryservice)		
				domMod::line::NadLinePtr m_NadLine_Pharmacy;			// NAD BY (pharmacy)		

				domMod::line::UnhLinePtr m_UnsLine_D;	// the UnsLines embrace the Positionssegment ..
				domMod::line::UnhLinePtr m_UnsLine_S;	// .. logically it should be a segment of its own, but we want to avoid overkill

		};

	    typedef boost::shared_ptr< HeaderSegment > HeaderSegmentPtr;

	} // namespace transferOrderDM
} // namespace domMod

#endif // GUARD_DOMMOD_TRANSFERORDERDM_PAYLOADHEADERSEGMENT_H
