#include "cmnexcept.h"
#include "orderprotocolexception.h"

namespace libabbauw
{
    OrderProtocolException::OrderProtocolException( const basar::ExceptInfo & sInfo )
    : AbbauwException( sInfo, "OrderProtocolException" )
    {
    }
    
    OrderProtocolException::~OrderProtocolException()
    {
    }
} 
