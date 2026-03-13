//------------------------------------------------------------------------------------------------------------//
// include section
//------------------------------------------------------------------------------------------------------------//
#include "libabbauw_loggerpool.h"
#include <libbasarcmnutil.h>

//------------------------------------------------------------------------------------------------------------//
// using declaration section
//------------------------------------------------------------------------------------------------------------//
using log4cplus::Logger;
using basar::cmnutil::Logging;

//------------------------------------------------------------------------------------------------------------//
// namespace section
//------------------------------------------------------------------------------------------------------------//
namespace libabbauw
{
    Logger LoggerPool::loggerViewConn              = Logging::getInstance( "csc_batches.viewConn" );
    Logger LoggerPool::loggerUseCases              = Logging::getInstance( "csc_batches.useCases" );
    Logger LoggerPool::loggerMain                  = Logging::getInstance( "csc_batches.main" );

    Logger LoggerPool::loggerLibAbbaUW             = Logging::getInstance( "libabbauw" );  // equals to Name in .cfg-File
    Logger LoggerPool::loggerTransferOrder         = Logging::getInstance( "libabbauw.transferorder" );
    Logger LoggerPool::loggerTransferOrderCreation = Logging::getInstance( "libabbauw.transferordercreation" );
	Logger LoggerPool::loggerCSCOrder              = Logging::getInstance( "libabbauw.cscorder" );
	Logger LoggerPool::loggerCustomer              = Logging::getInstance( "libabbauw.customer" );
	Logger LoggerPool::loggerManufacturer          = Logging::getInstance( "libabbauw.manufacturerdm" );
	Logger LoggerPool::loggerArticle		       = Logging::getInstance( "libabbauw.article" );
    Logger LoggerPool::loggerStock                 = Logging::getInstance( "libabbauw.stock" );
	Logger LoggerPool::loggerOrderProposal         = Logging::getInstance( "libabbauw.orderproposal" );
	Logger LoggerPool::loggerParameter             = Logging::getInstance( "libabbauw.domMod.parameter" );
    Logger LoggerPool::loggerPurchaseOrderSupplier = Logging::getInstance( "libabbauw.domMod.purchaseordersupplierdm" );
}
