//---------------------------------------------------------------------------//
/*! \file		flatfileordirexception.h
 *  \brief      exception indicating a failure with handling of a flatfile or a directory for flatfiles
 *  \author     P. Kessling
 *  \date       04.04.13
 *  \version    00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#ifndef GUARD_LIBABBAUW_FLATFILEORDIR_EXCEPTION_H
#define GUARD_LIBABBAUW_FLATFILEORDIR_EXCEPTION_H

#include "abbauwexception.h"

namespace libabbauw
{

class FlatfileOrDirException : public AbbauwException
{
public:
	/*! \brief std constructor */
	FlatfileOrDirException( const basar::ExceptInfo & sInfo );
	/*! \brief std-destructor */
    ~FlatfileOrDirException();

private:
	//!< forbidden
	FlatfileOrDirException();					
};

} // end namespace libabbauw



#endif // GUARD_LIBABBAUW_FLATFILEORDIR_EXCEPTION_H