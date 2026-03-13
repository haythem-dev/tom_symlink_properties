#include "lockaccessorexception.h"

//-----------------------------------------------------------------------------------------------//
// namespace section
//-----------------------------------------------------------------------------------------------//
namespace libLockManager
{
namespace accessor
{
namespace exceptions
{

	LockAccessorException::LockAccessorException( const basar::ExceptInfo& sInfo )
	: libLockManager::exceptions::BaseException( sInfo, "LockAccessorException" )
	{
	}

	LockAccessorException::~LockAccessorException()
	{
	}

} // end namespace exceptions
} // end namespace accessor
} // end namespace libLockManager
