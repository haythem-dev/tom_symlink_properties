#include "apimisuseexception.h"

namespace libLockManager
{
namespace exceptions
{

	APIMisuseException::APIMisuseException( const basar::ExceptInfo& sInfo )
	: BaseException( sInfo, "APIMisuseException" )
	{
	}

	APIMisuseException::~APIMisuseException()
	{
	}

} // end namespace exceptions
} // end namespace libLockManager
