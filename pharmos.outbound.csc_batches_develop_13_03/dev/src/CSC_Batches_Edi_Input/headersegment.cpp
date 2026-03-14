


//-------------------------------------------------------------------------------------------------//
/*! \file	headersegment.cpp
 *  \brief      headersegment
 *  \author Jia Li, P. Kessling
 *  \date   02.01.2013 
 *  history: 09.04.13 pk rework
*/
//-------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "headersegment.h"

#include "unhline.h"
#include "bgmline.h"
#include "nadline.h"

namespace domMod
{
	namespace transferOrderDM
	{

		// resets everything in class
		void HeaderSegment::resetMembers()
		{
			m_UnhLine.reset();
			m_BgmLine.reset();
			m_DtmLine_OrderDate.reset();			
			m_DtmLine_EdiDeliveryDate.reset();		
			m_DtmLine_DeferredPaymentDate.reset();	
			//m_DtmLine_4.reset(0);					
			m_NadLine_Manufacturer.reset();		
			m_NadLine_Branch.reset();				
			m_NadLine_Pharmacy.reset();			

			m_UnsLine_D.reset();	
			m_UnsLine_S.reset();	

		}

		// clears all stored lines
		bool HeaderSegment::empty()
		{
			resetMembers();
			return true;
		}

		// put this line to the correct place in transferOrder-segments
		// param linePtr [transfer ownership]
		bool HeaderSegment::mountLine(domMod::line::EDIFACTLineBasePtr /*linePtr*/)
		{
			// ToDo kes: implementieren!!
			return true;
		}

		// is Info complete and ok for storage?
		bool HeaderSegment::isValid()
		{
			// ToDo kes: implementieren!!
			return true;
		}

	} // namespace transferOrderDM
} // namespace domMod
