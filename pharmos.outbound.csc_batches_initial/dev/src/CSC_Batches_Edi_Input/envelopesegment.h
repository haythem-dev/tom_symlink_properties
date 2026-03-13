// Nicht mehr benötigt!!


//-------------------------------------------------------------------------------------
/*! \file  payloadheadersegment.h
*  \brief  class for the envelope (UNB line) in a transferorder. 
*  \author Jia Li, P. Kessling
*  \date   02.01.2013 
*  history: 09.04.13 pk rework
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_TRANSFERORDERDM_ENVELOPESEGMENT_H
#define GUARD_DOMMOD_TRANSFERORDERDM_ENVELOPESEGMENT_H

#include "edifactline.h"
#include "unbline.h"


namespace domMod
{
	namespace transferOrderDM
	{
		class EnvelopeSegment 
		{
			public:

				EnvelopeSegment();
				~EnvelopeSegment();

				bool empty();										// clear all stored lines
				bool mountLine(domMod::line::EDIFACTLineBasePtr);	// param linePtr [transfer ownership]
																	// put this line to the correct place in envelope-segment
				bool isValid();										// is Info complete and ok for storage?


			private:
				EnvelopeSegment( const EnvelopeSegment& );				
				EnvelopeSegment& operator=( const EnvelopeSegment& );
		        
				domMod::line::UnbLinePtr m_UnbLine;
		};

	    typedef boost::shared_ptr< EnvelopeSegment > EnvelopeSegmentPtr;

	} // namespace transferOrderDM
} // namespace domMod

#endif // GUARD_DOMMOD_TRANSFERORDERDM_ENVELOPESEGMENT_H
