#include "loggerpool.h"
#include <libbasarcmnutil.h>

using log4cplus::Logger;
using basar::cmnutil::Logging;

namespace abbaUW
{
    Logger LoggerPool::LoggerAbbaUW = Logging::getInstance( "abbauw" );  // equals to Name in .cfg-File
    //Logger LoggerPool::LoggerUseCases				= Logging::getInstance( "returnproposer.useCases" );
} 

//namespace domMod 
//{
//    Logger LoggerPool::LoggerDomModules				= Logging::getInstance( "domMod" );
//    //Logger LoggerPool::LoggerGoodsInDM				= Logging::getInstance( "domMod.goodsindm" );
//
//} 
