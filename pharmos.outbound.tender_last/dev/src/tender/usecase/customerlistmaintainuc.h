#ifndef GUARD_TENDER_USECASE_CUSTOMERLISTMAINTAINUC_H
#define GUARD_TENDER_USECASE_CUSTOMERLISTMAINTAINUC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "customerlistmaintainvcptr.h"
#include "iusecasegetterptr.h"

#include <domainmodule/customerlist/icustomerlistdmptr.h>

// libs
#include <libutil/irunnable.h>
#include <libutil/session.h>

#include <libbasarappl_eventmacros.h>
#include <libbasarguitie.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace tender
{
	namespace useCase
	{
		//-------------------------------------------------------------------------------------------------//
        // class declaration
        //-------------------------------------------------------------------------------------------------//
		class CustomerListMaintainUC : public libutil::useCase::IRunnable
		{
			public:
                CustomerListMaintainUC();
                virtual ~CustomerListMaintainUC();

				SYSTEM_EVENT_HANDLER_DECLARATION( AddCustomerRequested    );
				SYSTEM_EVENT_HANDLER_DECLARATION( EditCustomerRequested   );
				SYSTEM_EVENT_HANDLER_DECLARATION( DeleteCustomerRequested );
                SYSTEM_EVENT_HANDLER_DECLARATION( SearchCustomerRequested );
				SYSTEM_EVENT_HANDLER_DECLARATION( CheckCustomerRequested  );

				void injectCustomerListMaintainVC( tender::viewConn::CustomerListMaintainVCPtr customerPickerVC );
				void injectCustomerListDM( libtender::domMod::customerlist::ICustomerListDMPtr customerListDM );
                void injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter );

				void setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit );

				virtual void run();

			private:
				CustomerListMaintainUC(const CustomerListMaintainUC& );
				CustomerListMaintainUC& operator= ( const CustomerListMaintainUC& );

				void init();
				void reset();
				void registerEventHandlers( const bool forSave );

				basar::appl::EventReturnType addCustomer( basar::db::aspect::AccessorPropertyTable_YIterator );

				const log4cplus::Logger&							getLogger() const;
				tender::viewConn::CustomerListMaintainVCPtr			getCustomerListMaintainVC();
				libtender::domMod::customerlist::ICustomerListDMPtr	getCustomerListDM();
				tender::componentManager::IUseCaseGetterPtr			getUseCaseGetter();

				//////////////////////////////////////////////////////////////////////////////////////
                // member variables
                //////////////////////////////////////////////////////////////////////////////////////
				tender::viewConn::CustomerListMaintainVCPtr			m_CustomerListMaintainVC;
				libtender::domMod::customerlist::ICustomerListDMPtr	m_CustomerList;
				tender::componentManager::IUseCaseGetterPtr			m_UseCaseGetter;

				const log4cplus::Logger								m_Logger;
		};

	} // end namespace useCase
} // end namespace tender

#endif // end GUARD_TENDER_USECASE_CUSTOMERLISTMAINTAINUC_H
