#ifndef GUARD_TENDER_USECASE_MAINUC_H
#define GUARD_TENDER_USECASE_MAINUC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "mainvcptr.h"
#include "tenderapplcompmgrptr.h"
#include "iusecasegetterptr.h"
#include <domainmodule/tender/itendercollectiondmptr.h>

#include <libutil/irunnable.h>
#include <libutil/qapplication.h>

#include <libbasarappl_eventmacros.h>
#include <libbasarguitie.h>

//-------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace appl
    {
        class IEventSource;
        struct EventReturnStruct;
    }
}

namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
            class Manager;
        }
    }
}

namespace log4cplus
{
    class Logger;
}

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
        class MainUC : public libutil::useCase::IRunnable
        {
            public:
                MainUC();
                virtual ~MainUC();

                void                                                    injectMainVC( tender::viewConn::MainVCPtr mainVC);
                void                                                    injectQApplication( libutil::infrastructure::IQApplicationPtr qApplication);
                void                                                    injectUseCaseGetter( tender::componentManager::IUseCaseGetterPtr useCaseGetter);
				void													injectTenderCollectionDM( libtender::domMod::tender::ITenderCollectionDMPtr tenderCollectionDM );

                virtual void                                            run();

                SYSTEM_EVENT_HANDLER_DECLARATION( StartAboutDlg         )
                SYSTEM_EVENT_HANDLER_DECLARATION( StartCreateNewTender  )
                SYSTEM_EVENT_HANDLER_DECLARATION( StartSearchTender     )
                SYSTEM_EVENT_HANDLER_DECLARATION( StartImportTender     )
				SYSTEM_EVENT_HANDLER_DECLARATION( StartImportDeliveries )
                SYSTEM_EVENT_HANDLER_DECLARATION( StartCreateTO         ) // TODO remove later

            private:
                MainUC(const MainUC& );
                MainUC& operator= (const MainUC& );

                void                                                    registerEventHandlers( const bool forSave );

                tender::viewConn::MainVCPtr                             getMainVC();
                libutil::infrastructure::IQApplicationPtr               getQApplication();
                tender::componentManager::IUseCaseGetterPtr             getUseCaseGetter();
				libtender::domMod::tender::ITenderCollectionDMPtr		getTenderCollectionDM();

                //////////////////////////////////////////////////////////////////////////////////////
                // member variables
                //////////////////////////////////////////////////////////////////////////////////////
                const log4cplus::Logger	                                m_Logger;
                tender::viewConn::MainVCPtr                             m_MainVC;
				libtender::domMod::tender::ITenderCollectionDMPtr		m_TenderCollection;
                libutil::infrastructure::IQApplicationPtr               m_QApplication;
                tender::componentManager::IUseCaseGetterPtr             m_UseCaseGetter;
        };
    } // namespace useCase
} // namespace tender

#endif //end GUARD_TENDER_USECASE_MAINUC_H
