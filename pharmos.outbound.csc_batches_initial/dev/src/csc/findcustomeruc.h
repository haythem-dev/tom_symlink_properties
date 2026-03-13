#ifndef GUARD_CSCBATCHES_USECASE_FINDCUSTOMERUC_H
#define GUARD_CSCBATCHES_USECASE_FINDCUSTOMERUC_H

#include <libutil/irunnable.h>

#include "icustomerfindervcptr.h"
#include "libabbauw/icustomercollectionfinderptr.h"

#include <libbasarappl_eventmacros.h>
#include <libbasarappl_systemeventmanager.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace appl
    {
        struct EventReturnStruct;
    }
}
    

namespace CSC_Batches
{
namespace useCase {

	//-------------------------------------------------------------------------------------------------//
	// class declaration
	//-------------------------------------------------------------------------------------------------//
	class FindCustomerUC : public libutil::useCase::IRunnable
	{
	public:
		FindCustomerUC();
		~FindCustomerUC();

		SYSTEM_EVENT_HANDLER_DECLARATION( FindCustomerRequested  );
		SYSTEM_EVENT_HANDLER_DECLARATION( NewCustomerRequested   );
        SYSTEM_EVENT_HANDLER_DECLARATION( DialogCloseRequested   );

		void injectVC( viewConn::ICustomerFinderVCPtr );

        void injectDM( libabbauw::domMod::customer::ICustomerCollectionFinderPtr );

        // from IRunnable interface
		void run();
		
	private:
		FindCustomerUC( const FindCustomerUC& orderuc );
		FindCustomerUC& operator= ( const FindCustomerUC& order );
		
		void registerEventHandlers(const bool forSave);
		
    private:
		viewConn::ICustomerFinderVCPtr                                                        m_CustomerFinderVC;
		libabbauw::domMod::customer::ICustomerCollectionFinderPtr                             m_CustomerCollectionFinderDM;
	};
	
}// namespace useCase
}// namespace
#endif // GUARD_CSCBATCHES_USECASE_FINDCUSTOMERUC_H
