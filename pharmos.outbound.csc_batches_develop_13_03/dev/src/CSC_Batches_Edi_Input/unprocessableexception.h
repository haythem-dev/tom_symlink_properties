//---------------------------------------------------------------------------//
/*! \file   
 *  \brief      exception indicating a failure with processing of an Edi-Input-Line
 *  \author     P. Kessling
 *  \date       20.01.13
 *  \version    00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#ifndef GUARD_LIBABBAUW_UNPROCESSABLE_EXCEPTION_H
#define GUARD_LIBABBAUW_UNPROCESSABLE_EXCEPTION_H

#include "abbauwexception.h"

namespace libabbauw
{

class UnprocessableException : public AbbauwException
{
public:
	/*! \brief std constructor */
	UnprocessableException( const basar::ExceptInfo & sInfo );
	/*! \brief std-destructor */
    ~UnprocessableException();

private:
	//!< forbidden
	UnprocessableException();					
};

} // end namespace libabbauw



#endif // GUARD_LIBABBAUW_UNPROCESSABLE_EXCEPTION_H