#include "ordernotfoundexception.h"

namespace libabbauw
{
namespace exceptions
{

    OrderNotFoundException::OrderNotFoundException( const basar::ExceptInfo & sInfo ) 
    : BasarException( sInfo, "OrderNotFoundException" )
    {
    }

    OrderNotFoundException::~OrderNotFoundException()
    {
    }

    const basar::VarString & OrderNotFoundException::reason() const
    {
	    return m_InfoStruct.reason;
    }

} // end namespace exceptions
} // end namespace libabbauw
