//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects in CSC_BATCHES
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBCSCBATCH_LOGGERPOOL_H
#define GUARD_LIBCSCBATCH_LOGGERPOOL_H

namespace log4cplus {
  class Logger;
}

//----------------------------------------------------------------------------
namespace libcscbatch {

//---------------------------------------------------------------------------
//! class for initialized logger objects\n final class

class LoggerPool
{
public:	
	static log4cplus::Logger	loggerMain;		//!< logger for CSC_BATCHES (global)
	static log4cplus::Logger	loggerViewConn;							//!< logger for view connectors	
	static log4cplus::Logger	loggerUseCases;							//!< logger for use cases
	static log4cplus::Logger	loggerDomModules;						//!< logger for domain modules
};

}	//namespace libcscbatch

#endif	// GUARD_LIBCSCBATCH_LOGGERPOOL_H
