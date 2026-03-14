#ifndef GUARD_LIBABBAUW_EXCEPTIONS_ORDERSPLITEXCEPTION_H
#define GUARD_LIBABBAUW_EXCEPTIONS_ORDERSPLITEXCEPTION_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_exceptions.h>

namespace libabbauw
{
namespace exceptions
{
    class OrderSplitException : public basar::Exception
    {
    public:
        OrderSplitException( const basar::ExceptInfo & sInfo );
        virtual ~OrderSplitException();

        const basar::VarString& reason() const;

    private:
        // forbidden
        OrderSplitException();
    };

} // end namespace exceptions
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_EXCEPTIONS_ORDERSPLITEXCEPTION_H