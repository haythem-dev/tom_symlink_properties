#include "key.h"

#include "apimisuseexception.h"
#include "metaclassmacro_definitions.h"
#include "serializer.h"

namespace libLockManager
{

    Key::Key( basar::Int32 id )
        : m_id( id ), m_branchNo( -1 )
    {
    }

    Key::Key( basar::Int32 id, basar::Int16 branchNo )
    : m_id( id ), m_branchNo( branchNo )
    {
    }

    Key::~Key()
    {
    }

    void Key::setIdentifierName( const basar::VarString& identifier )
    {
        m_identifier = identifier;
    }

    Key::DataCollection Key::serialize() const
    {
        METHODNAME_DEF( Key, serialize );
        if( m_identifier.empty() )
        {
            basar::VarString msg = "ID must not be empty! setIdentifierName() has not been called!";
            throw exceptions::APIMisuseException( basar::ExceptInfo( fun, msg, __FILE__, __LINE__ ) );
        }

        serialization::Serializer serializer;
        serializer.add( m_identifier, m_id );

        // only serialize branchNo if set
        if( m_branchNo > 0 )
        {
            serializer.add( "branchNo", m_branchNo );
        }

        return serializer.serialize();
    }

} // end namespace libLockManager
