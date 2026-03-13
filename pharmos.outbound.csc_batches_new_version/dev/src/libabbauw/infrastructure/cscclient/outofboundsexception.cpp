#include "outofboundsexception.h"

namespace libabbauw
{
namespace infrastructure
{
namespace cscClient
{
    OutOfBoundsException::OutOfBoundsException( const basar::ExceptInfo& sInfo )
        : CSCClientException( sInfo, "OutOfBoundsException" )
    {
    }
    
    OutOfBoundsException::~OutOfBoundsException()
    {
    }

} // end namespace cscClient
} // end namespace infrastructure
} // end namespace libabbauw
