#include "alreadylockedexception.h"

//-----------------------------------------------------------------------------------------------//
// namespace section
//-----------------------------------------------------------------------------------------------//
namespace libLockManager
{
namespace exceptions
{

	AlreadyLockedException::AlreadyLockedException( const basar::ExceptInfo& sInfo )
	: libLockManager::exceptions::BaseException( sInfo, "AlreadyLockedException" )
	{
	}

	AlreadyLockedException::~AlreadyLockedException()
	{
	}

} // end namespace exceptions
} // end namespace libLockManager
