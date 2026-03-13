//---------------------------------------------------------------------------//
/*! \file		flatfileordirexception.cpp
 *  \brief      exception indicating a failure with handling of a flatfile or a directory for flatfiles
 *  \author     Petra Kessling
 *  \date       04.04.13
 *  \version    00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// include section
//---------------------------------------------------------------------------//
// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include "cmnexcept.h"

#include "flatfileordirexception.h"

namespace libabbauw
{

FlatfileOrDirException::FlatfileOrDirException( const basar::ExceptInfo & sInfo )
: AbbauwException( sInfo, "FlatfileOrDirException" )
{
}

FlatfileOrDirException::~FlatfileOrDirException()
{
}

} // end namespace libabbauw

