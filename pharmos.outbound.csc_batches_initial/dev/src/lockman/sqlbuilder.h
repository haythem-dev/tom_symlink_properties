#ifndef GUARD_LIBLOCKMANAGER_ACCESSOR_UTIL_SQLBUILDER_H
#define GUARD_LIBLOCKMANAGER_ACCESSOR_UTIL_SQLBUILDER_H

#include <libbasar_definitions.h>
#include <libbasarcmnutil_bstring.h>

//---------------------------------------------------------------------------//
// forward declaration section
//---------------------------------------------------------------------------//
namespace libLockManager
{
    class ISerializable;
}

//---------------------------------------------------------------------------//
// class definition section
//---------------------------------------------------------------------------//
namespace libLockManager
{
namespace accessor
{
namespace util
{
    class SQLBuilder
    {
    public:
        explicit SQLBuilder( const basar::VarString& tableName );
        ~SQLBuilder();

        basar::VarString createInsertStatement     ( const ISerializable& ) const;
        basar::VarString createDeleteStatement     ( const ISerializable& ) const;
        basar::VarString createCountStatement      ( const ISerializable& ) const;

    private:
        // forbidden
        SQLBuilder();
        SQLBuilder( const SQLBuilder& );
        SQLBuilder& operator = ( const SQLBuilder& );

        basar::VarString whereClauseBuilder( const ISerializable& ) const;

    private:
        basar::VarString m_TableName;
    };
} // end namespace util
} // end namespace accessor
} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ACCESSOR_UTIL_SQLBUILDER_H
