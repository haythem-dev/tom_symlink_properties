#include "orderlockedexception.h"

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning(disable: 4127 4512 )
#endif
#include <QtCore/QString>
#include <QtCore/QCoreApplication>
#ifdef _MSC_VER
#pragma warning (pop)
#endif

namespace libabbauw
{
namespace exceptions
{

    OrderLockedException::OrderLockedException( const basar::ExceptInfo & sInfo, basar::Int32 orderNo ) 
        : BasarException( sInfo, "OrderLockedException" ), m_orderNo( orderNo )
    {
    }

    OrderLockedException::~OrderLockedException()
    {
    }

    basar::VarString OrderLockedException::reason() const
    {
        QString reason = QCoreApplication::translate( "OrderLockedException", "Order '%1' is locked!" ).arg( m_orderNo );
        return reason.toLocal8Bit().constData();
    }

    basar::Int32 OrderLockedException::getOrderNo() const
    {
        return m_orderNo;
    }

} // end namespace exceptions
} // end namespace libabbauw
