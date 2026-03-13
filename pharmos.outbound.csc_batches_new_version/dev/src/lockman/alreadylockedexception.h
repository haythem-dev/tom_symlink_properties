#ifndef GUARD_LIBLOCKMANAGER_EXCEPTIONS_ALREADYLOCKEDEXCEPTION_H
#define GUARD_LIBLOCKMANAGER_EXCEPTIONS_ALREADYLOCKEDEXCEPTION_H

#include "baseexception.h"

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace libLockManager
{
namespace exceptions
{
	class AlreadyLockedException : public libLockManager::exceptions::BaseException
	{
		public:
			AlreadyLockedException( const basar::ExceptInfo& );
			~AlreadyLockedException();

		private:
			AlreadyLockedException();					
	};
} // end namespace exceptions
} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_EXCEPTIONS_ALREADYLOCKEDEXCEPTION_H
