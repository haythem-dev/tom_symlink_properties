//---------------------------------------------------------------------------//
/*! \file filenotopenableexception.cpp
 *  \briefexception thrown if parameter is not set in database
 *  \author 
 *  \date 
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#include "filenotopenableexception.h"


namespace print
{
namespace exceptions
{
FileNotOpenableException::FileNotOpenableException( const basar::ExceptInfo & sInfo, const basar::VarString& fileName ) 
: BasarException( sInfo, "FileNotOpenableException" ), m_FileName( fileName )
{
}

FileNotOpenableException::~FileNotOpenableException()
{
}

const basar::VarString & FileNotOpenableException::reason() const
{
	return m_InfoStruct.reason;
}

const basar::VarString & FileNotOpenableException::fileName() const
{
	return m_FileName;
}

} // end namespace exceptions
} // end namespace print
