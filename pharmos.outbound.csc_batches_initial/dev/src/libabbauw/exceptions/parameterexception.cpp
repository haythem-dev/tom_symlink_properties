#include "parameterexception.h"

namespace libabbauw
{
namespace exceptions
{

    ParameterException::ParameterException( const basar::ExceptInfo & sInfo, const basar::VarString& param ) 
    : BasarException( sInfo, "ParameterException" ), m_ParamName( param )
    {
    }

    ParameterException::~ParameterException()
    {
    }

    const basar::VarString & ParameterException::reason() const
    {
	    return m_InfoStruct.reason;
    }

    const basar::VarString & ParameterException::wrongParamName() const
    {
	    return m_ParamName;
    }

} // end namespace exceptions
} // end namespace libabbauw
