#ifndef GUARD_LIBABBAUW_EXCEPTIONS_ORDERNOTFOUNDEXCEPTION_H
#define GUARD_LIBABBAUW_EXCEPTIONS_ORDERNOTFOUNDEXCEPTION_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_exceptions.h>

namespace libabbauw
{
namespace exceptions
{

    class OrderNotFoundException : public basar::Exception
    {
    public:
	    OrderNotFoundException( const basar::ExceptInfo & sInfo );
	    virtual ~OrderNotFoundException();

	    const basar::VarString & reason() const;

    private:
        // forbidden
	    OrderNotFoundException();
    };

} // end namespace exceptions
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_EXCEPTIONS_ORDERNOTFOUNDEXCEPTION_H
