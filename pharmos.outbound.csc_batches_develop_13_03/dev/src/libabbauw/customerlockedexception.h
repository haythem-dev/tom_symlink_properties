#ifndef GUARD_LIBABBAUW_EXCEPTIONS_CUSTOMERLOCKEDEXCEPTION_H
#define GUARD_LIBABBAUW_EXCEPTIONS_CUSTOMERLOCKEDEXCEPTION_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_exceptions.h>

namespace libabbauw
{
namespace exceptions
{
class CustomerLockedException : public basar::Exception
{
public:
	CustomerLockedException( const basar::ExceptInfo & sInfo );
	virtual ~CustomerLockedException();

	const basar::VarString & reason() const;

private:
    // forbidden
	CustomerLockedException();
};

} // end namespace exceptions
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_EXCEPTIONS_CUSTOMERLOCKEDEXCEPTION_H
