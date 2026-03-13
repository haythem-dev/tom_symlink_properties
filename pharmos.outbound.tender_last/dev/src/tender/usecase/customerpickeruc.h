#ifndef GUARD_TENDER_USECASE_CUSTOMERPICKERUC_H
#define GUARD_TENDER_USECASE_CUSTOMERPICKERUC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "customerpickervcptr.h"
#include "ipickeruc.h"
#include "iusecasegetterptr.h"

#include <domainmodule/customer/icustomercollectiondmptr.h>

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
		class CustomerPickerUC : public tender::useCase::IPickerUC
		{
			public:
                CustomerPickerUC();
                virtual ~CustomerPickerUC();

                SYSTEM_EVENT_HANDLER_DECLARATION( SearchCustomerRequested );

				void injectCustomerPickerVC     ( tender::viewConn::CustomerPickerVCPtr                 customerPickerVC     );
				void injectCustomerCollectionDM ( libtender::domMod::customer::ICustomerCollectionDMPtr customerCollectionDM );
                void injectUseCaseGetter        ( tender::componentManager::IUseCaseGetterPtr           useCaseGetter        );

				virtual void setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit );

                virtual void                                               run();
			    virtual basar::db::aspect::AccessorPropertyTable_YIterator getPicked();
				virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYIterator();
				virtual void setCheckMode(bool);

			private:
				CustomerPickerUC(const CustomerPickerUC& );
				CustomerPickerUC& operator= ( const CustomerPickerUC& );

				void init();
				void reset();
				void registerEventHandlers( const bool forSave );

				const log4cplus::Logger&                              getLogger() const;
				tender::viewConn::CustomerPickerVCPtr                 getCustomerPickerVC();
				libtender::domMod::customer::ICustomerCollectionDMPtr getCustomerCollectionDM();
				tender::componentManager::IUseCaseGetterPtr           getUseCaseGetter();

				basar::appl::EventReturnType searchCustomer();
				basar::appl::EventReturnType checkCustomer();

				//////////////////////////////////////////////////////////////////////////////////////
                // member variables
                //////////////////////////////////////////////////////////////////////////////////////
				tender::viewConn::CustomerPickerVCPtr                 m_CustomerPickerVC;
				libtender::domMod::customer::ICustomerCollectionDMPtr m_Customer;
				tender::componentManager::IUseCaseGetterPtr           m_UseCaseGetter;

				const log4cplus::Logger	                              m_Logger;
				bool												  m_CheckMode;
		};

	} // end namespace useCase
} // end namespace tender

#endif // end GUARD_TENDER_USECASE_CUSTOMERPICKERUC_H
