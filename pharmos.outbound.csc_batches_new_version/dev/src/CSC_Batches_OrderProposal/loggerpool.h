//---------------------------------------------------------------------------//
/*! \file   
 *  \brief      
 *  \author     
 *  \date       
 *  \version    
 */ //-----------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_LOGGERPOOL_H
#define GUARD_ABBAUW_LOGGERPOOL_H

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
namespace abbaUW
{
    class LoggerPool
    {
        public:	
	        static log4cplus::Logger LoggerAbbaUW;
	        //static log4cplus::Logger LoggerUseCases;
    };
} 


//namespace domMod
//{
//    class LoggerPool
//    {
//        public:	
//	        static log4cplus::Logger LoggerDomModules;
//	        //static log4cplus::Logger LoggerGoodsInDM;
//    };
//} 

#endif 
