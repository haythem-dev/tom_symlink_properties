// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch			(ToDo kes: ???)
#include "cmnexcept.h"

#include "abbauwexception.h"

namespace libabbauw
{

AbbauwException::AbbauwException( const basar::ExceptInfo & sInfo, const basar::VarString & exceptionType )
: BasarException( sInfo, exceptionType )
{
}

AbbauwException::~AbbauwException()
{
}

} // end namespace libabbauw


