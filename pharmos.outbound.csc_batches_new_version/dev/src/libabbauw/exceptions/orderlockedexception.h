#ifndef GUARD_LIBABBAUW_EXCEPTIONS_ORDERLOCKEDEXCEPTION_H
#define GUARD_LIBABBAUW_EXCEPTIONS_ORDERLOCKEDEXCEPTION_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_exceptions.h>

namespace libabbauw
{
namespace exceptions
{
    class OrderLockedException : public basar::Exception
    {
    public:
        OrderLockedException( const basar::ExceptInfo & sInfo, basar::Int32 orderNo );
        virtual ~OrderLockedException();

        basar::VarString reason() const;
        basar::Int32 getOrderNo() const;

    private:
        // forbidden
        OrderLockedException();

    private:
        basar::Int32 m_orderNo;
    };

} // end namespace exceptions
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_EXCEPTIONS_ORDERLOCKEDEXCEPTION_H
