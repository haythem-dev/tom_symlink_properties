//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "tenderloggerpool.h"
#include <libbasarcmnutil.h>

//--------------------------------------------------------------------------------------------------//
// using declaration section
//--------------------------------------------------------------------------------------------------//
using basar::cmnutil::Logging;

//--------------------------------------------------------------------------------------------------//
// class definition section
//--------------------------------------------------------------------------------------------------//
namespace libtender
{
    using log4cplus::Logger;
    using basar::cmnutil::Logging;

    const log4cplus::Logger & LoggerPool::getLoggerTender()
    {
		static log4cplus::Logger logger = Logging::getInstance( "tender" );
		return logger;
    }

    const log4cplus::Logger & LoggerPool::getLoggerTenderImport()
    {
        static log4cplus::Logger logger = Logging::getInstance( "tender.import" );
		return logger;
    }

    const log4cplus::Logger & LoggerPool::getLoggerViewConn()
    {
        static log4cplus::Logger logger = Logging::getInstance( "tender.viewConn" );
		return logger;
    }

    const log4cplus::Logger & LoggerPool::getLoggerUseCases()
    {
        static log4cplus::Logger logger = Logging::getInstance( "tender.useCase" );
		return logger;
    }

    const log4cplus::Logger & LoggerPool::getLoggerDomModules()
    {
        static log4cplus::Logger logger = Logging::getInstance( "tender.domMod" );
		return logger;
    }

    const log4cplus::Logger & LoggerPool::getLoggerDebugOutput()
    {
        static log4cplus::Logger logger = Logging::getInstance( "tender.debugOutput" );
		return logger;
    }

}    //namespace libtender
