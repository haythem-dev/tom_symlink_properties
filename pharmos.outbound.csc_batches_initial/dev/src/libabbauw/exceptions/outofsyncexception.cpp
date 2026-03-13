#include "outofsyncexception.h"

namespace libabbauw
{
namespace exceptions
{

    OutOfSyncException::OutOfSyncException( const basar::ExceptInfo & sInfo ) 
    : BasarException( sInfo, "OutOfSyncException" )
    {
    }

    OutOfSyncException::~OutOfSyncException()
    {
    }

    const basar::VarString & OutOfSyncException::reason() const
    {
	    return m_InfoStruct.reason;
    }

} // end namespace exceptions
} // end namespace libabbauw
