#ifndef GUARD_LIBTENDER_TENDERLOGGERPOOL_H
#define GUARD_LIBTENDER_TENDERLOGGERPOOL_H

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
namespace libtender
{
    class LoggerPool
    {
        public:
            static const log4cplus::Logger & getLoggerTender();
            static const log4cplus::Logger & getLoggerTenderImport();
            static const log4cplus::Logger & getLoggerViewConn();
            static const log4cplus::Logger & getLoggerUseCases();
            static const log4cplus::Logger & getLoggerDomModules();
            static const log4cplus::Logger & getLoggerDebugOutput();
    };
} // tender

#endif //end GUARD_LIBTENDER_TENDERLOGGERPOOL_H
