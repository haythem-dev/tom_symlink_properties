#include "liblockmanager_loggerpool.h"

#include <libbasarcmnutil_bstring.h>
#include <libbasarcmnutil_logging.h>

namespace libLockManager
{
    using basar::cmnutil::Logging;

    log4cplus::Logger LoggerPool::loggerLiblockManager        = Logging::getInstance( "libLockManager" );  // equals to Name in .cfg-File
    log4cplus::Logger LoggerPool::loggerAccessor              = Logging::getInstance( "libLockManager.accessor" );
}
