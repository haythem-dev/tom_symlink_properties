#ifndef GUARD_LIBLOCKMANAGER_ACCESSOR_LOCKACCESSOR_H
#define GUARD_LIBLOCKMANAGER_ACCESSOR_LOCKACCESSOR_H

#include "ilockaccessor.h"
//#include "iconstconfigurationptr.h"

#include "ikeyptr.h"
#include "ilockentryptr.h"

#include <libbasarcmnutil_bstring.h>
#include <libbasardbsql_connectionref.h>

//---------------------------------------------------------------------------//
// forward declaration section 
//---------------------------------------------------------------------------//
namespace log4cplus
{
    class Logger;
}

//---------------------------------------------------------------------------//
// class definition section
//---------------------------------------------------------------------------//
namespace libLockManager
{
namespace accessor
{

    class LockAccessor : public ILockAccessor
    {
    public:
        LockAccessor( const basar::VarString& tableName );
        ~LockAccessor();

        void                                        injectConnection( basar::db::sql::ConnectionRef );

    // from ILockAccessor interface
    public:
        bool                                        findByKey( const IKey&       );
        void                                        save     ( const ILockEntry& );
        void                                        erase    ( const IKey&       );

        void                                        eraseStaleLocks();

    private:
        // don't allow copying of this class
        LockAccessor( const LockAccessor& );
        LockAccessor& operator = ( const LockAccessor& );

        basar::db::sql::ConnectionRef               getConnection();
        void                                        checkForTransaction();

    private:
        const log4cplus::Logger&                    m_Logger;
        basar::db::sql::ConnectionRef               m_Connection;
        basar::VarString                            m_TableName;
    };

} // end namespace accessor
} // end namespace libLockManager

#endif // GUARD_LIBLOCKMANAGER_ACCESSOR_LOCKACCESSOR_H
