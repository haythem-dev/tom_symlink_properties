#ifndef GUARD_LIBTENDER_FILE_OPEN_EXCEPTION_H
#define GUARD_LIBTENDER_FILE_OPEN_EXCEPTION_H

#include <libutil/exception.h>

namespace libtender
{
class FileOpenException : public libutil::exceptions::BaseException
{
public:
	FileOpenException( const basar::ExceptInfo & sInfo );
	virtual ~FileOpenException();
};

} // end namespace libtender

#endif // GUARD_LIBTENDER_FILE_OPEN_EXCEPTION_H

