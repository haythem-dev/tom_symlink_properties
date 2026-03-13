//---------------------------------------------------------------------------//
/*! \file filenotopenableexception.h
 *  \brief exception thrown if file could not be opened
 *  \author 
 *  \date 
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#ifndef GUARD_PRINT_FILENOTWRITABLEEXCEPTION_H
#define GUARD_PRINT_FILENOTWRITABLEEXCEPTION_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_exceptions.h>

namespace print
{
namespace exceptions
{
class FileNotWritableException : public basar::Exception
{
public:
	/*! \brief std constructor */
	FileNotWritableException( const basar::ExceptInfo & sInfo, const basar::VarString& fileName );
	/*! \brief std-destructor */
	virtual ~FileNotWritableException();

	const basar::VarString & reason() const;
	const basar::VarString & fileName() const;

private:
	//!< forbidden
	FileNotWritableException();

private:
	basar::VarString m_FileName;
};

} // end namespace exceptions
} // end namespace print

#endif // GUARD_PRINT_FILENOTWRITABLEEXCEPTION_H

