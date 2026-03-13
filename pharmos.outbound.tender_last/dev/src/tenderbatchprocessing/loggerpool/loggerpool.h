#ifndef GUARD_IMEXTRANFER2BFARM_LOGGERPOOL_H
#define GUARD_IMEXTRANFER2BFARM_LOGGERPOOL_H

#include <libbasarcmnutil.h>

//--------------------------------------------------------------------------------------------------//
// forward declaration section
//--------------------------------------------------------------------------------------------------//
namespace log4cplus
{
    class Logger;
}

namespace basar
{
    namespace cmnutil
    {
        class Logging;
    }
}

//--------------------------------------------------------------------------------------------------//
// class declaration section
//--------------------------------------------------------------------------------------------------//
namespace tenderbatchprocessing
{
    class LoggerPool
    {
        public:
            static const log4cplus::Logger&    getLoggerTenderbatchprocessing();
            static const log4cplus::Logger&    getLoggerUseCases();
            static const log4cplus::Logger&    getLoggerDomModules();
            static const log4cplus::Logger&    getLoggerDebugOutput();
    };
} // transfer2bfarm

#endif
