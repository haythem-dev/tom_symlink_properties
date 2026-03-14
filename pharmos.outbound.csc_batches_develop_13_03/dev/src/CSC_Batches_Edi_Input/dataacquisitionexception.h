//---------------------------------------------------------------------------//
/*! \file   
 *  \brief      exception indicating a failure with the acquisition of additional data
 *  \author     P. Kessling
 *  \date       12.06.13
 *  \version    00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#ifndef GUARD_LIBABBAUW_DATA_ACQUISITION_EXCEPTION_H
#define GUARD_LIBABBAUW_DATA_ACQUISITION_EXCEPTION_H

#include "abbauwexception.h"

namespace libabbauw
{

class DataAcquisitionException : public AbbauwException
{
public:
	/*! \brief std constructor */
	DataAcquisitionException( const basar::ExceptInfo & sInfo );
	/*! \brief std-destructor */
    ~DataAcquisitionException();

private:
	//!< forbidden
	DataAcquisitionException();					
};

} // end namespace libabbauw



#endif // GUARD_LIBABBAUW_DATA_ACQUISITION_EXCEPTION_H