#include "baseexception.h"

namespace libLockManager
{
namespace exceptions
{

	BaseException::BaseException( const basar::ExceptInfo & sInfo, const basar::VarString & exceptionType )
	: BasarException( sInfo, exceptionType )
	{
	}

	BaseException::~BaseException()
	{
	}

	const basar::VarString& BaseException::reason() const
	{
		return m_InfoStruct.reason;
	}

} // end namespace exceptions
} // end namespace libLockManager
