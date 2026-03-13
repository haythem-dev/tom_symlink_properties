#include "orderreleaseexception.h"

namespace libabbauw
{
namespace exceptions
{

    OrderReleaseException::OrderReleaseException( const basar::ExceptInfo & sInfo, OrderReleaseException::Error error ) 
    : BasarException( sInfo, "OrderReleaseException" ), m_error( error )
    {
    }

    OrderReleaseException::~OrderReleaseException()
    {
    }

    const basar::VarString & OrderReleaseException::reason() const
    {
	    return m_InfoStruct.reason;
    }

    OrderReleaseException::Error OrderReleaseException::errorCode() const
    {
	    return m_error;
    }

} // end namespace exceptions
} // end namespace libabbauw
