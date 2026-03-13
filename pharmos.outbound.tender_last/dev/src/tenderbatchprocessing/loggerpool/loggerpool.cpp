//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "loggerpool/loggerpool.h"

//--------------------------------------------------------------------------------------------------//
// using declaration section
//--------------------------------------------------------------------------------------------------//
//using log4cplus::Logger;
using basar::cmnutil::Logging;

//--------------------------------------------------------------------------------------------------//
// class definition section
//--------------------------------------------------------------------------------------------------//
namespace tenderbatchprocessing
{
    using log4cplus::Logger;
    using basar::cmnutil::Logging;

    const log4cplus::Logger& LoggerPool::getLoggerTenderbatchprocessing()
    {
        static Logger log = Logging::getInstance( "tenderbatchprocessing" );
        return log;
    }

    const log4cplus::Logger& LoggerPool::getLoggerUseCases()
    {
        static Logger log = Logging::getInstance( "tenderbatchprocessing.useCase" );
        return log;
    }

    const log4cplus::Logger& LoggerPool::getLoggerDomModules()
    {
        static Logger log = Logging::getInstance( "tenderbatchprocessing.domMod" );
        return log;
    }

    const log4cplus::Logger& LoggerPool::getLoggerDebugOutput()
    {
        static Logger log = Logging::getInstance( "tenderbatchprocessing.debugOutput" );
        return log;
    }

}    //namespace transfer2bfarm
