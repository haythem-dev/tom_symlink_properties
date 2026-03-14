#include "serializer.h"

#include <libbasarcmnutil.h>

namespace libLockManager
{
namespace serialization
{
    Serializer::Serializer()
    {
    }

    Serializer::Serializer( const ISerializable::DataCollection& dataColl )
        : m_DataCollection( dataColl )
    {
    }

    ISerializable::DataCollection Serializer::serialize() const
    {
        return m_DataCollection;
    }

    void Serializer::add( const basar::VarString& identifier, const basar::VarString& value )
    {
        m_DataCollection[ identifier ] = "'" + value + "'";
    }

    void Serializer::add( const basar::VarString& identifier, const basar::I18nString& value )
    {
        m_DataCollection[ identifier ] = "'" + basar::VarString( value.c_str() ) + "'";
    }

    void Serializer::add( const basar::VarString& identifier, const basar::Int32 value )
    {
        basar::VarString svalue;
        svalue.itos( value );

        m_DataCollection[ identifier ] = svalue;
    }

    void Serializer::add( const basar::VarString& identifier, const basar::Decimal value )
    {
        m_DataCollection[ identifier ] = "'" + value.toString() + "'";
    }

    void Serializer::add( const basar::VarString& identifier, const basar::Float64 value )
    {
        basar::VarString svalue;
        svalue.ftos( value );

        m_DataCollection[ identifier ] = "'" + svalue + "'";
    }

    void Serializer::add( const basar::VarString& identifier, const basar::Date& value )
    {
        m_DataCollection[ identifier ] = "'" + value.toStrDate() + "'";
    }

    void Serializer::add( const basar::VarString& identifier, const basar::DateTime& value )
    {
        m_DataCollection[ identifier ] = "'" + value.toStrDateTime() + "'";
    }

} // end namespace serialization
} // end namespace libLockManager
