#include "loggerpool.h"
#include <libbasarcmnutil.h>

namespace cscbatchhistory
{
	//! \brief get global logging instance for CSC_BATCHES
	log4cplus::Logger LoggerPool::loggerMain = basar::cmnutil::Logging::getInstance("cscbatchhistory");

	//! \brief get logging instance for Use Cases
	log4cplus::Logger LoggerPool::loggerUseCases = basar::cmnutil::Logging::getInstance("cscbatchhistory.useCases");

	//! \brief get logging instance for Domain Modules
	log4cplus::Logger LoggerPool::loggerDomModules = basar::cmnutil::Logging::getInstance("cscbatchhistory.domMod");
}
