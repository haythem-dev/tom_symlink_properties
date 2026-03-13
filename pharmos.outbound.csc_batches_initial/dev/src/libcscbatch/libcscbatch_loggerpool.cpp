//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------
#include "libcscbatch_loggerpool.h"
#include <libbasarcmnutil.h>

//----------------------------------------------------------------------------
namespace libcscbatch
{
//! \brief get global logging instance for CSC_Batches
log4cplus::Logger LoggerPool::loggerMain = basar::cmnutil::Logging::getInstance("csc_batches");

//! \brief get logging instance for View Connectors
log4cplus::Logger LoggerPool::loggerViewConn = basar::cmnutil::Logging::getInstance("csc_batches.viewConn");

//! \brief get logging instance for Use Cases
log4cplus::Logger LoggerPool::loggerUseCases = basar::cmnutil::Logging::getInstance("csc_batches.useCases");

//! \brief get logging instance for Domain Modules
log4cplus::Logger LoggerPool::loggerDomModules	= basar::cmnutil::Logging::getInstance("csc_batches.domMod");

}// namespace libcscbatch
