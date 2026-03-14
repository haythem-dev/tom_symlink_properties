#ifndef GUARD_LIBLOCKMANAGER_LOGGERPOOL_H
#define GUARD_LIBLOCKMANAGER_LOGGERPOOL_H

//---------------------------------------------------------------------------//
// forward declaration section 
//---------------------------------------------------------------------------//
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

//---------------------------------------------------------------------------//
// class definition section
//---------------------------------------------------------------------------//
namespace libLockManager
{
    class LoggerPool
    {
        public:	
	        static log4cplus::Logger loggerLiblockManager;
            static log4cplus::Logger loggerAccessor;
    };
} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_LOGGERPOOL_H
