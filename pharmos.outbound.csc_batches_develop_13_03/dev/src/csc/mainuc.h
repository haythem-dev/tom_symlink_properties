#ifndef GUARD_CSC_BATCHES_USECASE_MAINUC_H
#define GUARD_CSC_BATCHES_USECASE_MAINUC_H

#include "mainwindowvcptr.h"
#include "applicationconfigvcptr.h"
#include "iusecasegetterptr.h"
#include "libabbauw/iparameterdmptr.h"
#include "iuserrightsptr.h"

#include <libutil/qapplication.h>
#include <libutil/irunnable.h>

#include <libbasarappl_eventmacros.h>
#include <libbasarappl_systemeventmanager.h>

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

namespace CSC_Batches
{
namespace useCase
{
    class MainUC : public libutil::useCase::IRunnable
    {
        public:
            MainUC();
            ~MainUC();

            void                                                     injectMainVC( CSC_Batches::viewConn::MainWindowVCPtr );
            void                                                     injectApplicationConfigVC( basar::gui::qt::ApplicationConfigVCPtr );
            void                                                     injectQApplication( libutil::infrastructure::IQApplicationPtr );
            void                                                     injectUseCaseGetter( CSC_Batches::componentManager::IUseCaseGetterPtr );
            void                                                     injectParameter( libabbauw::domMod::parameter::IParameterDMPtr );
            void                                                     injectUserRights( infrastructure::rights::IUserRightsPtr );

            void                                                     run();

            SYSTEM_EVENT_HANDLER_DECLARATION( startAboutDlg                       )
            SYSTEM_EVENT_HANDLER_DECLARATION( startBatchOrderDlg                  )
            SYSTEM_EVENT_HANDLER_DECLARATION( startBatchOrderMaintenanceRequested )
            SYSTEM_EVENT_HANDLER_DECLARATION( createTransferOrderRequested        )
		    SYSTEM_EVENT_HANDLER_DECLARATION( editBatchOrderRequested             )
		    SYSTEM_EVENT_HANDLER_DECLARATION( startSettingsRequested              )

        private:
            MainUC( const MainUC& );
            MainUC& operator = (const MainUC& );

            void                                                     registerEventHandlers( const bool forSave );

            viewConn::MainWindowVCPtr                                getMainVC()           const;
            libutil::infrastructure::IQApplicationPtr                getQApplication()     const;
            componentManager::IUseCaseGetterPtr                      getUseCaseGetter()    const;
            libabbauw::domMod::parameter::IParameterDMPtr            getParameter()        const;
            infrastructure::rights::IUserRightsPtr                   getUserRights()       const;

        private:
            const log4cplus::Logger&                                 m_Logger;
            viewConn::MainWindowVCPtr                                m_MainVC;
            basar::gui::qt::ApplicationConfigVCPtr                   m_ApplicationConfigVC;
            libutil::infrastructure::IQApplicationPtr                m_QApplication;
            componentManager::IUseCaseGetterPtr                      m_UseCaseGetter;
            libabbauw::domMod::parameter::IParameterDMPtr            m_Parameter;
            infrastructure::rights::IUserRightsPtr                   m_UserRights;
    };
} // end namespace useCase
} // end namespace CSC_Batches

#endif // GUARD_CSC_BATCHES_USECASE_MAINUC_H
