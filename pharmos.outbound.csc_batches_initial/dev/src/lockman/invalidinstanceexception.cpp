#include "invalidinstanceexception.h"

namespace libLockManager
{
namespace exceptions
{

	InvalidInstanceException::InvalidInstanceException( const basar::ExceptInfo& sInfo )
	: BaseException( sInfo, "InvalidInstanceException" )
	{
	}

	InvalidInstanceException::~InvalidInstanceException()
	{
	}

} // end namespace exceptions
} // end namespace libLockManager
