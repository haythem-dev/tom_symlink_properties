#ifndef GUARD_TENDERBATCHPROCESSINGAPPLCTRL_H
#define GUARD_TENDERBATCHPROCESSINGAPPLCTRL_H

#include "componentmanager/tenderbatchprocessinginfrastructurecompmgrptr.h"
#include <libbasarcmnutil.h>

namespace tenderbatchprocessing
{
    class ApplCtrlImpl
    {
        friend class basar::cmnutil::Singleton<ApplCtrlImpl>;

        public:
            ~ApplCtrlImpl();

            void											init( int argc, char *argv[] );
            void											run();
            void											shutdown();

        private:
            ApplCtrlImpl();
            ApplCtrlImpl( const ApplCtrlImpl& );
            ApplCtrlImpl & operator = ( const ApplCtrlImpl& );

            const log4cplus::Logger                         getLogger() const;

            void                                            createInfrastructureCM( int argc, char *argv[] );
            void                                            destroyInfrastructureCM();
            componentManager::InfrastructureCompMgrPtr      getInfrastructureCM();

            const log4cplus::Logger                         m_Logger;
            componentManager::InfrastructureCompMgrPtr      m_InfrastructureCM;
    };

    //---------------------------------------------------------------------------------------//
    /*! \brief  derived class from TENDERBATCHPROCESSINGApplBase.
                This is a final class. Singleton
    *  \note    class is necessary for dll exporting Singleton\<TENDERBATCHPROCESSINGApplBase\>  */
    class ApplCtrl : public basar::cmnutil::Singleton< ApplCtrlImpl >
    {
        private:
            ApplCtrl            ();
            ApplCtrl                        ( const ApplCtrl& r );
            ApplCtrl &          operator =  ( const ApplCtrl& r );
    };

}    // end namespace tenderbatchprocessing

#endif //end GUARD_TENDERBATCHPROCESSINGAPPLCTRL_H
