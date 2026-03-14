//---------------------------------------------------------------------------//
/*! \file   
 *  \brief      exception indicating a failure with the storing of EDI data in DB
 *  \author     Petra Kessling
 *  \date       11.06.13
 *  \version    00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// include section
//---------------------------------------------------------------------------//
// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include "cmnexcept.h"

#include "datastoringexception.h"

namespace libabbauw
{

DataStoringException::DataStoringException( const basar::ExceptInfo & sInfo )
: AbbauwException( sInfo, "DataStoringException" )
{
}

DataStoringException::~DataStoringException()
{
}

} // end namespace libabbauw

