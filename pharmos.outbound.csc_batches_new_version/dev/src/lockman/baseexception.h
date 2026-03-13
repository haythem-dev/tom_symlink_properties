#ifndef GUARD_LIBLOCKMANGER_EXCEPTIONS_BASE_EXCEPTION_H
#define GUARD_LIBLOCKMANGER_EXCEPTIONS_BASE_EXCEPTION_H

#include <libbasarcmnutil_exceptions.h>

namespace libLockManager
{
namespace exceptions
{
	class BaseException : public basar::cmnutil::BasarException
	{
	public:
		BaseException( const basar::ExceptInfo & sInfo, const basar::VarString & exceptionType );
		virtual ~BaseException();

		const basar::VarString & reason() const;

	private:
		BaseException();
	};
} // end namespace exceptions
} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANGER_EXCEPTIONS_BASE_EXCEPTION_H
