#ifndef GUARD_DOMMOD_EDI_INPUT_ORDERPROTOCOLENUMS_H
#define GUARD_DOMMOD_EDI_INPUT_ORDERPROTOCOLENUMS_H

#include <libbasar_definitions.h>


namespace domMod
{
    namespace libAbbauw
    {
        namespace orderProtocol
        {
            const basar::I18nString CREATE = "C";
            const basar::I18nString UPDATE = "U";
            const basar::I18nString DELETE = "D";

			// Watch out: The same enum definition exists by intention redundantly in cdc_batches_edi_input, cscbatch_history und csc_batches. 
			// Reason: Don't want to include libabbauw in the two processes 
        	enum ProcessedByEnum
        	{
        		ABBA_UW					= 1,
        		CSC_BATCHES_EDI_INPUT	= 2,
        		CSC_BATCHES_HISTORY		= 3
        	};
        
			// Watch out: The same enum definition exists by intention redundantly in cdc_batches_edi_input, cscbatch_history und csc_batches. 
			// Reason: Don't want to include libabbauw in the two processes 
        	enum EventTypeEnum
        	{
        		NO_EVENT_TYPE			= 0,
        		SPLIT_RED_YELLOW		= 1,
        		SPLIT_BTM_K				= 2,
        		RELEASE					= 3,
        		DEFERRED_RELEASE		= 4,
        		GREEN_RELEASE			= 5,
        		GREEN_DEFERRED_RELEASE	= 6,
        		ORDER_REQUEST_CREATED	= 7,
        		ORDER_REQUEST_CANCELED	= 8
        	};
        
        
        } // end namespace orderProtocol
    } // end namespace libabbauw
} // end namespace domMod

#endif // GUARD_DOMMOD_EDI_INPUT_ORDERPROTOCOLENUMS_H
