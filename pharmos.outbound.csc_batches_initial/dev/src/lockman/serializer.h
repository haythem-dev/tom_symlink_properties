#ifndef GUARD_LIBLOCKMANAGER_SERIALIZATION_SERIALIZER_H
#define GUARD_LIBLOCKMANAGER_SERIALIZATION_SERIALIZER_H

#include "iserializable.h"

namespace libLockManager
{
namespace serialization
{

    class Serializer
    {
    public:
        Serializer();
        Serializer( const ISerializable::DataCollection& );

        ISerializable::DataCollection serialize() const;

        void add( const basar::VarString& identifier, const basar::VarString&  value );
        void add( const basar::VarString& identifier, const basar::I18nString& value );
        void add( const basar::VarString& identifier, const basar::Int32       value );
        void add( const basar::VarString& identifier, const basar::Decimal     value );
        void add( const basar::VarString& identifier, const basar::Float64     value );
        void add( const basar::VarString& identifier, const basar::Date&       value );
        void add( const basar::VarString& identifier, const basar::DateTime&   value );

    private:
        // forbidden
        Serializer( const Serializer& );
        Serializer& operator = ( const Serializer& );
        
    private:
        ISerializable::DataCollection m_DataCollection;
    };

} // end namespace serialization

} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_SERIALIZATION_SERIALIZER_H
