#include "ordersplitexception.h"

namespace libabbauw
{
namespace exceptions
{

    OrderSplitException::OrderSplitException( const basar::ExceptInfo& sInfo )
    : BasarException( sInfo, "OrderSplitException" )
    {
    }

    OrderSplitException::~OrderSplitException()
    {
    }

    const basar::VarString& OrderSplitException::reason() const
    {
	    return m_InfoStruct.reason;
    }

} // end namespace exceptions
} // end namespace libabbauw
