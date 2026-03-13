#ifndef GUARD_LIBLOCKMANAGER_EXCEPTIONS_INVALIDINSTANCEEXCEPTION_H
#define GUARD_LIBLOCKMANAGER_EXCEPTIONS_INVALIDINSTANCEEXCEPTION_H

#include "baseexception.h"

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace libLockManager
{
namespace exceptions
{
	class InvalidInstanceException : public BaseException
	{
	public:
		InvalidInstanceException( const basar::ExceptInfo& );
		~InvalidInstanceException();

	private:
		InvalidInstanceException();					
	};
} // end namespace exceptions
} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_EXCEPTIONS_INVALIDINSTANCEEXCEPTION_H
