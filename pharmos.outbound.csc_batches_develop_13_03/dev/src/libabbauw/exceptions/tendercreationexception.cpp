#include "tendercreationexception.h"

namespace libabbauw
{
namespace exceptions
{

    TenderCreationException::TenderCreationException( const basar::ExceptInfo & sInfo )
    : BasarException( sInfo, "TenderCreationException" )
    {
    }

    TenderCreationException::~TenderCreationException()
    {
    }

    const basar::VarString & TenderCreationException::reason() const
    {
        return m_InfoStruct.reason;
    }

} // end namespace exceptions
} // end namespace libabbauw
