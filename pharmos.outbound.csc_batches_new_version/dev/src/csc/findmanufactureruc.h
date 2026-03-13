#ifndef GUARD_CSCBATCHES_USECASE_FINDMANUFACTURERUC_H
#define GUARD_CSCBATCHES_USECASE_FINDMANUFACTURERUC_H

#include <libutil/irunnable.h>

#include <libbasarappl_eventmacros.h>
#include "libbasarappl_systemeventmanager.h"

#include "imanufacturerfindervcptr.h"
#include "libabbauw/ifindmanufacturerptr.h"

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
	class FindManufacturerUC : public libutil::useCase::IRunnable
	{
	public:
		FindManufacturerUC();
		~FindManufacturerUC();

		SYSTEM_EVENT_HANDLER_DECLARATION( FindManufacturerRequested );
		SYSTEM_EVENT_HANDLER_DECLARATION( NewManufacturerRequested  );
        SYSTEM_EVENT_HANDLER_DECLARATION( DialogCloseRequested  );
		
		void injectVC( viewConn::IManufacturerFinderVCPtr );
		void injectDM( libabbauw::domMod::manufacturerdm::IFindManufacturerPtr );

        // from IRunnable interface
		void run();
		
	private:
		FindManufacturerUC(const FindManufacturerUC& orderuc);
		FindManufacturerUC& operator= (const FindManufacturerUC& order);
		
		void registerEventHandlers(const bool forSave);

    private:
		viewConn::IManufacturerFinderVCPtr                       m_ManufacturerFinderVC;
		libabbauw::domMod::manufacturerdm::IFindManufacturerPtr  m_FindManufacturerDM;
	};
	
}// namespace useCase
}// namespace
#endif // GUARD_CSCBATCHES_USECASE_FINDMANUFACTURERUC_H
