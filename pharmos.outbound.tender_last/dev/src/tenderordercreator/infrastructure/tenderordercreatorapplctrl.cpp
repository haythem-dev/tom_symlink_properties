 //-------------------------------------------------------------------------------------------------//
/*! \file
*   \brief  application (main) functions for TENDERORDERCREATOR
*   \author
*   \date
*/
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "tenderordercreatorapplctrl.h"
#include "componentmanager/tenderordercreatorinfrastructurecompmgr.h"

#include "loggerpool/loggerpool.h"

#include <libutil/irunnable.h>
#include <libutil/exception.h>

//-------------------------------------------------------------------------------------------------//
// singleton qualification
//-------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace cmnutil
    {
        /*! specialised method */
        template <>
        typename Singleton<tenderordercreator::ApplCtrlImpl>::InstancePtr& Singleton<tenderordercreator::ApplCtrlImpl>::inst()
        {
            static InstancePtr s_inst;
            return s_inst;
        }
    }
}

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace tenderordercreator
{
    //-------------------------------------------------------------------------------------------------//
    /*! \throw no-throw */
    //-------------------------------------------------------------------------------------------------//
    ApplCtrlImpl::ApplCtrlImpl()
    : m_Logger( LoggerPool::getLoggerTenderordercreator() )
    {
        METHODNAME_DEF( ApplCtrlImpl, ApplCtrlImpl )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    //-------------------------------------------------------------------------------------------------//
    /*! \throw no-throw */
    //-------------------------------------------------------------------------------------------------//
    ApplCtrlImpl::~ApplCtrlImpl()
    {
        METHODNAME_DEF( ApplCtrlImpl, ~ApplCtrlImpl )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    const log4cplus::Logger& ApplCtrlImpl::getLogger() const
    {
        return m_Logger;
    }

    //-------------------------------------------------------------------------------------------------//
    /*! \throw no-throw */
    //-------------------------------------------------------------------------------------------------//
    void ApplCtrlImpl::createInfrastructureCM( int argc, char *argv[] )
    {
        METHODNAME_DEF( ApplCtrlImpl, createInfrastructureCM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_InfrastructureCM = componentManager::InfrastructureCompMgrPtr( new componentManager::InfrastructureCompMgr(argc, argv) );
    }

    //-------------------------------------------------------------------------------------------------//
    /*! \throw no-throw */
    //-------------------------------------------------------------------------------------------------//
    componentManager::InfrastructureCompMgrPtr ApplCtrlImpl::getInfrastructureCM()
    {
        METHODNAME_DEF( ApplCtrlImpl, getInfrastructureCM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        return m_InfrastructureCM;
    }

    //-------------------------------------------------------------------------------------------------//
    /*! \throw no-throw */
    //-------------------------------------------------------------------------------------------------//
    void ApplCtrlImpl::destroyInfrastructureCM()
    {
        METHODNAME_DEF( ApplCtrlImpl, destroyInfrastructureCM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_InfrastructureCM.reset();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    void ApplCtrlImpl::init( int argc, char *argv[] )
    {
        METHODNAME_DEF( ApplCtrlImpl, init )
        BLOG_TRACE_METHOD( getLogger(), fun );

        try {
            createInfrastructureCM( argc, argv );
            getInfrastructureCM()->getInfrastructureSetup()->init();
        } catch( basar::Exception& e ){
            libutil::exceptions::ExceptionHandler::processBasarException( e, getLogger(), fun, __FILE__, __LINE__ );
            throw;
        }
    }

    //-------------------------------------------------------------------------------------------------//
    /*! \throw no-throw
    \retval true if initialization was ok */
    //-------------------------------------------------------------------------------------------------//
    void ApplCtrlImpl::run()
    {
        METHODNAME_DEF( ApplCtrlImpl, run )
        BLOG_TRACE_METHOD( getLogger(), fun );

        try {
            getInfrastructureCM()->getInfrastructureSetup()->checkSetting();
            getInfrastructureCM()->getApplComponentManager()->getMainUC()->run();
        } catch( basar::Exception& e ){
            libutil::exceptions::ExceptionHandler::processBasarException( e, getLogger(), fun, __FILE__, __LINE__ );
            throw;
        }
    }

    //-------------------------------------------------------------------------------------------------//
    /*! \throw no-throw */
    //-------------------------------------------------------------------------------------------------//
    void ApplCtrlImpl::shutdown()
    {
        METHODNAME_DEF( ApplCtrlImpl, shutdown )
        BLOG_TRACE_METHOD( getLogger(), fun );

        try{
            getInfrastructureCM()->getApplComponentManager()->clear();
            getInfrastructureCM()->getInfrastructureSetup()->shutdown();
            destroyInfrastructureCM();
        } catch( basar::Exception& e ){
            libutil::exceptions::ExceptionHandler::processBasarException( e, getLogger(), fun, __FILE__, __LINE__ );
            throw;
        }
    }

} // namespace tenderordercreator
