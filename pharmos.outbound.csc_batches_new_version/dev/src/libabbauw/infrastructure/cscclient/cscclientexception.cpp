#include "cscclientexception.h"

namespace libabbauw
{
namespace infrastructure
{
namespace cscClient
{
    CSCClientException::CSCClientException( const basar::ExceptInfo& sInfo, const basar::VarString& exceptionType )
        : BasarException( sInfo, exceptionType )
    {
    }
    
    CSCClientException::~CSCClientException()
    {
    }

    const basar::VarString& CSCClientException::reason() const
    {
        return m_InfoStruct.reason;
    }

} // end namespace cscClient
} // end namespace infrastructure
} // end namespace libabbauw
