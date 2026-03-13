#ifndef GUARD_TENDER_USECASE_SUPPLIERPICKERUC_H
#define GUARD_TENDER_USECASE_SUPPLIERPICKERUC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "supplierpickervcptr.h"
#include "ipickeruc.h"
#include "iusecasegetterptr.h"

#include <domainmodule/supplier/isuppliercollectiondmptr.h>

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
        class SupplierPickerUC : public tender::useCase::IPickerUC
        {
            public:
                SupplierPickerUC();
                virtual ~SupplierPickerUC();

                SYSTEM_EVENT_HANDLER_DECLARATION( SearchSupplierRequested );

                void injectSupplierPickerVC      ( tender::viewConn::SupplierPickerVCPtr                supplierPickerVC     );
                void injectSupplierCollectionDM  ( libtender::domMod::supplier::ISupplierCollectionDMPtr supplierCollectionDM );
                void injectUseCaseGetter        ( tender::componentManager::IUseCaseGetterPtr         useCaseGetter       );

                virtual void setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yit );

                virtual void                                               run();
                virtual basar::db::aspect::AccessorPropertyTable_YIterator getPicked();
                virtual basar::db::aspect::AccessorPropertyTable_YIterator getSearchYIterator();
                virtual void setCheckMode( bool );

            private:
                SupplierPickerUC(const SupplierPickerUC& );
                SupplierPickerUC& operator= ( const SupplierPickerUC& );

                void init();
                void reset();
                void registerEventHandlers( const bool forSave );

                const log4cplus::Logger&                               getLogger() const;
                tender::viewConn::SupplierPickerVCPtr                  getSupplierPickerVC();
                libtender::domMod::supplier::ISupplierCollectionDMPtr  getSupplierCollectionDM();
                tender::componentManager::IUseCaseGetterPtr            getUseCaseGetter();

                basar::appl::EventReturnType                           searchSupplier();
                basar::appl::EventReturnType                           checkSupplier();

                //////////////////////////////////////////////////////////////////////////////////////
                // member variables
                //////////////////////////////////////////////////////////////////////////////////////
                tender::viewConn::SupplierPickerVCPtr                   m_SupplierPickerVC;
                libtender::domMod::supplier::ISupplierCollectionDMPtr   m_Supplier;
                tender::componentManager::IUseCaseGetterPtr             m_UseCaseGetter;

                const log4cplus::Logger                                 m_Logger;
                bool                                                    m_CheckMode;
        };

    } // end namespace useCase
} // end namespace tender

#endif // end GUARD_TENDER_USECASE_SUPPLIERPICKERUC_H
