//---------------------------------------------------------------------------//
/*! \file   
 *  \brief      exception indicating a failure with the storing of EDI data in DB
 *  \author     P. Kessling
 *  \date       12.06.13
 *  \version    00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#ifndef GUARD_LIBABBAUW_DATA_STORING_EXCEPTION_H
#define GUARD_LIBABBAUW_DATA_STORING_EXCEPTION_H

#include "abbauwexception.h"

namespace libabbauw
{

class DataStoringException : public AbbauwException
{
public:
	/*! \brief std constructor */
	DataStoringException( const basar::ExceptInfo & sInfo );
	/*! \brief std-destructor */
    ~DataStoringException();

private:
	//!< forbidden
	DataStoringException();					
};

} // end namespace libabbauw



#endif // GUARD_LIBABBAUW_DATA_STORING_EXCEPTION_H