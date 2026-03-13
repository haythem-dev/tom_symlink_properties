#ifndef GUARD_TENDER_USECASE_PARTNERPICKERUC_H
#define GUARD_TENDER_USECASE_PARTNERPICKERUC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "partnerpickervcptr.h"
#include "ipickeruc.h"
#include "iusecasegetterptr.h"

#include <domainmodule/partner/ipartnercollectiondmptr.h>

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
		class PartnerPickerUC : public tender::useCase::IPickerUC
		{
			public:
                PartnerPickerUC();
                virtual ~PartnerPickerUC();

                SYSTEM_EVENT_HANDLER_DECLARATION( SearchPartnerRequested );

				void injectPartnerPickerVC      ( tender::viewConn::PartnerPickerVCPtr                partnerPickerVC     );
				void injectPartnerCollectionDM  ( libtender::domMod::partner::IPartnerCollectionDMPtr partnerCollectionDM );
                void injectUseCaseGetter        ( tender::componentManager::IUseCaseGetterPtr         useCaseGetter       );

				virtual void setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit );

                virtual void                                               run();
			    virtual basar::db::aspect::AccessorPropertyTable_YIterator getPicked();
				virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYIterator();
				virtual void setCheckMode( bool );
				
			private:
				PartnerPickerUC(const PartnerPickerUC& );
				PartnerPickerUC& operator= ( const PartnerPickerUC& );

				void init();
				void reset();
				void registerEventHandlers( const bool forSave );

				const log4cplus::Logger&                              getLogger() const;
				tender::viewConn::PartnerPickerVCPtr                  getPartnerPickerVC();
				libtender::domMod::partner::IPartnerCollectionDMPtr   getPartnerCollectionDM();
				tender::componentManager::IUseCaseGetterPtr           getUseCaseGetter();

				basar::appl::EventReturnType                          searchPartner();
				basar::appl::EventReturnType						  checkPartner();

				//////////////////////////////////////////////////////////////////////////////////////
                // member variables
                //////////////////////////////////////////////////////////////////////////////////////
				tender::viewConn::PartnerPickerVCPtr                  m_PartnerPickerVC;
				libtender::domMod::partner::IPartnerCollectionDMPtr   m_Partner;
				tender::componentManager::IUseCaseGetterPtr           m_UseCaseGetter;

				const log4cplus::Logger	                              m_Logger;
				bool												  m_CheckMode;
		};

	} // end namespace useCase
} // end namespace tender

#endif // end GUARD_TENDER_USECASE_PARTNERPICKERUC_H
