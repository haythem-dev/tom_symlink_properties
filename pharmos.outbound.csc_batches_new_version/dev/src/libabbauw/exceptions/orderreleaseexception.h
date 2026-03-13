#ifndef GUARD_LIBABBAUW_EXCEPTIONS_ORDERRELEASEEXCEPTION_H
#define GUARD_LIBABBAUW_EXCEPTIONS_ORDERRELEASEEXCEPTION_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_exceptions.h>

namespace libabbauw
{
namespace exceptions
{
    class OrderReleaseException : public basar::Exception
    {
    public:
	    enum Error
	    {
		    KSC_ERROR,
		    ORDER_HAS_PURPLE_COLOR,
            ORDER_HAS_NO_COLOR,
            ORDER_CONTAINS_NON_STOCK_ITEM,
            WRONG_ORDER_STATUS
	    };

    public:
	    OrderReleaseException( const basar::ExceptInfo & sInfo, OrderReleaseException::Error error = OrderReleaseException::KSC_ERROR );
	    virtual ~OrderReleaseException();

	    const basar::VarString & reason() const;
	    Error errorCode() const;

    private:
	    //!< forbidden
	    OrderReleaseException();
	    Error m_error;
    };

} // end namespace exceptions
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_EXCEPTIONS_ORDERRELEASEEXCEPTION_H
