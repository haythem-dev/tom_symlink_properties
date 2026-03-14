#ifndef GUARD_LIBABBAUW_INFRASTRUCTURE_CSCCLIENT_CSCCLIENTEXCEPTION_H
#define GUARD_LIBABBAUW_INFRASTRUCTURE_CSCCLIENT_CSCCLIENTEXCEPTION_H

#include <libbasarcmnutil_exceptions.h>

namespace libabbauw
{
namespace infrastructure
{
namespace cscClient
{
    class CSCClientException : public basar::cmnutil::BasarException
    {
    public:
        CSCClientException( const basar::ExceptInfo& sInfo, const basar::VarString& exceptionType );
        virtual ~CSCClientException();

        const basar::VarString& reason() const;

    private:
        // forbidden
        CSCClientException();
    };
} // end namespace cscClient
} // end namespace infrastructure
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_INFRASTRUCTURE_CSCCLIENT_CSCCLIENTEXCEPTION_H
