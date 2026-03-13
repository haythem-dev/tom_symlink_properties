#include "fileopenexception.h"

namespace libtender
{
FileOpenException::FileOpenException( const basar::ExceptInfo & sInfo )
: libutil::exceptions::BaseException( sInfo, "FileOpenException" )
{
}

FileOpenException::~FileOpenException()
{
}

} // end namespace libtender


