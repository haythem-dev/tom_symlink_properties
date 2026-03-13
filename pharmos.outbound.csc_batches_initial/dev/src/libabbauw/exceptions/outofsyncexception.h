#ifndef GUARD_LIBABBAUW_EXCEPTIONS_OUTOFSYNCEXCEPTION_H
#define GUARD_LIBABBAUW_EXCEPTIONS_OUTOFSYNCEXCEPTION_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_exceptions.h>

namespace libabbauw
{
namespace exceptions
{
    class OutOfSyncException : public basar::Exception
    {
    public:
	    OutOfSyncException( const basar::ExceptInfo & sInfo );
	    virtual ~OutOfSyncException();

	    const basar::VarString & reason() const;

    private:
        // forbidden
	    OutOfSyncException();
    };

} // end namespace exceptions
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_EXCEPTIONS_OUTOFSYNCEXCEPTION_H
