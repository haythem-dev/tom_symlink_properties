//---------------------------------------------------------------------------//
/*! \file   
 *  \brief      
 *  \author     
 *  \date       
 *  \version    
 */ //-----------------------------------------------------------------------//

#ifndef GUARD_LIBABBAUW_LOGGERPOOL_H
#define GUARD_LIBABBAUW_LOGGERPOOL_H

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
namespace libabbauw
{
    class LoggerPool
    {
        public:
            static log4cplus::Logger loggerViewConn;
            static log4cplus::Logger loggerUseCases;
            static log4cplus::Logger loggerMain;

	        static log4cplus::Logger loggerLibAbbaUW;
			static log4cplus::Logger loggerTransferOrder;
            static log4cplus::Logger loggerTransferOrderCreation;
			static log4cplus::Logger loggerStock;
			static log4cplus::Logger loggerCSCOrder;
			static log4cplus::Logger loggerCustomer;
			static log4cplus::Logger loggerManufacturer;
			static log4cplus::Logger loggerOrderProposal;
			static log4cplus::Logger loggerParameter;
            static log4cplus::Logger loggerPurchaseOrderSupplier;
			static log4cplus::Logger loggerArticle;  
	};
} 

#endif // GUARD_LIBABBAUW_LOGGERPOOL_H
