#include "orderproposalexception.h"

namespace libabbauw
{
namespace exceptions
{

    OrderProposalException::OrderProposalException( const basar::ExceptInfo & sInfo, OrderProposalException::Error error ) 
		: BasarException( sInfo, "OrderProposalException" ), m_error( error )
    {
    }

    OrderProposalException::~OrderProposalException()
    {
    }

    const basar::VarString & OrderProposalException::reason() const
    {
	    return m_InfoStruct.reason;
    }

    OrderProposalException::Error OrderProposalException::errorCode() const
    {
	    return m_error;
    }

} // end namespace exceptions
} // end namespace libabbauw
