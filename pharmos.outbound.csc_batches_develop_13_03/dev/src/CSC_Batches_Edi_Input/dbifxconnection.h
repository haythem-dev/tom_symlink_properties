//--------------------------------------------------------------------------------------------------//
/*! \file		dbifxconnection.h
 *  \brief		declaration of informix connection class
 *  \author     Johann Franz
 *  \date       09.08.2012
 *  \version	00.00.00.01 first version
 */ 
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_ABBAUW_DBCONNECTION_DBIFXCONNECTION_H
#define GUARD_ABBAUW_DBCONNECTION_DBIFXCONNECTION_H

#include <dbconnectionbase.h>

namespace abbaUW {
namespace dbConnection {

    class DBIfxConnection : public libBatch::dbConnection::DBConnectionBase
    {
        public:	
	        DBIfxConnection( const basar::db::sql::DatabaseInfo& );
	        virtual ~DBIfxConnection();
            virtual void connect();

        private:
            virtual basar::ConnectionTypeEnum getConnectionType() const;

	        DBIfxConnection( const DBIfxConnection& );
	        DBIfxConnection& operator = ( const DBIfxConnection& );	
	};

} // end namespace dbConnection
} // end namespace abbaUW

#endif // end GUARD_ABBAUW_DBCONNECTION_DBIFXCONNECTION_H
