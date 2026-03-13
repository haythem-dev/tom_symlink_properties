//---------------------------------------------------------------------------//
/*! \file filenotopenableexception.cpp
 *  \briefexception thrown if parameter is not set in database
 *  \author 
 *  \date 
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#include "filenotwritableexception.h"


namespace print
{
namespace exceptions
{
FileNotWritableException::FileNotWritableException( const basar::ExceptInfo & sInfo, const basar::VarString& fileName ) 
: BasarException( sInfo, "FileNotWritableException" ), m_FileName( fileName )
{
}

FileNotWritableException::~FileNotWritableException()
{
}

const basar::VarString & FileNotWritableException::reason() const
{
	return m_InfoStruct.reason;
}

const basar::VarString & FileNotWritableException::fileName() const
{
	return m_FileName;
}

} // end namespace exceptions
} // end namespace print
