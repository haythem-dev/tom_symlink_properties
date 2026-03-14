//---------------------------------------------------------------------------//
/*! \file   
 *  \brief      exception indicating a failure with processing of an Edi-Input-Line
 *  \author     Petra Kessling
 *  \date       20.03.13
 *  \version    00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// include section
//---------------------------------------------------------------------------//
// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include "cmnexcept.h"

#include "unprocessableexception.h"

namespace libabbauw
{

UnprocessableException::UnprocessableException( const basar::ExceptInfo & sInfo )
: AbbauwException( sInfo, "UnprocessableException" )
{
}

UnprocessableException::~UnprocessableException()
{
}

} // end namespace libabbauw

