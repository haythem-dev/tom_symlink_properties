#include "cscbatchesapplctrl.h"
#include "infrastructurecompmgr.h"
#include "libabbauw/loggerpool/libabbauw_loggerpool.h"

#include <libutil/irunnable.h>
#include <libutil/exception.h>

#include <libbasarguitie.h>

namespace basar
{
    namespace cmnutil
    {
        /*! specialised method */
        template <>
        typename Singleton< CSC_Batches::ApplCtrlImpl >::InstancePtr& Singleton< CSC_Batches::ApplCtrlImpl >::inst()
        {
            static InstancePtr s_inst;
            return s_inst;
        }
    }
}

namespace CSC_Batches
{

    ApplCtrlImpl::ApplCtrlImpl()
        : m_Logger( libabbauw::LoggerPool::loggerLibAbbaUW )
    {
        METHODNAME_DEF( ApplCtrlImpl, ApplCtrlImpl )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    ApplCtrlImpl::~ApplCtrlImpl()
    {
        METHODNAME_DEF( ApplCtrlImpl, ~ApplCtrlImpl )
        BLOG_TRACE_METHOD( getLogger(), fun );
    }

    const log4cplus::Logger& ApplCtrlImpl::getLogger() const
    {
        return m_Logger;
    }

    void ApplCtrlImpl::createInfrastructureCM( int argc, char *argv[] )
    {
        METHODNAME_DEF( ApplCtrlImpl, createInfrastructureCM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_InfrastructureCM = componentManager::InfrastructureCompMgrPtr( new componentManager::InfrastructureCompMgr( argc, argv ) );
    }

    componentManager::InfrastructureCompMgrPtr ApplCtrlImpl::getInfrastructureCM()
    {
        METHODNAME_DEF( ApplCtrlImpl, getInfrastructureCM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        return m_InfrastructureCM;
    }

    void ApplCtrlImpl::deleteInfrastructureCM()
    {
        METHODNAME_DEF( ApplCtrlImpl, deleteInfrastructureCM )
        BLOG_TRACE_METHOD( getLogger(), fun );

        m_InfrastructureCM.reset();
    }

    void ApplCtrlImpl::init( int argc, char *argv[] )
    {
        METHODNAME_DEF( ApplCtrlImpl, init )
        BLOG_TRACE_METHOD( getLogger(), fun );

        try
        {
            createInfrastructureCM( argc, argv );
            getInfrastructureCM()->getInfrastructureSetup()->init();
        }
        catch( basar::Exception& e )
        {
            libutil::exceptions::ExceptionHandler::processBasarException( e, getLogger(), fun, __FILE__, __LINE__ );
            throw;
        }
    }

    void ApplCtrlImpl::run()
    {
        METHODNAME_DEF( ApplCtrlImpl, run )
        BLOG_TRACE_METHOD( getLogger(), fun );

        try
        {
            getInfrastructureCM()->getInfrastructureSetup()->checkSetting();
            getInfrastructureCM()->getApplComponentManager()->getMainUC()->run();
        }
        catch( basar::Exception& e )
        {
            libutil::exceptions::ExceptionHandler::processBasarException( e, getLogger(), fun, __FILE__, __LINE__ );
        }
    }

    void ApplCtrlImpl::shutdown()
    {
        METHODNAME_DEF( ApplCtrlImpl, shutdown )
        BLOG_TRACE_METHOD( getLogger(), fun );

        try
        {
            getInfrastructureCM()->getApplComponentManager()->clear();
            getInfrastructureCM()->getInfrastructureSetup()->shutdown();
            deleteInfrastructureCM();
        }
        catch( basar::Exception& e )
        {
            libutil::exceptions::ExceptionHandler::processBasarException( e, getLogger(), fun, __FILE__, __LINE__ );
        }
    }

} // namespace CSC_Batches
