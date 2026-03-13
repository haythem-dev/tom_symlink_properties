#ifndef GUARD_IMEXTRANFER2BFARM_LOGGERPOOL_H
#define GUARD_IMEXTRANFER2BFARM_LOGGERPOOL_H

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
namespace tenderordercreator
{
    class LoggerPool
    {
        public:
            static const log4cplus::Logger&    getLoggerTenderordercreator();
            static const log4cplus::Logger&    getLoggerUseCases();
            static const log4cplus::Logger&    getLoggerDomModules();
            static const log4cplus::Logger&    getLoggerDebugOutput();
    };
} // transfer2bfarm

#endif
