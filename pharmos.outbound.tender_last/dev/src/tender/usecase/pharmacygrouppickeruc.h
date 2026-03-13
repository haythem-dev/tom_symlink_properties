#ifndef GUARD_TENDER_USECASE_PHARMACYGROUPPICKERUC_H
#define GUARD_TENDER_USECASE_PHARMACYGROUPPICKERUC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "pharmacygrouppickervcptr.h"
#include "ipickeruc.h"
#include "iusecasegetterptr.h"

#include <domainmodule/pharmacygroup/ipharmacygroupcollectiondmptr.h>

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
        class PharmacyGroupPickerUC : public tender::useCase::IPickerUC
        {
            public:
                PharmacyGroupPickerUC();
                virtual ~PharmacyGroupPickerUC();
                
                SYSTEM_EVENT_HANDLER_DECLARATION( SearchPharmacyGroupRequested );
                
                void injectPharmacyGroupPickerVC        ( tender::viewConn::PharmacyGroupPickerVCPtr                        pharmacyGroupPickerVC     );
                void injectPharmacyGroupCollectionDM    ( libtender::domMod::pharmacygroup::IPharmacyGroupCollectionDMPtr   pharmacyGroupCollectionDM );
                void injectUseCaseGetter                ( tender::componentManager::IUseCaseGetterPtr                       useCaseGetter             );
                
                virtual void setSearchCriteria  ( basar::db::aspect::AccessorPropertyTable_YIterator yit );
                
                virtual void                                               run();
                virtual basar::db::aspect::AccessorPropertyTable_YIterator getPicked();
                virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYIterator();
                virtual void setCheckMode(bool);
            
            private:
                PharmacyGroupPickerUC(const PharmacyGroupPickerUC& );
                PharmacyGroupPickerUC& operator= ( const PharmacyGroupPickerUC& );
                
                void init();
                void reset();
                void registerEventHandlers( const bool forSave );
                
                const log4cplus::Logger&                                        getLogger() const;
                tender::viewConn::PharmacyGroupPickerVCPtr                      getPharmacyGroupPickerVC();
                libtender::domMod::pharmacygroup::IPharmacyGroupCollectionDMPtr getPharmacyGroupCollectionDM();
                tender::componentManager::IUseCaseGetterPtr                     getUseCaseGetter();
                
                basar::appl::EventReturnType searchPharmacyGroup();
                basar::appl::EventReturnType checkPharmacyGroup();
                
                //////////////////////////////////////////////////////////////////////////////////////
                // member variables
                //////////////////////////////////////////////////////////////////////////////////////
                tender::viewConn::PharmacyGroupPickerVCPtr                      m_PharmacyGroupPickerVC;
                libtender::domMod::pharmacygroup::IPharmacyGroupCollectionDMPtr m_PharmacyGroup;
                tender::componentManager::IUseCaseGetterPtr                     m_UseCaseGetter;
                
                const log4cplus::Logger                                         m_Logger;
                bool                                                            m_CheckMode;
        };
    
    } // end namespace useCase
} // end namespace tender

#endif // end GUARD_TENDER_USECASE_PHARMACYGROUPPICKERUC_H
