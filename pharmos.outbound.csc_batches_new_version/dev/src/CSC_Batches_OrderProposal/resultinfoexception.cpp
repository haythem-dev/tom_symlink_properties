// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch			(ToDo kes: ???)

#include "cmnexcept.h"
#include "resultinfoexception.h"

namespace libabbauw
{

ResultInfoException::ResultInfoException( const basar::ExceptInfo & sInfo)
: AbbauwException( sInfo, "BadResultInfo" )
{
}

ResultInfoException::~ResultInfoException()
{
}

} // end namespace libabbauw


