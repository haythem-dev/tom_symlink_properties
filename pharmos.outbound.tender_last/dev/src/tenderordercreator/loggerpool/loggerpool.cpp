//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "loggerpool/loggerpool.h"
#include <libbasarcmnutil.h>

//--------------------------------------------------------------------------------------------------//
// using declaration section
//--------------------------------------------------------------------------------------------------//
//using log4cplus::Logger;
using basar::cmnutil::Logging;

//--------------------------------------------------------------------------------------------------//
// class definition section
//--------------------------------------------------------------------------------------------------//
namespace tenderordercreator
{
    using log4cplus::Logger;
    using basar::cmnutil::Logging;

    const log4cplus::Logger& LoggerPool::getLoggerTenderordercreator()
    {
        static Logger log = Logging::getInstance( "tenderordercreator" );
        return log;
    }

    const log4cplus::Logger& LoggerPool::getLoggerUseCases()
    {
        static Logger log = Logging::getInstance( "tenderordercreator.useCase" );
        return log;
    }

    const log4cplus::Logger& LoggerPool::getLoggerDomModules()
    {
        static Logger log = Logging::getInstance( "tenderordercreator.domMod" );
        return log;
    }

    const log4cplus::Logger& LoggerPool::getLoggerDebugOutput()
    {
        static Logger log = Logging::getInstance( "tenderordercreator.debugOutput" );
        return log;
    }

}    //namespace transfer2bfarm
