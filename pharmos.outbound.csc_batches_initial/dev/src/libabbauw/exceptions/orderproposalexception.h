#ifndef GUARD_LIBABBAUW_EXCEPTIONS_ORDERPROPOSALEXCEPTION_H
#define GUARD_LIBABBAUW_EXCEPTIONS_ORDERPROPOSALEXCEPTION_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_exceptions.h>

namespace libabbauw
{
namespace exceptions
{
    class OrderProposalException : public basar::Exception
    {
    public:
	    enum Error
	    {
		    ORDER_IS_TODAY,
            ORDER_IS_DONE,
			GENERAL
	    };

    public:
	    OrderProposalException( const basar::ExceptInfo & sInfo, OrderProposalException::Error error = OrderProposalException::GENERAL );
	    virtual ~OrderProposalException();

	    const basar::VarString & reason() const;
	    Error errorCode() const;

    private:
	    //!< forbidden
	    OrderProposalException();
	    Error m_error;
    };

} // end namespace exceptions
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_EXCEPTIONS_ORDERPROPOSALEXCEPTION_H
