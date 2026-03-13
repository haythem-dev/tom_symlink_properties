#ifndef GUARD_CSCBATCHESHISTORY_LOGGERPOOL_H
#define GUARD_CSCBATCHESHISTORY_LOGGERPOOL_H

namespace log4cplus 
{
	class Logger;
}

namespace cscbatchhistory 
{
	class LoggerPool 
	{
	public:	
		static log4cplus::Logger	loggerMain;			//!< logger for cscbatchhistory (global)
		static log4cplus::Logger	loggerUseCases;		//!< logger for use cases
		static log4cplus::Logger	loggerDomModules;	//!< logger for domain modules
	};
}

#endif // GUARD_CSCBATCHESHISTORY_LOGGERPOOL_H
