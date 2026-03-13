#ifndef GUARD_LIBLOCKMANAGER_ACCESSOR_EXCEPTIONS_LOCKACCESSOREXCEPTION_H
#define GUARD_LIBLOCKMANAGER_ACCESSOR_EXCEPTIONS_LOCKACCESSOREXCEPTION_H

#include "baseexception.h"

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace libLockManager
{
namespace accessor
{
namespace exceptions
{
	class LockAccessorException : public libLockManager::exceptions::BaseException
	{
		public:
			LockAccessorException( const basar::ExceptInfo& );
			~LockAccessorException();

		private:
			LockAccessorException();					
	};
} // end namespace exceptions
} // end namespace accessor
} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ACCESSOR_EXCEPTIONS_LOCKACCESSOREXCEPTION_H
