#ifndef GUARD_LIBABBAUW_PROPERTIES_ORDERPROTOCOLENUMS_H
#define GUARD_LIBABBAUW_PROPERTIES_ORDERPROTOCOLENUMS_H

#include <libbasardbaspect.h>


namespace libabbauw
{
    namespace domMod
    {
        namespace orderProtocol
        {
            const basar::I18nString CREATE = "C";
            const basar::I18nString UPDATE = "U";
            const basar::I18nString DELET = "D";

            const basar::I18nString ORDERVIEW   = "O";
            const basar::I18nString RELEASEVIEW = "R";

			// Watch out: The same enum definition exists redundantly in cdc_batches_edi_input, cscbatch_history und csc_batches. 
			// Reason: Don't want to include libabbauw in the two processes 
        	enum ProcessedByEnum
        	{
        		ABBA_UW					= 1,
        		CSC_BATCHES_EDI_INPUT	= 2,
        		CSC_BATCHES_HISTORY		= 3
        	};
        
			// Watch out: The same enum definition exists redundantly in cdc_batches_edi_input, cscbatch_history und csc_batches. 
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
        		ORDER_REQUEST_CANCELED	= 8,
				CREATE_TENDER			= 9,
				CANCEL_ORDER			= 10
        	};
        
        } // end namespace orderProtocol
    } // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_PROPERTIES_ORDERPROTOCOLENUMS_H
