#ifndef GUARD_LIBABBAUW_DATA_ORDERPROTOCOL_EXCEPTION_H
#define GUARD_LIBABBAUW_DATA_ORDERPROTOCOL_EXCEPTION_H

#include "abbauwexception.h"

namespace libabbauw
{
    class OrderProtocolException : public AbbauwException
    {
        public:
        	OrderProtocolException( const basar::ExceptInfo & sInfo );
            ~OrderProtocolException();
        
        private:
        	OrderProtocolException();					
    };
} 

#endif // GUARD_LIBABBAUW_DATA_ORDERPROTOCOL_EXCEPTION_H
