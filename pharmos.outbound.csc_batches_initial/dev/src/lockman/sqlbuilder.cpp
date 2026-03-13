#include "sqlbuilder.h"

#include "iserializable.h"

namespace libLockManager
{
namespace accessor
{
namespace util
{

    SQLBuilder::SQLBuilder( const basar::VarString& tableName )
        : m_TableName( tableName )
    {
    }

    SQLBuilder::~SQLBuilder()
    {
    }

    basar::VarString SQLBuilder::whereClauseBuilder( const ISerializable& serializable ) const
    {
        basar::VarString whereClause;

        ISerializable::DataCollection data = serializable.serialize();
        for( ISerializable::DataCollection::const_iterator it = data.begin(); it != data.end(); ++it )
        {
            if( it != data.begin() )
            {
                whereClause.append( " AND " );
            }
            whereClause.append( it->first + " = " + it->second );
        }

        return whereClause;
    }

    basar::VarString SQLBuilder::createInsertStatement( const ISerializable& serializable ) const
    {
        basar::VarString sql = "INSERT INTO " + m_TableName;

        basar::VarString columns = "( ", values = " VALUES( ";

        ISerializable::DataCollection data = serializable.serialize();
        for( ISerializable::DataCollection::const_iterator it = data.begin(); it != data.end(); ++it )
        {
            columns.append( it->first  ).append( ", " );
            values.append ( it->second ).append( ", " );
        }
        columns.append( "sessionid )" );
        values.append ( "dbinfo('sessionid') )" );

        sql.append( columns + values );
        return sql;
    }

    basar::VarString SQLBuilder::createDeleteStatement( const ISerializable& serializable ) const
    {
        basar::VarString sql = "DELETE FROM " + m_TableName + " WHERE ";

        sql.append( whereClauseBuilder( serializable ) );
        return sql;
    }

    basar::VarString SQLBuilder::createCountStatement( const ISerializable& serializable ) const
    {
        // stale locks will be filtered out
        basar::VarString sql = "SELECT COUNT(*)::INT FROM " + m_TableName + " l "
                               "JOIN sysmaster:syssessions s ON l.sessionid = s.sid "
                               "WHERE " + whereClauseBuilder( serializable );
        
        return sql;
    }

} // end namespace util
} // end namespace accessor
} // end namespace libLockManager
