
// Nicht mehr benötigt!!


//-------------------------------------------------------------------------------------------------//
/*! \file	envelopesegment.cpp
 *  \brief  envelopesegment
 *  \author Jia Li, P. Kessling
 *  \date   02.01.2013 
 *  history: 09.04.13 pk rework
*/
//-------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "envelopesegment.h"
#include "unbline.h"

namespace domMod
{
	namespace transferOrderDM
	{
		// clears all stored lines
		bool EnvelopeSegment::empty()
		{
			// ToDo kes: implementieren!!
			return true;
		}

		// put this line to the correct place in transferOrder-segments
		// param linePtr [transfer ownership]
		bool EnvelopeSegment::mountLine(domMod::line::EDIFACTLineBasePtr /*linePtr*/)
		{
			// ToDo kes: implementieren!!
			return true;
		}

		// is Info complete and ok for storage?
		bool EnvelopeSegment::isValid()
		{
			// ToDo kes: implementieren!!
			return true;
		}

	} // namespace transferOrderDM
} // namespace domMod
