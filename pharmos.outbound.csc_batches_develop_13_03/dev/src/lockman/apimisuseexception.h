#ifndef GUARD_LIBUTIL_EXCEPTIONS_APIMISUSEEXCEPTION_H
#define GUARD_LIBUTIL_EXCEPTIONS_APIMISUSEEXCEPTION_H

#include "baseexception.h"

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace libLockManager
{
namespace exceptions
{
	class APIMisuseException : public BaseException
	{
	public:
		APIMisuseException( const basar::ExceptInfo & sInfo );
		~APIMisuseException();

	private:
		APIMisuseException();
	};
} // end namespace exceptions
} // end namespace libLockManager

#endif // GUARD_LIBUTIL_EXCEPTIONS_APIMISUSEEXCEPTION_H
