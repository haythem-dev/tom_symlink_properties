//---------------------------------------------------------------------------//
/*! \file   
 *  \brief      exception indicating a failure with the acquisition of additional data
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

#include "dataacquisitionexception.h"

namespace libabbauw
{

DataAcquisitionException::DataAcquisitionException( const basar::ExceptInfo & sInfo )
: AbbauwException( sInfo, "DataAcquisitionException" )
{
}

DataAcquisitionException::~DataAcquisitionException()
{
}

} // end namespace libabbauw

