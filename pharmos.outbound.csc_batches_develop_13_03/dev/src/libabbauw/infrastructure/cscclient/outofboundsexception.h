#ifndef GUARD_LIBABBAUW_INFRASTRUCTURE_CSCCLIENT_OUTOFBOUNDSEXCEPTION_H
#define GUARD_LIBABBAUW_INFRASTRUCTURE_CSCCLIENT_OUTOFBOUNDSEXCEPTION_H

#include "cscclientexception.h"

namespace libabbauw
{
namespace infrastructure
{
namespace cscClient
{
    class OutOfBoundsException : public CSCClientException
    {
    public:
        OutOfBoundsException( const basar::ExceptInfo& sInfo );
        virtual ~OutOfBoundsException();

    private:
        // forbidden
        OutOfBoundsException();
    };
} // end namespace cscClient
} // end namespace infrastructure
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_INFRASTRUCTURE_CSCCLIENT_OUTOFBOUNDSEXCEPTION_H
