#ifndef GUARD_LIBABBAUW_EXCEPTIONS_TENDERCREATIONEXCEPTION_H
#define GUARD_LIBABBAUW_EXCEPTIONS_TENDERCREATIONEXCEPTION_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_exceptions.h>

namespace libabbauw
{
namespace exceptions
{
    class TenderCreationException : public basar::Exception
    {
    public:
        TenderCreationException( const basar::ExceptInfo & sInfo );
        virtual ~TenderCreationException();
        
        const basar::VarString & reason() const;
    private:
        TenderCreationException();
    };

} // end namespace exceptions
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_EXCEPTIONS_TENDERCREATIONEXCEPTION_H
