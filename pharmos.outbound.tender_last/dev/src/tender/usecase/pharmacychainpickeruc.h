#ifndef GUARD_TENDER_USECASE_PHARMACYCHAINPICKERUC_H
#define GUARD_TENDER_USECASE_PHARMACYCHAINPICKERUC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "pharmacychainpickervcptr.h"
#include "ipickeruc.h"
#include "iusecasegetterptr.h"

#include <domainmodule/pharmacychain/ipharmacychaincollectiondmptr.h>

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
		class PharmacyChainPickerUC : public tender::useCase::IPickerUC
		{
			public:
                PharmacyChainPickerUC();
                virtual ~PharmacyChainPickerUC();

                SYSTEM_EVENT_HANDLER_DECLARATION( SearchPharmacyChainRequested );

				void injectPharmacyChainPickerVC( tender::viewConn::PharmacyChainPickerVCPtr pharmacyChainPickerVC );
				void injectPharmacyChainCollectionDM( libtender::domMod::pharmacychain::IPharmacyChainCollectionDMPtr pharmacyChainCollectionDM );
                void injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter );

				virtual void setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit );

                virtual void                                               run();
			    virtual basar::db::aspect::AccessorPropertyTable_YIterator getPicked();
				virtual basar::db::aspect::AccessorPropertyTableRef		   getFound();
				virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYIterator();
				virtual void setCheckMode(bool);
				virtual void setSearchWithoutGUIMode(bool);
				
				virtual bool isPharmacyChainValid();
				virtual bool isCustomerGroupIDValid();

			private:
				PharmacyChainPickerUC(const PharmacyChainPickerUC& );
				PharmacyChainPickerUC& operator= ( const PharmacyChainPickerUC& );

				void init();
				void reset();
				void registerEventHandlers( const bool forSave );

				const log4cplus::Logger&										getLogger() const;
				tender::viewConn::PharmacyChainPickerVCPtr						getPharmacyChainPickerVC();
				libtender::domMod::pharmacychain::IPharmacyChainCollectionDMPtr getPharmacyChainCollectionDM();
				tender::componentManager::IUseCaseGetterPtr						getUseCaseGetter();

				basar::appl::EventReturnType									searchPharmacyChain();
				void															checkPharmacyChain();
				void															checkCustomerGroupID();

				//////////////////////////////////////////////////////////////////////////////////////
                // member variables
                //////////////////////////////////////////////////////////////////////////////////////
				tender::viewConn::PharmacyChainPickerVCPtr						m_PharmacyChainPickerVC;
				libtender::domMod::pharmacychain::IPharmacyChainCollectionDMPtr m_PharmacyChain;
				tender::componentManager::IUseCaseGetterPtr						m_UseCaseGetter;

				const log4cplus::Logger											m_Logger;
				bool															m_CheckMode;
				bool															m_SeachWithoutGUIMode;

				bool															m_isPharmacyChainValid;
				bool															m_isCustomerGroupIDValid;
		};

	} // end namespace useCase
} // end namespace tender

#endif // end GUARD_TENDER_USECASE_PHARMACYCHAINPICKERUC_H
