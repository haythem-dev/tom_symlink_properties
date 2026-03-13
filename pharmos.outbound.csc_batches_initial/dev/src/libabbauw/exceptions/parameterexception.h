#ifndef GUARD_LIBABBAUW_EXCEPTIONS_PARAMETEREXCEPTION_H
#define GUARD_LIBABBAUW_EXCEPTIONS_PARAMETEREXCEPTION_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_exceptions.h>

namespace libabbauw
{
namespace exceptions
{

    class ParameterException : public basar::Exception
    {
    public:
	    /*! \brief std constructor */
	    ParameterException( const basar::ExceptInfo & sInfo, const basar::VarString& param );
	    /*! \brief std-destructor */
	    virtual ~ParameterException();

	    const basar::VarString & reason() const;
	    const basar::VarString & wrongParamName() const;

    private:
	    //!< forbidden
	    ParameterException();

    private:
	    basar::VarString       m_ParamName;
    };

} // end namespace exceptions
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_EXCEPTIONS_PARAMETEREXCEPTION_H
