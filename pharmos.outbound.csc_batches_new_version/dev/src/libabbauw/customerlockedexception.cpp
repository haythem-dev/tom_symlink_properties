#include "customerlockedexception.h"

namespace libabbauw
{
namespace exceptions
{

CustomerLockedException::CustomerLockedException( const basar::ExceptInfo & sInfo ) 
: BasarException( sInfo, "CustomerLockedException" )
{
}

CustomerLockedException::~CustomerLockedException()
{
}

const basar::VarString & CustomerLockedException::reason() const
{
	return m_InfoStruct.reason;
}

} // end namespace exceptions
} // end namespace libabbauw
