#ifndef GUARD_TENDERAPPLCTRL_H
#define GUARD_TENDERAPPLCTRL_H

#include "tenderinfrastructurecompmgrptr.h"

#include <libutil/classinfo.h>
#include <libbasarcmnutil.h>


namespace tender
{
    class ApplCtrlImpl
    {
        friend class basar::cmnutil::Singleton<ApplCtrlImpl>;

        public:
            static const libutil::misc::ClassInfo&                      getClassInfo();

        public:
            ~ApplCtrlImpl();

            void                                                    init( int argc, char *argv[] );
            void                                                    run();
            void                                                    shutdown();

        private:
            ApplCtrlImpl                ();
            ApplCtrlImpl                ( const ApplCtrlImpl& );
            ApplCtrlImpl & operator =    ( const ApplCtrlImpl& );

            const log4cplus::Logger&                                    getLogger() const;

            void                                                        createInfrastructureCM( int argc, char *argv[] );
            void                                                        destroyInfrastructureCM();
            componentManager::InfrastructureCompMgrPtr                  getInfrastructureCM();

            const log4cplus::Logger	                                    m_Logger;
            componentManager::InfrastructureCompMgrPtr                  m_InfrastructureCM;
    };

    //---------------------------------------------------------------------------------------//
    /*! \brief  derived class from tenderApplBase.
                This is a final class. Singleton
    *  \note    class is necessary for dll exporting Singleton\<tenderApplBase\>  */
    class ApplCtrl : public basar::cmnutil::Singleton< ApplCtrlImpl >
    {
        private:
            ApplCtrl            ();
            ApplCtrl                        ( const ApplCtrl& r );
            ApplCtrl &          operator =  ( const ApplCtrl& r );
    };

}

#endif 
