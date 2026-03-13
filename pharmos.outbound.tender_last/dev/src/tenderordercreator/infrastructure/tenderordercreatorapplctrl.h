//------------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  application class (main) for TENDERORDERCREATOR component (singleton)
 *  \author
 *  \date
 */
//------------------------------------------------------------------------------------------------------------------//
#ifndef GUARD_TENDERORDERCREATORAPPLCTRL_H
#define GUARD_TENDERORDERCREATORAPPLCTRL_H

//------------------------------------------------------------------------------------------------------------------//
// include section
//------------------------------------------------------------------------------------------------------------------//
#include "componentmanager/tenderordercreatorinfrastructurecompmgrptr.h"
#include <libbasarcmnutil.h>

//------------------------------------------------------------------------------------------------------------------//
// class declaration section
//------------------------------------------------------------------------------------------------------------------//
namespace tenderordercreator
{
    class ApplCtrlImpl
    {
        //--------------------------------------------------------------------------------------------------//
        // friends
        //--------------------------------------------------------------------------------------------------//
        friend class basar::cmnutil::Singleton<ApplCtrlImpl>;

        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
        public:
            ~ApplCtrlImpl();

            void													init( int argc, char *argv[] );
            void													run();
            void													shutdown();

        private:
            //---------------------------------------------------------//
            // member methods
            //---------------------------------------------------------//
            ApplCtrlImpl                ();
            ApplCtrlImpl                ( const ApplCtrlImpl& );
            ApplCtrlImpl & operator =    ( const ApplCtrlImpl& );

            const log4cplus::Logger&                                    getLogger() const;

            void                                                        createInfrastructureCM( int argc, char *argv[] );
            void                                                        destroyInfrastructureCM();
            componentManager::InfrastructureCompMgrPtr                  getInfrastructureCM();

            //---------------------------------------------------------//
            // member variables
            //---------------------------------------------------------//
            const log4cplus::Logger&                                    m_Logger;
            componentManager::InfrastructureCompMgrPtr                  m_InfrastructureCM;
    };

    //---------------------------------------------------------------------------------------//
    /*! \brief  derived class from TENDERORDERCREATORApplBase.
                This is a final class. Singleton
    *  \note    class is necessary for dll exporting Singleton\<TENDERORDERCREATORApplBase\>  */
    class ApplCtrl : public basar::cmnutil::Singleton< ApplCtrlImpl >
    {
        private:
            ApplCtrl            ();
            ApplCtrl                        ( const ApplCtrl& r );
            ApplCtrl &          operator =  ( const ApplCtrl& r );
    };

}    // end namespace tenderordercreator

#endif //end GUARD_TENDERORDERCREATORAPPLCTRL_H
